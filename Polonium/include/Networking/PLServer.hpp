#include <string>
#include "IPEndpoint.hpp"
#include "TCPConnection.hpp"


namespace pl {
	constexpr int MAX_CLIENTS = 16;
	constexpr int TIMEOUT = 1; //[ms]

	class PLServer {
	public:
		bool Initialize(IPEndpoint ip);
		void Frame();
	protected:
		virtual void OnConnect(TCPConnection & newConnection);
		virtual void OnDisconnect(TCPConnection & lostConnection, std::string reason);
		virtual bool ProcessPacket(Packet& packet);
		
		void UpdateListen();
		void UpdateConnections();

		bool ReadFromConnection(int connectionIndex);
		void SendToConnection(int connectionIndex);

		void CloseConnection(int connectionIndex, std::string reason);
		void Clear();

		Socket p_listeningSocket; //socket that listens for more connections
		std::vector<TCPConnection> p_connections; //all connections made to the server
		std::vector<WSAPOLLFD> p_masterFd; //master data to check
		std::vector< WSAPOLLFD> p_useFd; //temporary data to check 
		bool p_running = false;
	};
}
