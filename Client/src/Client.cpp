#include "pch.h"
#include "Client.hpp"
#include "ui/Logbox.hpp"
#include "ui/ClientList.hpp"
#include <SPDLOG/spdlog.h>
#include <Utilities/Utilities.hpp>

using namespace priv;

Client::Client()
{
	Connect(pl::IPEndpoint("::1", 6112));
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

void Client::SendPacket(pl::Packet* packet)
{
	if (!IsConnected()) {
		spdlog::warn("Tried sending packet without connection established!");
		delete packet;
		return;
	}

	p_connection.pmOutgoing.Append(packet);
}

bool Client::ProcessPacket(pl::Packet& packet)
{
	switch (packet.GetPacketType())
	{
	case pl::PacketType::ChatMessage:
	{
		std::string chatmessage;
		packet >> chatmessage;
		spdlog::info("Received Chat Message {}",chatmessage);

		ProcessChatMessage(packet, chatmessage);
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

void Client::ProcessChatMessage(pl::Packet& packet,std::string data)
{
	ClientData client;
	std::string oldNick;
	std::string newNick;
	switch (packet.GetChatmessageType())
	{
	case pl::ChatType::Connected:
		logBox.GetText().SetDrawingColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		logBox.AddMessage(data + "\n");
		client.nick = ErasePart(':',data);
		client.nick.pop_back();
		clientList.AddClient(client);
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
		break;
	case pl::ChatType::Disconnected:
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		logBox.AddMessage(data + "\n");
		client.nick = ErasePart(':', data);
		client.nick.pop_back();
		clientList.RemoveClient(client);
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
		break;
	case pl::ChatType::HasSendMessage:
		logBox.AddMessage(data + "\n");
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
		break;
	case pl::ChatType::SetNick:
		oldNick = ErasePart(':', data);
		oldNick.pop_back();
		data.erase(0, 1);
		newNick = data;
		clientList.ChangeClientsNick(oldNick, newNick);
		logBox.GetText().SetDrawingColor(glm::vec4(22, 7, 173,255)/255.f);
		break;
	case pl::ChatType::WhispersTo:
		logBox.GetText().SetDrawingColor(glm::vec4(116, 114, 130,255)/255.f);
		logBox.AddMessage(data + "\n");
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
		break;
	case pl::ChatType::WhoYouAre:
		clientList.SetMe(data);
		logBox.GetText().SetDrawingColor(glm::vec4(87, 87, 89, 255) / 255.f);
		logBox.AddMessage("You have joined successfully! You are: " + data + "\n");
		logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
	default:
		break;
	}
}

void Client::OnConnect()
{
	std::cout << "Successfully connected to the server!" << std::endl;

	pl::Packet* helloPacket = new pl::Packet(pl::PacketType::ChatMessage);
	*helloPacket << std::string("Hello from the client!");
	p_connection.pmOutgoing.Append(helloPacket);
}
