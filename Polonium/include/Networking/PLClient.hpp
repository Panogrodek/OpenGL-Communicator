#pragma once
#include "TCPConnection.hpp"
#include <string>

namespace pl {
	class PLClient {
	public:
		bool Connect(IPEndpoint ip);
		bool IsConnected();
		bool Frame();

		virtual void Destroy();
	protected:
		virtual bool ProcessPacket(Packet& packet);
		virtual void OnConnect();
		virtual void OnConnectFail();
		virtual void OnDisconnect(std::string reason);

		void CloseConnection(std::string reason);
		
		bool HandleErrors();
		bool ReadFromServer();
		void SendToServer();

		TCPConnection p_connection;
		bool m_isConnected = false;
	private:
		WSAPOLLFD m_masterFd;
		WSAPOLLFD m_useFd;
	};
}