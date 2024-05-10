#pragma once
#include "Networking/Networking.hpp"

/**
 * @file Client.hpp
 * @brief Contains the declaration of the Client class.
 */

namespace priv {
	class Client : public pl::PLClient {
	public:
		/**
		* @brief Constructor of the Client class.
		*
		* Initializes a Client object by connecting to the server.
		*/
		Client();

		/**
		* @brief Runs the client.
		*
		* Runs the client if it is connected to the server.
		*/
		void Run();
		/**
		* @brief Sends a packet.
		*
		* Sends a packet if the client is connected to the server.
		*
		* @param packet Pointer to the packet to be sent.
		*/
		void SendPacket(pl::Packet* packet);
	private:
		/**
		* @brief Processes a received packet.
		*
		* Processes a received packet based on its type.
		*
		* @param packet The received packet.
		* @return True if the packet was processed successfully, otherwise false.
		*/
		bool ProcessPacket(pl::Packet& packet) override;
		/**
		* @brief Processes a chat message packet.
		*
		* Processes a chat message packet based on its type.
		*
		* @param packet The chat message packet.
		* @param data The chat message data.
		*/
		void ProcessChatMessage(pl::Packet& packet, std::string data);
		/**
		* @brief Callback function invoked when the client is connected to the server.
		*/
		void OnConnect() override;
	};
}

inline priv::Client client;