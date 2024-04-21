#pragma once
#include "SocketOptions.hpp"
#include <vector>
#include <WS2tcpip.h>

namespace pl {
	class IPEndpoint {
	public:
		IPEndpoint() = default;
		IPEndpoint(const char* ip, uint16_t port);
		IPEndpoint(sockaddr* addr);
		uint16_t GetPort();
		IpVersion GetIpVersion();
		std::string GetHostname();
		std::string GetIpString();
		std::vector<uint8_t> GetIpBytes();
		sockaddr_in GetSockaddrIPv4();
		sockaddr_in6 GetSockaddrIPv6();

		void Print();
	private:
		uint16_t m_port = 0; //port connection is bound to
		IpVersion m_ipversion = IpVersion::Unknown; //ipversion connection is using
		std::string m_hostname{}; //name of the host
		std::string m_ipString{}; //ip in string representation
		std::vector<uint8_t> m_ipBytes{}; //ip in bytes representation
	};
}