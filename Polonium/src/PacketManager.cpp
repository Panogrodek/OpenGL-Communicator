#include "plpch.h"
#include "Networking/PacketManager.hpp"

using namespace pl;

void PacketManager::Clear()
{
	while (!m_packets.empty())
		Pop();
	m_packets = std::queue<Packet*>{};
}

bool PacketManager::HasPendingPackets()
{
	return (!m_packets.empty());
}

void PacketManager::Append(Packet* p)
{
	m_packets.push(std::move(p));
}

Packet* PacketManager::Retrieve()
{
	return m_packets.front();
}

void PacketManager::Pop()
{
	delete m_packets.front();
	m_packets.pop();
}
