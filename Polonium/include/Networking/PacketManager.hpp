#pragma once
#include "Packet.hpp"

#include <queue>

namespace pl {
	enum class PacketManagerTask {
		ProcessPacketSize,
		ProcessPacketContents
	};

	class PacketManager {
	public:
		void Clear();
		bool HasPendingPackets();
		void Append(Packet* packets);
		Packet* Retrieve();
		void Pop();

		uint16_t currentPacketSize = 0;
		int currentPacketExtractionOffset = 0;
		PacketManagerTask currentTask = PacketManagerTask::ProcessPacketSize;
	private:
		std::queue<Packet*> m_packets;
	};
}