#include "plpch.h"
#include "Networking/Networking.hpp"
#include <winsock.h>

using namespace pl;

bool Networking::InitializeWinsock(int versionMaj, int versionMin)
{
    // Initialize Winsock
    WORD wVersionRequested = MAKEWORD(versionMaj, versionMin);
    WSADATA wsaData;

    //Could not find the dll of winsock
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        spdlog::critical("Can't initialize Winsock! Error: {}",err);
        return false;
    }

    if (LOBYTE(wsaData.wVersion) != versionMaj || HIBYTE(wsaData.wVersion) != versionMin) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        spdlog::critical("Could not find a usable version of Winsock.dll");
        WSACleanup();
        return false;
    }

    return true;
}

void Networking::Shutdown()
{
    WSACleanup();
}
