#include "plpch.h"
#include <Networking/Networking.hpp>

using namespace pl;

bool PLServer::Initialize(IPEndpoint ip)
{
    if (!Networking::InitializeWinsock())
        return false;

    p_listeningSocket = Socket(ip.GetIpVersion());
    if (p_listeningSocket.Create() != SocketResult::Success) {
        spdlog::critical("Could not create PLServer listening socket, shutting Down");
        ip.Print();
        Clear();
        return false;
    }

    if (p_listeningSocket.Listen(ip, 5) != SocketResult::Success) {
        spdlog::critical("Listening socket could not listen, shutting Down");
        ip.Print();
        Clear();
        return false;
    }

    WSAPOLLFD listeningSocketFD = {};
    listeningSocketFD.fd = p_listeningSocket.GetHandle();
    listeningSocketFD.events = POLLRDNORM;
    listeningSocketFD.revents = 0;

    p_masterFd.push_back(listeningSocketFD);

    spdlog::info("Successfully listening");
    ip.Print();
    p_running = true;
    return true;
}

void PLServer::Frame()
{
    p_useFd = p_masterFd;

    //spdlog::info("clients: {}", p_connections.size());

    if (WSAPoll(p_useFd.data(), p_useFd.size(), TIMEOUT) > 0) {
        UpdateListen();
        UpdateConnections();
    }
}

void PLServer::OnConnect(TCPConnection& newConnection)
{
    spdlog::info("New connection accepted {}", newConnection.ToString());
}

void PLServer::OnDisconnect(TCPConnection& lostConnection, std::string reason)
{
    spdlog::warn("{} has lost connection: {}", lostConnection.ToString(), reason);
}

bool PLServer::ProcessPacket(Packet& packet) {
    spdlog::info("Packet received with size: {}",packet.buffer.size());
    return true;
}

void PLServer::UpdateListen()
{
    WSAPOLLFD& listeningSocketFD = p_useFd[0];
    if (listeningSocketFD.revents & POLLRDNORM) {
        Socket newConnection;
        IPEndpoint newConnectionEndpoint;
        if (p_listeningSocket.Accept(newConnection, &newConnectionEndpoint) == SocketResult::Success) {
            p_connections.emplace_back(TCPConnection(newConnection, newConnectionEndpoint));
            auto& acceptedConnection = p_connections.back();
            OnConnect(acceptedConnection);
            WSAPOLLFD newConnectionFD = {};
            newConnectionFD.fd = newConnection.GetHandle();
            newConnectionFD.events = POLLRDNORM | POLLWRNORM;
            newConnectionFD.revents = 0;
            p_masterFd.push_back(newConnectionFD);
        }
        else {
            spdlog::error("Failed to accept new connection");
            newConnectionEndpoint.Print();
        }
    }
}

void PLServer::UpdateConnections()
{
    //for each connection we check if we can receive data/send data
    for (int i = p_useFd.size() - 1; i >= 1; i--) {
        int connectionIndex = i - 1;
        auto& connection = p_connections[connectionIndex];

        if (p_useFd[i].revents & POLLERR) {
            CloseConnection(connectionIndex, "POLLERR");
            continue;
        }

        if (p_useFd[i].revents & POLLHUP) {
            CloseConnection(connectionIndex, "POLLHUP");
            continue;
        }

        if (p_useFd[i].revents & POLLNVAL) {
            CloseConnection(connectionIndex, "POLLNVAL");
            continue;
        }

        if (p_useFd[i].revents & POLLRDNORM) //PLServer reading data from connections
            if (!ReadFromConnection(connectionIndex))
                continue;

        if (p_useFd[i].revents & POLLWRNORM)//PLServer sending data to connections
            SendToConnection(connectionIndex);
    }

    for (int i = p_connections.size() - 1; i >= 0; i--) { //PLServer procesing packets that arrived from connections
        while (p_connections[i].pmIncoming.HasPendingPackets()) {
            Packet* frontPacket = p_connections[i].pmIncoming.Retrieve();
            if (!ProcessPacket(*frontPacket)) {
                CloseConnection(i, "Failed to process incoming packet");
                break;
            }
            p_connections[i].pmIncoming.Pop();
        }
    }
}

