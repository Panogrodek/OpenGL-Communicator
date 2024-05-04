#pragma once
#include <vector>
#include <stdint.h>

namespace pl {

	enum PacketType : uint16_t { //this is more of a type decleration
		Invalid,
		ChatMessage,
		IntegerArray,
	};

	//this should be called User Defined Packet types tbh
	enum ChatType : uint16_t { //this is user specific for application purpouses (e.g. it declares what to do with data)
		Error,
		Connected,
		SetNick,
		Disconnected,
		HasSendMessage,
		WhispersTo,
	};

	class Packet {
	public:
		Packet(PacketType type = PacketType::Invalid, ChatType chat = ChatType::Error);
		
		PacketType GetPacketType();
		void SetPacketType(PacketType type);

		ChatType GetChatmessageType();
		void SetChatmessageType(ChatType type);

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