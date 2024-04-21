#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "IPEndpoint.hpp"
#include "Packet.hpp"

namespace pl {
	class Socket {
	public:
		Socket(IpVersion ipversion = IpVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
		SocketResult Create();
		SocketResult Close();

		SocketResult Bind(IPEndpoint endpoint);
		SocketResult Listen(IPEndpoint endpoint, int backlog);

		SocketResult Accept(Socket& output, IPEndpoint* endpoint = nullptr);
		SocketResult Connect(IPEndpoint endpoint);

		//General data sending functions
		SocketResult Send(const void* data, int numberOfBytes, int& bytesSent);
		SocketResult Recv(void* destination, int numberOfBytes, int& bytesRecv);
		SocketResult SendAll(const void* data, int numberOfBytes);
		SocketResult RecvAll(void* data, int numberOfBytes);

		//Packet handling
		SocketResult Send(Packet& packet);
		SocketResult Recv(Packet& packet);

		SocketResult SetBlocking(bool isBlocking);

		SocketHandle GetHandle();
		IpVersion GetIpVersion();
	private:
		SocketResult SetSocketOption(SocketOption option, BOOL value);
		IpVersion m_ipversion = IpVersion::IPv4;
		SocketHandle m_handle = INVALID_SOCKET;
	};
}