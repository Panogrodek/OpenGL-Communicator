#include "plpch.h"
#include <Networking/Networking.hpp>

using namespace pl;

bool PLClient::Connect(IPEndpoint ip)
{
    m_isConnected = false;
    if (!pl::Networking::InitializeWinsock())
        return false;

    Socket socket = Socket(ip.GetIpVersion());
    if (socket.Create() == SocketResult::Success) {
        if (socket.SetBlocking(true) != SocketResult::Success)
            return false;

        spdlog::info("Socket successfully created");
        if (socket.Connect(ip) == SocketResult::Success) {

            if (socket.SetBlocking(false) == SocketResult::Success) {
                p_connection = TCPConnection(socket, ip);
                m_masterFd.fd = p_connection.socket.GetHandle();
                m_masterFd.events = POLLRDNORM | POLLWRNORM;
                m_masterFd.revents = 0;
                m_isConnected = true;
                OnConnect();
                return true;
            }
        }
        socket.Close();
    }
    OnConnectFail();
    return false;
}

bool PLClient::IsConnected()
{
    return m_isConnected;
}

bool PLClient::Frame()
{
    m_useFd = m_masterFd;

    if (WSAPoll(&m_useFd, 1, 1) > 0) {
        if (!HandleErrors())
            return false;

        if (m_useFd.revents & POLLRDNORM)
            if (!ReadFromServer())
                return false;

        if (m_useFd.revents & POLLWRNORM)
            SendToServer();

        while (p_connection.pmIncoming.HasPendingPackets()) //handle packets
        {
            Packet* frontPacket = p_connection.pmIncoming.Retrieve();
            if (!ProcessPacket(*frontPacket))
            {
                CloseConnection("Failed to process incoming packet.");
                return false;
            }
            p_connection.pmIncoming.Pop();
        }
    }


}

void PLClient::Destroy() {
    p_connection.Close();
    Networking::Shutdown();
    m_isConnected = false;
}

bool PLClient::HandleErrors() {
    if (m_useFd.revents & POLLERR) {
        CloseConnection("POLLERR");
        return false;
    }

    if (m_useFd.revents & POLLHUP) {
        CloseConnection("POLLHUP");
        return false;
    }

    if (m_useFd.revents & POLLNVAL) {
        CloseConnection("POLLNVAL");
        return false;
    }
    return true;
}

bool PLClient::ReadFromServer()
{
    int bytesReceived = 0;

    if (p_connection.pmIncoming.currentTask == PacketManagerTask::ProcessPacketSize) {
        bytesReceived = recv(
            m_useFd.fd,
            (char*)&p_connection.pmIncoming.currentPacketSize + p_connection.pmIncoming.currentPacketExtractionOffset,
            sizeof(uint16_t) - p_connection.pmIncoming.currentPacketExtractionOffset, 0);
    }
    else {
        bytesReceived = recv(
            m_useFd.fd,
            (char*)&p_connection.buffer + p_connection.pmIncoming.currentPacketExtractionOffset,
            p_connection.pmIncoming.currentPacketSize - p_connection.pmIncoming.currentPacketExtractionOffset, 0);
    }


    if (bytesReceived == 0) {
        CloseConnection("[Recv==0]Connection lost: ");
        return false;
    }
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            CloseConnection("[Recv<0]Connection lost: ");
            return false;
        }
    }

    if (bytesReceived > 0) {
        p_connection.pmIncoming.currentPacketExtractionOffset += bytesReceived;
        if (p_connection.pmIncoming.currentTask == PacketManagerTask::ProcessPacketSize)
        {
            if (p_connection.pmIncoming.currentPacketExtractionOffset == sizeof(uint16_t)) {
                p_connection.pmIncoming.currentPacketSize = ntohs(p_connection.pmIncoming.currentPacketSize);

                if (p_connection.pmIncoming.currentPacketSize > MaxPacketSize) {
                    CloseConnection("Packet size too large");
                    return false;
                }

                p_connection.pmIncoming.currentPacketExtractionOffset = 0;
                p_connection.pmIncoming.currentTask = PacketManagerTask::ProcessPacketContents;
            }
        }
        else
        {
            if (p_connection.pmIncoming.currentPacketExtractionOffset == p_connection.pmIncoming.currentPacketSize) {
                Packet* packet = new Packet();
                packet->buffer.resize(p_connection.pmIncoming.currentPacketSize);
                memcpy(&packet->buffer[0], p_connection.buffer, p_connection.pmIncoming.currentPacketSize);

                p_connection.pmIncoming.Append(packet);

                p_connection.pmIncoming.currentPacketSize = 0;
                p_connection.pmIncoming.currentPacketExtractionOffset = 0;
                p_connection.pmIncoming.currentTask = PacketManagerTask::ProcessPacketSize;
            }
        }
    }
    return true;
}

void PLClient::SendToServer()
{
    PacketManager& pm = p_connection.pmOutgoing;
    while (pm.HasPendingPackets()) {
        if (pm.currentTask == PacketManagerTask::ProcessPacketSize) {
            pm.currentPacketSize = pm.Retrieve()->buffer.size();
            uint16_t bigEndianPacketSize = htons(pm.currentPacketSize);
            int bytesSent = send(m_useFd.fd,
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
            int bytesSent = send(m_useFd.fd,
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

bool PLClient::ProcessPacket(Packet& packet)
{
    spdlog::info("Packet received with size: {}", packet.buffer.size());
    return true;
}

void PLClient::OnConnect()
{
    spdlog::info("Succesfully connected");
}

void PLClient::OnConnectFail()
{
    spdlog::error("Failed to connect");
}

void PLClient::OnDisconnect(std::string reason)
{
    spdlog::error("Lost connection. Reason: {}", reason);
}

void PLClient::CloseConnection(std::string reason)
{
    OnDisconnect(reason);
    m_masterFd.fd = 0;
    m_isConnected = false;
    p_connection.Close();
}
