#pragma once
#include "Networking/Networking.hpp"
#include <unordered_map>
#include <Utilities/Utilities.hpp>

struct Client { //stores client information needed for application
	std::string nick{};
};

class Server : pl::PLServer {
public:
	Server();
	~Server();

	void Run();
private:
	void OnConnect(pl::TCPConnection& newConnection) override;
	void OnDisconnect(pl::TCPConnection& lostConnection, std::string reason) override;
	bool ProcessPacket(pl::Packet& packet, pl::TCPConnection& connection) override;

	void ProcessChatMessage(pl::Packet& packet,pl::TCPConnection& connection, std::string data);

	std::unordered_map<std::string, Client> m_connectedClients;
};