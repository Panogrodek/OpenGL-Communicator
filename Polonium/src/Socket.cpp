#include "plpch.h"
#include "Networking/Socket.hpp"

#include <WinSock2.h>

using namespace pl;

Socket::Socket(IpVersion ipversion, SocketHandle handle) :
	m_ipversion(ipversion), m_handle(handle)
{
}

SocketResult Socket::Create()
{
	if (m_handle != INVALID_SOCKET) {
		return SocketResult::SocketAlreadyCreated;
	}

	m_handle = socket((m_ipversion == IpVersion::IPv4) ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP);

	if (m_handle == INVALID_SOCKET) {
		int error = WSAGetLastError();
		return SocketResult::CouldNotCreateTCPSocket;
	}

	if (SetBlocking(false) != SocketResult::Success) {
		return SocketResult::CouldNotSetBlocking;
	}

	if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != SocketResult::Success)
		return SocketResult::CouldNotSetSocketOption;

	return SocketResult::Success;
}

SocketResult Socket::Close()
{
	if (m_handle == INVALID_SOCKET) {
		return SocketResult::SocketCorrupted;
	}

	int result = closesocket(m_handle);
	if (result != 0) {
		int error = WSAGetLastError();
		return SocketResult::CouldNotCloseSocket;
	}

	m_handle = INVALID_SOCKET;
	return SocketResult::Success;
}

SocketResult Socket::Bind(IPEndpoint endpoint)
{
	if (m_ipversion == IpVersion::IPv4) {
		sockaddr_in addr = endpoint.GetSockaddrIPv4();

		int result = bind(m_handle, (sockaddr*)&addr, sizeof(sockaddr_in));
		if (result == 0)
			return SocketResult::Success;

		int error = WSAGetLastError();
		return SocketResult::CouldNotBindSocket;
	}
	else if (m_ipversion == IpVersion::IPv6) {
		sockaddr_in6 addr = endpoint.GetSockaddrIPv6();

		int result = bind(m_handle, (sockaddr*)&addr, sizeof(sockaddr_in6));
		if (result == 0)
			return SocketResult::Success;

		int error = WSAGetLastError();
		return SocketResult::CouldNotBindSocket;
	}

	return SocketResult::CouldNotBindSocket;
}

SocketResult Socket::Listen(IPEndpoint endpoint, int backlog) {
	if (m_ipversion == IpVersion::IPv6) {
		if (SetSocketOption(SocketOption::IPv6_Only, FALSE) != SocketResult::Success)
			return SocketResult::CouldNotSetSocketOption;
	}


	if (Bind(endpoint) != SocketResult::Success)
		return SocketResult::CouldNotBindSocket;

	int result = listen(m_handle, backlog);
	if (result != 0) {
		int error = WSAGetLastError();
		return SocketResult::CouldNotSetSocketListen;
	}

	return SocketResult::Success;
}

SocketResult Socket::Accept(Socket& output, IPEndpoint* endpoint)
{
	if (m_ipversion == IpVersion::IPv4) {
		sockaddr_in addr = {};
		int len = sizeof(sockaddr_in);
		SocketHandle acceptedConnectionHandle = accept(m_handle, (sockaddr*)(&addr), &len);
		if (acceptedConnectionHandle == INVALID_SOCKET) {
			int error = WSAGetLastError();
			return SocketResult::SocketCorrupted;
		}
		if (endpoint != nullptr) {
			*endpoint = IPEndpoint((sockaddr*)&addr);
		}

		output = Socket(IpVersion::IPv4, acceptedConnectionHandle);
	}
	else if (m_ipversion == IpVersion::IPv6) {
		sockaddr_in6 addr = {};
		int len = sizeof(sockaddr_in6);
		SocketHandle acceptedConnectionHandle = accept(m_handle, (sockaddr*)(&addr), &len);
		if (acceptedConnectionHandle == INVALID_SOCKET) {
			int error = WSAGetLastError();
			return SocketResult::SocketCorrupted;
		}
		if (endpoint != nullptr) {
			*endpoint = IPEndpoint((sockaddr*)&addr);
		}

		output = Socket(IpVersion::IPv6, acceptedConnectionHandle);
	}

	return SocketResult::Success;
}

SocketResult Socket::Connect(IPEndpoint endpoint)
{
	int result = -1;
	if (m_ipversion == IpVersion::IPv4) {
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		result = connect(m_handle, (sockaddr*)&addr, sizeof(sockaddr_in));
	}
	else if (m_ipversion == IpVersion::IPv6) {
		sockaddr_in6 addr = endpoint.GetSockaddrIPv6();
		result = connect(m_handle, (sockaddr*)&addr, sizeof(sockaddr_in6));
	}
	if (result != 0) {
		int error = WSAGetLastError();
		std::cout << "Error code: " << error << "\n";
		return SocketResult::CouldNotConnect;
	}

	return SocketResult::Success;
}

