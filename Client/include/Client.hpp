#pragma once
#include "Networking/Networking.hpp"

namespace priv {
	class Client : public pl::PLClient {
	public:
		Client();
		~Client();

		void Run();
	private:
		bool ProcessPacket(pl::Packet& packet) override;
		void OnConnect() override;
	};
}

inline priv::Client client;