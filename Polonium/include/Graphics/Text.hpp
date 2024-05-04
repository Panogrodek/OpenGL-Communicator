#pragma once
#include "Drawable.hpp"
#include "Font.hpp"

namespace pl {
	class Text : public Drawable {
	public:
		Text();
		Text(std::string font, std::string stringText = "", glm::vec2 position = glm::vec2{0.f}, glm::vec2 size = glm::vec2{1.f});
		~Text();

		void AddChar(char newChar);
		void AddString(std::string newString);

		void RemoveLast();

		void SetString(std::string stringText); //very expensive operation, TODO: optimise
		std::string GetString() const;

		void SetFont(Font& font);
		Font& GetFont();

		void SetDrawingColor(glm::vec4 color);
		glm::vec4 GetDrawingColor() const;

		void SetTextBounds(pl::AABB2D bounds);
		pl::AABB2D GetTextBounds(pl::AABB2D bounds) const;

		void Update() { UpdateVertices(); };
	private:
		void UpdateVertexFontData(std::string newData);

		virtual void InitVertices();
		virtual void UpdateVertices(); //this determines what we do in the transform
		std::string m_stringText{};
		std::string m_font{};

		glm::vec2 m_cursorPos{};
		glm::vec4 m_drawingColor{1.0f};

		pl::AABB2D m_textBounds{};
	};	
}