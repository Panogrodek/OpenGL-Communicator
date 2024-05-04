#pragma once
#include "Graphics/Graphics.hpp"

class Textbox {
public:
	Textbox() {};
	Textbox(pl::AABB2D aabb);

	~Textbox();

	void Update();
	void Render();

	void SetBody(pl::RectangleShape& body);
	void SetText(pl::Text& text);

	pl::RectangleShape& GetBody();
	pl::Text& GetText();
private:
	void HandleShift(char c);
	pl::AABB2D m_hitbox;
	pl::RectangleShape* m_body;
	pl::Text* m_text;
};