SocketResult Socket::Send(const void* data, int numberOfBytes, int& bytesSent)
{
	bytesSent = send(m_handle, (const char*)data, numberOfBytes, NULL);

	if (bytesSent == SOCKET_ERROR) {
		int error = WSAGetLastError();
		return SocketResult::CouldNotSend;
	}

	return SocketResult::Success;
}

SocketResult Socket::Recv(void* destination, int numberOfBytes, int& bytesRecv)
{
	bytesRecv = recv(m_handle, (char*)destination, numberOfBytes, NULL);

	if (bytesRecv == 0) {
		return SocketResult::CouldNotReceive;
	}
	if (bytesRecv == SOCKET_ERROR) {
		int error = WSAGetLastError();
		return SocketResult::CouldNotReceive;
	}
	return SocketResult::Success;
}

SocketResult Socket::SendAll(const void* data, int numberOfBytes)
{
	int totalBytesSent = 0;
	while (totalBytesSent < numberOfBytes) {
		int bytesRemaining = numberOfBytes - totalBytesSent;
		int bytesSent = 0;
		char* bufferOffset = (char*)data + totalBytesSent;
		SocketResult result = Send(bufferOffset, bytesRemaining, bytesSent);
		if (result != SocketResult::Success)
			return SocketResult::CouldNotSend;

		totalBytesSent += bytesSent;
	}

	return SocketResult::Success;
}

SocketResult Socket::RecvAll(void* data, int numberOfBytes)
{
	int totalBytesSent = 0;
	while (totalBytesSent < numberOfBytes) {
		int bytesRemaining = numberOfBytes - totalBytesSent;
		int bytesSent = 0;
		char* bufferOffset = (char*)data + totalBytesSent;
		SocketResult result = Recv(bufferOffset, bytesRemaining, bytesSent);
		if (result != SocketResult::Success)
			return SocketResult::CouldNotReceive;

		totalBytesSent += bytesSent;
	}

	return SocketResult::Success;
}

SocketResult Socket::Send(Packet& packet)
{
	uint16_t encodedPacketSize = htons(packet.buffer.size());
	SocketResult result = SendAll(&encodedPacketSize, sizeof(uint16_t));
	if (result != SocketResult::Success)
		return SocketResult::CouldNotSend;

	result = SendAll(packet.buffer.data(), packet.buffer.size());
	if (result != SocketResult::Success)
		return SocketResult::CouldNotSend;

	return SocketResult::Success;
}

SocketResult Socket::Recv(Packet& packet)
{
	packet.Clear();

	uint16_t encodedSize = 0;
	SocketResult result = RecvAll(&encodedSize, sizeof(uint16_t));
	if (result != SocketResult::Success)
		return SocketResult::CouldNotReceive;

	uint16_t bufferSize = ntohs(encodedSize);

	if (bufferSize > MaxPacketSize)
		return SocketResult::MaxPacketSizeExceeded;

	packet.buffer.resize(bufferSize);
	result = RecvAll(&packet.buffer[0], bufferSize);
	if (result != SocketResult::Success)
		return SocketResult::CouldNotReceive;

	return SocketResult::Success;
}

SocketResult Socket::SetBlocking(bool isBlocking)
{
	unsigned long nonBlocking = 1;
	unsigned long blocking = 0;
	int result = ioctlsocket(m_handle, FIONBIO, isBlocking ? &blocking : &nonBlocking);
	if (result == SOCKET_ERROR) {
		int error = WSAGetLastError();
		return SocketResult::CouldNotSetBlocking;
	}

	return SocketResult::Success;
}

SocketHandle Socket::GetHandle()
{
	return m_handle;
}

IpVersion Socket::GetIpVersion()
{
	return m_ipversion;
}

SocketResult Socket::SetSocketOption(SocketOption option, BOOL value)
{
	int result{};
	switch (option)
	{
	case SocketOption::TCP_NoDelay:
		result = setsockopt(m_handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
		break;
	case SocketOption::IPv6_Only:
		result = setsockopt(m_handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&value, sizeof(value));
		break;
	default:
		return SocketResult::CouldNotSetSocketOption;
	}

	if (result != 0) {
		int error = WSAGetLastError();
		return SocketResult::CouldNotSetSocketOption;
	}

	return SocketResult::Success;
}
