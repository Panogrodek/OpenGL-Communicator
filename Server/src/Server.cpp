#include "Server.hpp"

#include <SPDLOG/spdlog.h>


Server::Server()
{
	Initialize(pl::IPEndpoint("::", 6112));
}

Server::~Server()
{
	Clear();
}

void Server::Run()
{
	while (p_running) {
		Frame();
	}
}

void Server::OnConnect(pl::TCPConnection& newConnection)
{
	spdlog::info("{} - new connection accepted server side" , newConnection.ToString());
	m_connectedClients[newConnection.ToString()] = Client();
	m_connectedClients[newConnection.ToString()].nick = std::string("dummy" + std::to_string(m_connectedClients.size()));
	auto& newCon = m_connectedClients[newConnection.ToString()];

	for (auto& con : p_connections) {
		if (&con == &newConnection)
			continue;
		pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::Connected);
		*stringPacket << std::string(newCon.nick + ": has successfully connected!");
		con.pmOutgoing.Append(stringPacket);
	}

	for (auto& con : p_connections) {
		pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::Connected);
		*stringPacket << std::string(m_connectedClients[con.ToString()].nick + ": has successfully connected!");
		newConnection.pmOutgoing.Append(stringPacket);
	}

	pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::WhoYouAre);
	*stringPacket << std::string(m_connectedClients[newConnection.ToString()].nick);
	newConnection.pmOutgoing.Append(stringPacket);
}

void Server::OnDisconnect(pl::TCPConnection& lostConnection, std::string reason)
{
	spdlog::warn("Connection lost: {} reason: {}",lostConnection.ToString(),reason);
	std::string nick = "unknown";
	if (m_connectedClients.find(lostConnection.ToString()) != m_connectedClients.end()) {
		nick = m_connectedClients[lostConnection.ToString()].nick;
		m_connectedClients.erase(lostConnection.ToString());
	}

	for (auto& con : p_connections) {
		pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::Disconnected);
		*stringPacket << std::string(nick + ": has lost connection!");
		if (&con != &lostConnection)
			con.pmOutgoing.Append(stringPacket);
	}
}

bool Server::ProcessPacket(pl::Packet& packet, pl::TCPConnection& connection)
{
	switch (packet.GetPacketType())
	{
		case pl::PacketType::ChatMessage:
		{
			std::string chatmessage;
			packet >> chatmessage;
			spdlog::info("Received Chat Message Type {}: {}",packet.GetChatmessageType(), chatmessage);
			
			ProcessChatMessage(packet,connection, chatmessage);
			break;
		}
	}
	return true;
}

void Server::ProcessChatMessage(pl::Packet& packet,pl::TCPConnection& connection, std::string data)
{
	std::string nick = m_connectedClients[connection.ToString()].nick;
	std::string receivedNick{};
	switch (packet.GetChatmessageType())
	{
	case pl::ChatType::Connected:
		break;
	case pl::ChatType::Disconnected:
		break;
	case pl::ChatType::HasSendMessage: 		
		//this might be counter intuitive to send this to all clients, but this confirms that packet has
		//been received by the server
		for (auto& con : p_connections) {
			pl::Packet* outgoing = new pl::Packet(pl::ChatMessage,pl::HasSendMessage); //because server sends message
			*outgoing << nick + ": " + data; 
			con.pmOutgoing.Append(outgoing);
		}
		break;
	case pl::ChatType::SetNick:
		m_connectedClients[connection.ToString()].nick = data;
		for (auto& con : p_connections) {
			pl::Packet* outgoing = new pl::Packet(pl::ChatMessage, pl::SetNick); 
			*outgoing << nick + ": " + data; 
			con.pmOutgoing.Append(outgoing);

			outgoing = new pl::Packet(pl::ChatMessage, pl::HasSendMessage); 
			*outgoing << nick + " is now known as " + data; 
			con.pmOutgoing.Append(outgoing);
		}
		break;
	case pl::ChatType::WhispersTo:
		receivedNick = ErasePart(':',data);
		receivedNick.pop_back();
		for (auto& [ip, client] : m_connectedClients) {
			if (client.nick != receivedNick)
				continue;

			for (auto& con : p_connections) {
				if (con.ToString() != ip)
					continue;

				pl::Packet* outgoing = new pl::Packet(pl::ChatMessage, pl::WhispersTo);
				*outgoing << nick + " whispers: " + data;
				con.pmOutgoing.Append(outgoing);
				break;
			}

			break;
		}
		break;
	default:
		break;
	}
}
