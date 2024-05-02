#pragma once
#include "Drawable.hpp"
#include "Font.hpp"

namespace pl {
	class Text : public Drawable {
	public:
		Text(std::string stringText = "", Font* font = nullptr, glm::vec2 position = glm::vec2{0.f}, glm::vec2 size = glm::vec2{1.f});

		void SetString(std::string stringText);
		std::string GetString() const;

		void SetFont(Font* font);
		Font* GetFont();
	private:
		virtual void InitVertices();
		virtual void UpdateVertices(); //this determines what we do in the transform
		std::string m_stringText{};
		Font* m_font = nullptr;
	};	
}