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
	for (auto& con : p_connections) {
		pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::Connected);
		*stringPacket << std::string(newConnection.ToString() + "has successfully connected!");
		con.pmOutgoing.Append(stringPacket);
	}
}

void Server::OnDisconnect(pl::TCPConnection& lostConnection, std::string reason)
{
	spdlog::warn("Connection lost: {} reason: {}",lostConnection.ToString(),reason);
	for (auto& con : p_connections) {
		pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage, pl::ChatType::Disconnected);
		*stringPacket << std::string(lostConnection.ToString() + "has lost connection!");
		if (&con != &lostConnection)
			con.pmOutgoing.Append(stringPacket);
	}
}

bool Server::ProcessPacket(pl::Packet& packet)
{
	switch (packet.GetPacketType())
	{
		case pl::PacketType::ChatMessage:
		{
			std::string chatmessage;
			packet >> chatmessage;
			spdlog::info("Received Chat Message Type {}: {}",packet.GetChatmessageType(), chatmessage);
			
			ProcessChatMessage(packet, chatmessage);
			break;
		}
	}
	return true;
}

void Server::ProcessChatMessage(pl::Packet& packet, std::string data)
{
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
			*outgoing << con.ToString() + data; //TODO: VERY IMPORTANT! change ip to nick for security
			con.pmOutgoing.Append(outgoing);
		}
		break;
	case pl::ChatType::SetNick:
		break;
	case pl::ChatType::WhispersTo:
		break;
	default:
		break;
	}
}
