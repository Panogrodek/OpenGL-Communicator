#pragma once
#include "Graphics/Graphics.hpp"

class LogBox {
public:
	LogBox(pl::AABB2D aabb = pl::AABB2D{});
	~LogBox();
	void Update();
	void Render();

	pl::RectangleShape& GetBody();

	static void AddMessage(std::string message);
private:
	static std::vector<std::string> s_messages;
	pl::RectangleShape* m_body;
	pl::Text* m_text;
};