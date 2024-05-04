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

	pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage);
	*stringPacket << std::string("Hello from server\n");
	p_connections.back().pmOutgoing.Append(stringPacket);
}

void Server::OnDisconnect(pl::TCPConnection& lostConnection, std::string reason)
{
	spdlog::warn("Connection lost: {} reason: {}",lostConnection.ToString(),reason);

	pl::Packet* stringPacket = new pl::Packet(pl::PacketType::ChatMessage);
	*stringPacket << std::string(lostConnection.ToString() + " has lost connection!\n");

	for (auto& con : p_connections) {
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
			spdlog::info("Received Chat Message {}", chatmessage);
			

			//TODO: fix
			for (auto& connection : p_connections) {
				pl::Packet* newPacket = new pl::Packet(pl::PacketType::ChatMessage);
				*newPacket << chatmessage;
				connection.pmOutgoing.Append(newPacket);
			}
			break;
		}
	}
	return true;
}
