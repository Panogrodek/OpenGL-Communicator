#pragma once

#pragma comment(lib, "ws2_32.lib") //TODO: i dont want it here, this is a bad practice, oh well
#include "PLServer.hpp"
#include "PLClient.hpp"

namespace pl {
	class Networking {
	public:
		static bool InitializeWinsock(int versionMaj = 2, int versionMin = 2);
		static void Shutdown();
	};
}