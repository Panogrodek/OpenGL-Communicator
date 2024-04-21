#pragma once
#include "Socket.hpp"
#include "PacketManager.hpp"

namespace pl {
	class TCPConnection {
	public:
		TCPConnection() {};
		TCPConnection(Socket socket, IPEndpoint endpoint);

		void Close();
		std::string ToString();
		Socket socket;

		PacketManager pmIncoming;
		PacketManager pmOutgoing;
		char buffer[pl::MaxPacketSize];
	private:
		IPEndpoint m_endpoint;
		std::string m_stringRepresentation = "";
	};
}