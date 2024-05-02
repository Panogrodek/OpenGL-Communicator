#pragma once
#include <unordered_map>
#include "Texture.hpp"
#include <string>

namespace priv {
	class FontManager;
}

namespace pl {
	struct Character {
		glm::vec2 pngPos;
		glm::vec2 pngSize;
		glm::vec2 offset;
		float xAdvance;
	};

	class Font {
	public:
		Font(std::string name = "");

		Character GetCharacter(char c);
	private:
		friend class priv::FontManager;
		friend class Text;
	
		pl::Texture* m_fontTexture;
		glm::ivec2 m_atlasSize;
		std::string m_name;
		std::unordered_map<char, Character> m_characters;
	};
}

namespace priv {
	class FontManager {
	public:
		void LoadFont(std::string filePath, std::string name = "");
		void LoadFont(std::string filePathPNG, std::string filePathFNT, std::string name);
		pl::Font GetFont(std::string name);
		
	private:
		bool CheckIfExists(std::string name);
		bool ProcessCharacter(std::string line, pl::Font& font);

		std::unordered_map<std::string, pl::Font> m_fonts;
	};
}

namespace pl {
	inline priv::FontManager fontManager;
}