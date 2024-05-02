#pragma once
#include "Drawable.hpp"
#include "Font.hpp"

namespace pl {
	class Text : public Drawable {
	public:
		Text();
		Text(std::string font, std::string stringText = "", glm::vec2 position = glm::vec2{0.f}, glm::vec2 size = glm::vec2{1.f});

		void SetString(std::string stringText);
		std::string GetString() const;

		void SetFont(Font& font);
		Font& GetFont();

		void SetTextBounds(pl::AABB2D bounds);
		pl::AABB2D GetTextBounds(pl::AABB2D bounds) const;
	private:
		virtual void InitVertices();
		virtual void UpdateVertices(); //this determines what we do in the transform
		std::string m_stringText{};
		std::string m_font{};

		pl::AABB2D m_textBounds{};
	};	
}