#pragma once
#include "Graphics/Graphics.hpp"

namespace priv {

	class LogBox {
	public:
		void Init(pl::AABB2D aabb);
		~LogBox();

		void Update();
		void Render();

		pl::RectangleShape& GetBody();
		pl::Text& GetText();

		void AddMessage(std::string message);
	private:
		std::vector<std::string> s_incomingMessages;
		pl::RectangleShape* m_body;
		pl::Text* m_text;

		float m_ceiling;
		float m_bottom;
	};
}

inline priv::LogBox logBox;