#pragma once
#include "Graphics/Graphics.hpp"

/**
 * @file ClientList.hpp
 * @brief Contains the declaration of the ClientList singleton class as well as ClientData struct.
 */

struct ClientData { //stores only nick for now, but could store things like color or icon
	std::string nick;
};

namespace priv {
	class ClientList {
	public:
		/**
		* @brief Initializes the ClientList object.
		*
		* Initializes the ClientList object with the given AABB (Axis-Aligned Bounding Box).
		*
		* @param aabb The AABB to initialize the ClientList with.
		*/
		void Init(pl::AABB2D aabb);

		/**
		* @brief Updates the ClientList object.
		*
		* This function updates the ClientList object.
		*/
		void Update();
		/**
		* @brief Renders the ClientList object.
		*
		* This function renders the ClientList object.
		*/
		void Render();

		/**
		* @brief Gets the body of the ClientList object.
		*
		* @return A reference to the RectangleShape representing the body of the ClientList.
		*/
		pl::RectangleShape& GetBody();
		/**
		* @brief Gets the text of the ClientList object.
		*
		* @return A reference to the Text object representing the text of the ClientList.
		*/
		pl::Text& GetText();

		/**
		* @brief Sets the current client.
		*
		* Sets the current client based on the provided nickname.
		*
		* @param me The nickname of the current client.
		*/
		void SetMe(std::string me);
		/**
		* @brief Changes the nickname of a client.
		*
		* Changes the nickname of a client from old to new.
		*
		* @param old The old nickname of the client.
		* @param _new The new nickname of the client.
		*/
		void ChangeClientsNick(std::string old, std::string _new);
		/**
		* @brief Adds a client to the list.
		*
		* Adds a client with the provided nickname to the list of clients.
		*
		* @param nick The nickname of the client to be added.
		*/
		void AddClient(::ClientData nick);
		/**
		* @brief Removes a client from the list.
		*
		* Removes a client with the provided nickname from the list of clients.
		*
		* @param nick The nickname of the client to be removed.
		*/
		void RemoveClient(::ClientData nick);
		/**
		* @brief Checks if a client exists in the list.
		*
		* Checks if a client with the provided nickname exists in the list of clients.
		*
		* @param nick The nickname of the client to be checked.
		* @return True if the client exists, otherwise false.
		*/
		bool DoesExist(std::string nick);
	private:
		::ClientData* m_me;
		std::vector<::ClientData> m_clients;
		pl::RectangleShape* m_body;
		pl::Text* m_text;
	};
}

inline priv::ClientList clientList;