bool PLServer::ReadFromConnection(int connectionIndex)
{
    auto& connection = p_connections[connectionIndex];
    int i = connectionIndex + 1;
    int bytesReceived = 0;

    if (connection.pmIncoming.currentTask == PacketManagerTask::ProcessPacketSize) {
        bytesReceived = recv(
            p_useFd[i].fd,
            (char*)&connection.pmIncoming.currentPacketSize + connection.pmIncoming.currentPacketExtractionOffset,
            sizeof(uint16_t) - connection.pmIncoming.currentPacketExtractionOffset, 0);
    }
    else {
        bytesReceived = recv(
            p_useFd[i].fd,
            (char*)&connection.buffer + connection.pmIncoming.currentPacketExtractionOffset,
            connection.pmIncoming.currentPacketSize - connection.pmIncoming.currentPacketExtractionOffset, 0);
    }


    if (bytesReceived == 0) {
        CloseConnection(connectionIndex, "[Recv==0]Connection lost");
        return false;
    }
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            CloseConnection(connectionIndex, "[Recv<0]Connection lost: ");
            return false;
        }
        return false;
    }

    if (bytesReceived > 0) {
        connection.pmIncoming.currentPacketExtractionOffset += bytesReceived;
        if (connection.pmIncoming.currentTask == PacketManagerTask::ProcessPacketSize)
        {
            if (connection.pmIncoming.currentPacketExtractionOffset == sizeof(uint16_t)) {
                connection.pmIncoming.currentPacketSize = ntohs(connection.pmIncoming.currentPacketSize);

                if (connection.pmIncoming.currentPacketSize > MaxPacketSize) {
                    CloseConnection(connectionIndex, "Packet size too large");
                    return false;
                }

                connection.pmIncoming.currentPacketExtractionOffset = 0;
                connection.pmIncoming.currentTask = PacketManagerTask::ProcessPacketContents;
            }
        }
        else
        {
            if (connection.pmIncoming.currentPacketExtractionOffset == connection.pmIncoming.currentPacketSize) {
                Packet* packet = new Packet();
                packet->buffer.resize(connection.pmIncoming.currentPacketSize);
                memcpy(&packet->buffer[0], connection.buffer, connection.pmIncoming.currentPacketSize);

                connection.pmIncoming.Append(packet);

                connection.pmIncoming.currentPacketSize = 0;
                connection.pmIncoming.currentPacketExtractionOffset = 0;
                connection.pmIncoming.currentTask = PacketManagerTask::ProcessPacketSize;
            }
        }
    }
    return true;
}

void PLServer::SendToConnection(int connectionIndex)
{
    auto& connection = p_connections[connectionIndex];
    int i = connectionIndex + 1;

    PacketManager& pm = connection.pmOutgoing;
    while (pm.HasPendingPackets()) {
        if (pm.currentTask == PacketManagerTask::ProcessPacketSize) {
            pm.currentPacketSize = pm.Retrieve()->buffer.size();
            uint16_t bigEndianPacketSize = htons(pm.currentPacketSize);
            int bytesSent = send(p_useFd[i].fd,
                (char*)(&bigEndianPacketSize) + pm.currentPacketExtractionOffset,
                sizeof(uint16_t) - pm.currentPacketExtractionOffset, 0);

            if (bytesSent > 0) {
                pm.currentPacketExtractionOffset += bytesSent;
            }
            if (pm.currentPacketExtractionOffset == sizeof(uint16_t)) {
                pm.currentPacketExtractionOffset = 0;
                pm.currentTask = PacketManagerTask::ProcessPacketContents;
            }
            else {
                break;
            }
        }
        else {
            char* bufferPtr = &pm.Retrieve()->buffer[0];
            int bytesSent = send(p_useFd[i].fd,
                (char*)(bufferPtr)+pm.currentPacketExtractionOffset,
                pm.currentPacketSize - pm.currentPacketExtractionOffset, 0);

            if (bytesSent > 0) {
                pm.currentPacketExtractionOffset += bytesSent;
            }
            if (pm.currentPacketExtractionOffset == pm.currentPacketSize) {
                pm.currentPacketExtractionOffset = 0;
                pm.currentTask = PacketManagerTask::ProcessPacketSize;
                pm.Pop();
            }
            else {
                break;
            }
        }
    }
}

void PLServer::CloseConnection(int connectionIndex, std::string reason)
{
    auto& connection = p_connections[connectionIndex];
    OnDisconnect(connection, reason);
    p_masterFd.erase(p_masterFd.begin() + (connectionIndex + 1));
    p_useFd.erase(p_useFd.begin() + (connectionIndex + 1));
    connection.Close();
    p_connections.erase(p_connections.begin() + connectionIndex);
}

void PLServer::Clear()
{
    p_masterFd.clear();
    p_listeningSocket.Close();
    for (int i = 0; i < p_connections.size(); i++)
        CloseConnection(i,"PLServer is shutting down");
    p_connections.clear();
    Networking::Shutdown();
    p_running = false;
}
