#include "plpch.h"
#include "Networking/IPEndpoint.hpp"

using namespace pl;

IPEndpoint::IPEndpoint(const char* ip, uint16_t port)
{
    m_port = port;

    //IPv4
    in_addr addr{};
    int result = inet_pton(AF_INET, ip, &addr);

    if (result == 1 && addr.S_un.S_addr != INADDR_NONE) {
        m_ipString = ip;
        m_hostname = ip;

        m_ipBytes.resize(sizeof(ULONG));
        memcpy(&m_ipBytes[0], &addr.S_un.S_addr, sizeof(ULONG));

        m_ipversion = IpVersion::IPv4;

        return;
    }

    addrinfo hints = {};
    hints.ai_family = AF_INET; //ipv4 adresses only
    addrinfo* hostinfo = nullptr;
    result = getaddrinfo(ip, NULL, &hints, &hostinfo);
    if (result == 0) {
        sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);

        m_ipString.resize(16);
        inet_ntop(AF_INET, &host_addr->sin_addr, &m_ipString[0], 16);

        m_hostname = ip;

        ULONG ip_long = host_addr->sin_addr.S_un.S_addr;
        m_ipBytes.resize(sizeof(ULONG));
        memcpy(&m_ipBytes[0], &ip_long, sizeof(ULONG));

        m_ipversion = IpVersion::IPv4;

        freeaddrinfo(hostinfo);
        return;
    }

    //IPv6
    in6_addr addr6;
    result = inet_pton(AF_INET6, ip, &addr6);

    if (result == 1) {
        m_ipString = ip;
        m_hostname = ip;

        m_ipBytes.resize(16);
        memcpy(&m_ipBytes[0], &addr6.u, 16);

        m_ipversion = IpVersion::IPv6;
        return;
    }

    addrinfo hintsv6 = {};
    hintsv6.ai_family = AF_INET6;
    addrinfo* hostinfov6 = nullptr;
    result = getaddrinfo(ip, NULL, &hintsv6, &hostinfov6);
    if (result == 0) {
        sockaddr_in6* host_addr = reinterpret_cast<sockaddr_in6*>(hostinfov6->ai_addr);

        m_ipString.resize(46);
        inet_ntop(AF_INET6, &host_addr->sin6_addr, &m_ipString[0], 46);

        m_hostname = ip;

        m_ipBytes.resize(16);
        memcpy(&m_ipBytes[0], &host_addr->sin6_addr, 16);

        m_ipversion = IpVersion::IPv6;

        freeaddrinfo(hostinfov6);
        return;
    }
}

IPEndpoint::IPEndpoint(sockaddr* addr)
{
    if (addr->sa_family == AF_INET) {
        sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
        m_ipversion = IpVersion::IPv4;
        m_port = ntohs(addrv4->sin_port);
        m_ipBytes.resize(sizeof(ULONG));
        memcpy(&m_ipBytes[0], &addrv4->sin_addr, sizeof(ULONG));
        m_ipString.resize(16);
        inet_ntop(AF_INET, &addrv4->sin_addr, &m_ipString[0], 16);
        m_hostname = m_ipString;
    }
    else {
        sockaddr_in6* addrv6 = reinterpret_cast<sockaddr_in6*>(addr);
        m_ipversion = IpVersion::IPv6;
        m_port = ntohs(addrv6->sin6_port);
        m_ipBytes.resize(16);
        memcpy(&m_ipBytes[0], &addrv6->sin6_addr, 16);
        m_ipString.resize(46);
        inet_ntop(AF_INET6, &addrv6->sin6_addr, &m_ipString[0], 46);
        m_hostname = m_ipString;
    }
}

uint16_t IPEndpoint::GetPort()
{
    return m_port;
}

IpVersion IPEndpoint::GetIpVersion()
{
    return m_ipversion;
}

std::string IPEndpoint::GetHostname()
{
    return m_hostname;
}

std::string IPEndpoint::GetIpString()
{
    return m_ipString;
}

std::vector<uint8_t> IPEndpoint::GetIpBytes()
{
    return m_ipBytes;
}

sockaddr_in IPEndpoint::GetSockaddrIPv4()
{
    if (m_ipversion != IpVersion::IPv4)
        spdlog::error("Wrong ip version being used!");
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr, &m_ipBytes[0], sizeof(ULONG));
    addr.sin_port = htons(m_port);
    return addr;
}

sockaddr_in6 IPEndpoint::GetSockaddrIPv6()
{
    if (m_ipversion != IpVersion::IPv6)
        spdlog::error("Wrong ip version being used!");
    sockaddr_in6 addr = {};
    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, &m_ipBytes[0], 16);
    addr.sin6_port = htons(m_port);
    return addr;
}

void IPEndpoint::Print()
{
    switch (m_ipversion)
    {
    case IpVersion::IPv4:
        spdlog::info("IP Version: IPv4");
        break;
    case IpVersion::IPv6:
        spdlog::info("IP Version: IPv6");
        break;
    default:
        spdlog::error("IP Version: Unknown");
    }

    spdlog::info("Hostname: {}",m_hostname);
    spdlog::info("IP: {}"  , m_ipString);
    spdlog::info("Port: {}", m_port);

    std::string bytes{};
    for (auto& digit : m_ipBytes)
    {
        bytes += std::to_string((int)digit);
        bytes += " ";
    }

    spdlog::info("IP bytes: {}", bytes);
}
