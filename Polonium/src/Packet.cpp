#include "plpch.h"
#include "Networking/Packet.hpp"
#include "Networking/SocketOptions.hpp"

#include <WinSock2.h>

using namespace pl;

Packet::Packet(PacketType type)
{
	Clear();
	SetPacketType(type);
}

PacketType Packet::GetPacketType()
{
	PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
	return static_cast<PacketType>(ntohs(*packetTypePtr)); //network to host short
}

void Packet::SetPacketType(PacketType type)
{
	PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
	*packetTypePtr = static_cast<PacketType>(htons(type));
}

void Packet::Clear()
{
	buffer.resize(sizeof(PacketType));
	SetPacketType(PacketType::Invalid);
	extractionOffset = sizeof(PacketType);
}

void Packet::Append(const void* data, uint32_t size)
{
	if ((buffer.size() + size) > MaxPacketSize) {
		spdlog::error("packet size is greater than maxpacketsize = [{}]", pl::MaxPacketSize);
		return;
	}

	buffer.insert(buffer.end(), (char*)data, (char*)data + size);
}

Packet& Packet::operator<<(uint32_t data)
{
	data = htonl(data);
	Append(&data, sizeof(uint32_t));
	return *this;
}

Packet& Packet::operator>>(uint32_t& data)
{
	if ((extractionOffset + sizeof(uint32_t)) > buffer.size()) {
		spdlog::error("extracting not existing data");
		return *this; 
	}
	data = *reinterpret_cast<uint32_t*>(&buffer[extractionOffset]);
	data = ntohl(data);
	extractionOffset += sizeof(uint32_t);
	return *this;
}

Packet& Packet::operator<<(const std::string& data)
{
	*this << (uint32_t)data.size();
	Append(data.data(), data.size());
	return *this;
}

Packet& Packet::operator>>(std::string& data)
{
	data.clear();

	uint32_t stringSize = 0;
	*this >> stringSize;

	if ((extractionOffset + stringSize) > buffer.size()) {
		spdlog::error("extracting not existing data");
		return *this;
	}

	data.resize(stringSize);
	data.assign(&buffer[extractionOffset], stringSize);
	extractionOffset += stringSize;
	return *this;
}
