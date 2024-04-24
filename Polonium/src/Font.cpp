#include "plpch.h"
#include "Graphics/Font.hpp"
#include <GL/glew.h>
#include <algorithm>

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
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    FT_GlyphSlot g = face->glyph;
    unsigned int w = 0;
    unsigned int h = 0;
    auto& font = m_fonts[name];
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            spdlog::error("Loading character {} failed!", i);
            continue;
        }

        w += g->bitmap.width;
        h = std::max(h, g->bitmap.rows);
    }

    /* you might as well save this value as it is needed later on */
    font.m_atlasSize = { w,h };

    GLuint tex;
    int rendererid;
    glCreateTextures(GL_TEXTURE_2D, 1, &font.m_RendererId);
    glTextureStorage2D(font.m_RendererId, 1, GL_RED, w, h);

    //glTextureParameteri(font.m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTextureParameteri(font.m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTextureParameteri(font.m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTextureParameteri(font.m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int x = 0;
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;
        
        glTextureSubImage2D(font.m_RendererId, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        font.m_characters[char(i)] = Character();
        auto& c = font.m_characters[i];

        x += g->bitmap.width;
        c.Advance.x = g->advance.x >> 6;
        c.Advance.y = g->advance.y >> 6;

        c.BitmapSize.x = g->bitmap.width;
        c.BitmapSize.y = g->bitmap.rows;

        c.BitmapPos.x = g->bitmap_left;
        c.BitmapPos.y = g->bitmap_top;

        c.tx = (float)x / w;
        c.ti = 0;
    }

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
