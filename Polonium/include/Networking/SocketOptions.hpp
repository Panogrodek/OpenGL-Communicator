#pragma once
#include <WinSock2.h>

namespace pl {
	//own socket name define
	typedef SOCKET SocketHandle;

	enum class SocketResult {
		Success,
		NotYetImplemented,
		SocketCorrupted,
		SocketAlreadyCreated,
		CouldNotCreateTCPSocket,
		CouldNotSetBlocking,
		CouldNotSetSocketOption,
		CouldNotCloseSocket,
		CouldNotBindSocket,
		CouldNotSetSocketListen,
		CouldNotConnect,

		CouldNotSend,
		CouldNotReceive,
		MaxPacketSizeExceeded
	};

	enum class IpVersion {
		Unknown,
		IPv4,
		IPv6,
	};

	enum class SocketOption {
		TCP_NoDelay,
		IPv6_Only,
	};

	inline const int MaxPacketSize = 65535;
}