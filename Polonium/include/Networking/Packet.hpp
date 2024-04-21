#pragma once
#include <vector>
#include <stdint.h>

namespace pl {

	enum PacketType : uint16_t {
		Invalid,
		ChatMessage,
		IntegerArray,
	};

	class Packet {
	public:
		Packet(PacketType type = PacketType::Invalid);
		PacketType GetPacketType();
		void SetPacketType(PacketType type);

		void Clear();
		void Append(const void* data, uint32_t size);

		Packet& operator << (uint32_t data);
		Packet& operator >> (uint32_t& data);

		Packet& operator << (const std::string& data);
		Packet& operator >> (std::string& data);

		uint32_t extractionOffset = 0;
		std::vector<char> buffer;
	};
}