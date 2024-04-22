#include "pch.h"
#include "Client.hpp"

using namespace priv;

Client::Client()
{
	//Connect(pl::IPEndpoint("::1", 6112));
}

Client::~Client()
{

}

void Client::Run()
{
	if (!IsConnected())
		return;
	Frame();
}

bool Client::ProcessPacket(pl::Packet& packet)
{
	switch (packet.GetPacketType())
	{
	case pl::PacketType::ChatMessage:
	{
		std::string chatmessage;
		packet >> chatmessage;
		std::cout << "Chat Message: " << chatmessage << std::endl;
		break;
	}
	case pl::PacketType::IntegerArray:
	{
		uint32_t arraySize = 0;
		packet >> arraySize;
		std::cout << "Array Size: " << arraySize << std::endl;
		for (uint32_t i = 0; i < arraySize; i++)
		{
			uint32_t element = 0;
			packet >> element;
			std::cout << "Element[" << i << "] - " << element << std::endl;
		}
		break;
	}
	default:
		std::cout << "Unrecognized packet type: " << packet.GetPacketType() << std::endl;
		return false;
	}

	return true;
}

void Client::OnConnect()
{
	std::cout << "Successfully connected to the server!" << std::endl;

	pl::Packet* helloPacket = new pl::Packet(pl::PacketType::ChatMessage);
	*helloPacket << std::string("Hello from the client!");
	p_connection.pmOutgoing.Append(helloPacket);
}
