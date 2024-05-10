#pragma once
#include "Networking/Networking.hpp"
#include <unordered_map>
#include <Utilities/Utilities.hpp>

/**
 * @file Server.hpp
 * @brief Contains the declaration of the Server class as well as Client struct.
 */

struct Client { //stores client information needed for application
	std::string nick{};
};

/**
 * @class Server
 * @brief Represents a server that manages TCP connections and processes incoming packets.
 */
class Server : pl::PLServer {
public:
	/**
	* @brief Constructor for the Server class.
	*/
	Server();
	/**
	* @brief Destructor for the Server class.
	*/
	~Server();

	/**
	* @brief Runs the server, continuously processing incoming packets.
	*/
	void Run();
private:
	/**
	* @brief Callback function called when a new TCP connection is established.
	* @param newConnection Reference to the new TCP connection.
	*/
	void OnConnect(pl::TCPConnection& newConnection) override;
	/**
	* @brief Callback function called when a TCP connection is lost.
	* @param lostConnection Reference to the lost TCP connection.
	* @param reason Reason for the disconnection.
	*/
	void OnDisconnect(pl::TCPConnection& lostConnection, std::string reason) override;
	/**
	* @brief Processes an incoming packet.
	* @param packet Reference to the incoming packet.
	* @param connection Reference to the TCP connection from which the packet was received.
	* @return True if the packet is successfully processed, false otherwise.
	*/
	bool ProcessPacket(pl::Packet& packet, pl::TCPConnection& connection) override;

	/**
	* @brief Processes a chat message packet.
	* @param packet Reference to the incoming chat message packet.
	* @param connection Reference to the TCP connection from which the packet was received.
	* @param data The chat message data.
	*/
	void ProcessChatMessage(pl::Packet& packet,pl::TCPConnection& connection, std::string data);

	std::unordered_map<std::string, Client> m_connectedClients;
};