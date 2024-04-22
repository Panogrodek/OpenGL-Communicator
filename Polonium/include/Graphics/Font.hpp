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
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	class Font {
	public:
		Font(std::string name = "");
	private:
		friend class priv::FontManager;
		friend class Text;

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