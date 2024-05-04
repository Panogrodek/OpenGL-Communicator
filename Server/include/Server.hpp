#pragma once
#include "Networking/Networking.hpp"

class Server : pl::PLServer {
public:
	Server();
	~Server();

	void Run();
private:
	void OnConnect(pl::TCPConnection& newConnection) override;
	void OnDisconnect(pl::TCPConnection& lostConnection, std::string reason) override;
	bool ProcessPacket(pl::Packet& packet) override;

	void ProcessChatMessage(pl::Packet& packet, std::string data);
};