#include "plpch.h"
#include "Graphics/Font.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <fstream>

using namespace priv;
using namespace pl;

Font::Font(std::string name)
{
    m_name = name;
}

void FontManager::LoadFont(std::string filePath, std::string name)
{
    std::string filePathPNG = "";
    std::string filePathFNT = "";

    if (filePath.substr(filePath.length() - 4, 4) == ".fnt") {
        filePathFNT = filePath;
        filePathPNG = filePath.substr(0, filePath.length() - 3);
        filePathPNG += "png";
    }
    else if (filePath.substr(filePath.length() - 4, 4) == ".png") {
        filePathPNG = filePath;
        filePathFNT = filePath.substr(0, filePath.length() - 3);
        filePathFNT += "fnt";
    }
    else {
        spdlog::error("Could not access font at {}", filePath);
        return;
    }
    LoadFont(filePathPNG,filePathFNT,name);
}

void FontManager::LoadFont(std::string filePathPNG, std::string filePathFNT, std::string name)
{
    std::fstream file;
    file.open(filePathFNT);
    if (!file.good()) {
        spdlog::error("Could not access font at {}", filePathFNT);
        file.close();
        return;
    }

    if (name == "") {
        spdlog::warn("font name is not determined! Assigning new name: {}", filePathFNT);
        name = filePathFNT;
        if (CheckIfExists(name)) {
            file.close();
            return;
        }
    }

    for (auto& [name, font] : m_fonts) {
        if (!CheckIfExists(name))
            continue;
        spdlog::error("Font {} already exists! Failed to initialize font from path {}",name, filePathFNT);
        file.close();
        return;
    }

    m_fonts[name] = Font(name);
    auto& font = m_fonts[name];

    font.m_fontTexture = new Texture(filePathPNG);

    std::string line;
    while (std::getline(file,line)){
        if (line.substr(0, 5) != "char ")
            continue;
        ProcessCharacter(line, font);
    }
}

Font FontManager::GetFont(std::string name)
{
    if (CheckIfExists(name))
        return m_fonts[name];

    ASSERT(true); //Accessing not existing resource
    spdlog::critical("Accessing not existing resource!");
    return Font("");
}

bool FontManager::CheckIfExists(std::string name)
{
    if (m_fonts.find(name) == m_fonts.end()) {
        spdlog::error("Font {} already exists!");
        return false;
    }
    return true;
}

bool FontManager::ProcessCharacter(std::string line, Font& font)
{
    Character character{};

    ErasePart('=', line); //TODO: this is a bad practice, we trust that the fnt format will be the same for all files, very wrong
    char c = char(std::stoi(ErasePart(' ', line)));

    ErasePart('=', line);
    int pngPosX = std::stoi(ErasePart(' ', line));
    ErasePart('=', line);
    int pngPosY = std::stoi(ErasePart(' ', line));
    
    character.pngPos = glm::vec2(pngPosX, pngPosY);

    ErasePart('=', line);
    int pngSizeX = std::stoi(ErasePart(' ', line));
    ErasePart('=', line);
    int pngSizeY = std::stoi(ErasePart(' ', line));

    character.pngSize = glm::vec2(pngSizeX, pngSizeY);

    ErasePart('=', line);
    int offsetX = std::stoi(ErasePart(' ', line));
    ErasePart('=', line);
    int offsetY = std::stoi(ErasePart(' ', line));

    character.offset = glm::vec2(offsetX, offsetY);

    ErasePart('=', line);
    int advanceX = std::stoi(ErasePart(' ', line));

    character.xAdvance = advanceX;

    font.m_characters[c] = character;
    return true; //TODO: this would be implemented if we didnt trust the fnt format
}
