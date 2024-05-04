#pragma once
#include "Networking/Networking.hpp"

namespace priv {
	class Client : public pl::PLClient {
	public:
		Client();
		~Client();

		void Run();
		void SendPacket(pl::Packet* packet);
	private:
		bool ProcessPacket(pl::Packet& packet) override;
		void ProcessChatMessage(pl::Packet& packet, std::string data);
		void OnConnect() override;
	};
}

inline priv::Client client;