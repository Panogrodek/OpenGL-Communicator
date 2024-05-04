#pragma once
#include "Graphics/Graphics.hpp"

struct ClientData {
	std::string nick;
};

namespace priv {
	class ClientList {
	public:
		void Init(pl::AABB2D aabb);

		void Update();
		void Render();

		pl::RectangleShape& GetBody();
		pl::Text& GetText();

		void SetMe(std::string me);

		void ChangeClientsNick(std::string old, std::string _new);
		void AddClient(::ClientData nick);
		void RemoveClient(::ClientData nick);
		bool DoesExist(std::string nick);
	private:
		::ClientData* m_me;
		std::vector<::ClientData> m_clients;
		pl::RectangleShape* m_body;
		pl::Text* m_text;
	};
}

inline priv::ClientList clientList;