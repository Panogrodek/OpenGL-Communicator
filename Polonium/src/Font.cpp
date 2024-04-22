#include "plpch.h"
#include "Graphics/Font.hpp"
#include <GL/glew.h>

using namespace priv;
using namespace pl;

Font::Font(std::string name)
{
    m_name = name;
}

void FontManager::LoadFont(std::string filePath, std::string name)
{
    if (name == "") {
        spdlog::warn("font name is not determined! Assigning new name: {}", filePath);
        name = filePath;
        if (CheckIfExists(name))
            return;
    }

    m_fonts[name] = Font(name);

    FT_Face face;
    if (FT_New_Face(m_ft, filePath.c_str(), 0, &face))
    {
        spdlog::error("Failed to load font: {}", filePath);
        m_fonts.erase(m_fonts.find(name));
        return;
    }
    spdlog::info("Sucessfully loaded font {} from: {}", name,filePath);

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    auto& font = m_fonts[name];
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            spdlog::error("Failed to load Glyph!");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        font.m_characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
}

Font FontManager::GetFont(std::string name)
{
    if (CheckIfExists(name))
        return m_fonts[name];

    ASSERT(true); //Accessing not existing resource
    spdlog::critical("Accessing not existing resource!");
    return Font("");
}

bool FontManager::InitFreeType()
{
    if (FT_Init_FreeType(&m_ft))
    {
        spdlog::critical("Could not init FreeType Library!");
        return false;
    }
    return true;
}

bool FontManager::CloseFreeType()
{
    if (FT_Done_FreeType(m_ft)) {
        spdlog::error("Could not close FreeType!");
        return false;
    }
    return true;
}

bool FontManager::CheckIfExists(std::string name)
{
    if (m_fonts.find(name) == m_fonts.end()) {
        spdlog::error("Font {} already exists!");
        return false;
    }
    return true;
}
