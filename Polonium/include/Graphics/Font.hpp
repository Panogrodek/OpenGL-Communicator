#pragma once
#include "freetype/ft2build.h"
#include FT_FREETYPE_H  

#include <unordered_map>
#include <string>

namespace priv {
	class FontManager;
}

namespace pl {
	struct Character {
		glm::vec2 Advance;    // Offset to advance to next glyph
		glm::vec2 BitmapSize;
		glm::vec2 BitmapPos;

		float tx;
		float ti;
	};

	class Font {
	public:
		Font(std::string name = "");
	private:
		friend class priv::FontManager;
		friend class Text;
	
		uint32_t m_RendererId;
		glm::ivec2 m_atlasSize;
		std::string m_name;
		std::unordered_map<char,Character> m_characters;
	};
}

namespace priv {
	class FontManager {
	public:
		bool InitFreeType();
		bool CloseFreeType();

		void LoadFont(std::string filePath, std::string name = "");
		pl::Font GetFont(std::string name);
		
	private:
		bool CheckIfExists(std::string name);

		FT_Library m_ft;
		std::unordered_map<std::string, pl::Font> m_fonts;
	};
}

namespace pl {
	inline priv::FontManager fontManager;
}