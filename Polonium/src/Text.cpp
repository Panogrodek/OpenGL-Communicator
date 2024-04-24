#include "plpch.h"
#include "Graphics/Text.hpp"

using namespace pl;

Text::Text(std::string stringText, Font* font, glm::vec2 position, glm::vec2 size) :
	m_stringText(stringText), m_font(font)
{
    p_position = position;
    p_size = size;
	p_type = Shape::Text;
	InitVertices();
}

void Text::SetString(std::string stringText)
{
	m_stringText = stringText;
	InitVertices();
}

std::string Text::GetString() const
{
	return m_stringText;
}

void Text::SetFont(Font* font)
{
	m_font = font;
}

Font* Text::GetFont()
{
	return m_font;
}

void Text::InitVertices()
{
	p_vertexCount = m_stringText.length() * 4; //each char quad has 4 vertices
	if (p_baseVertices != nullptr)
		delete[] p_baseVertices;
	if (p_transformedVertices != nullptr)
		delete[] p_transformedVertices;

	p_baseVertices = new Vertex[p_vertexCount];
	p_transformedVertices = new glm::vec2[p_vertexCount];

	float base = p_position.x;
	int charCounter = 0;
	glm::vec2 pos = p_position;
	glm::vec2 size = p_size;

    for (auto& c : m_stringText)
    {
        Character ch = m_font->m_characters[c];

		float x2 = pos.x + ch.BitmapPos.x * size.x;
		float y2 = -pos.y - ch.BitmapPos.y * size.y;
		float w = ch.BitmapSize.x * size.x;
		float h = ch.BitmapSize.y * size.y;

		/* Advance the cursor to the start of the next character */
		pos.x += ch.Advance.x * size.x;
		pos.y += ch.Advance.y * size.y;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;
        // update VBO for each character
		
		int i = charCounter * 4;
		p_baseVertices[i+0].position = glm::vec2{ pos.x		,pos.y};
		p_baseVertices[i+1].position = glm::vec2{ pos.x		,pos.y + h };
		p_baseVertices[i+2].position = glm::vec2{ pos.x + w	,pos.y + h};
		p_baseVertices[i+3].position = glm::vec2{ pos.x + w	,pos.y };

		p_baseVertices[i + 0].color = glm::vec4{1.0f};
		p_baseVertices[i + 1].color = glm::vec4{1.0f};
		p_baseVertices[i + 2].color = glm::vec4{1.0f};
		p_baseVertices[i + 3].color = glm::vec4{1.0f};

		p_baseVertices[i+0].texCoords = glm::vec2{ ch.tx,ch.BitmapSize.y / m_font->m_atlasSize.y};
		p_baseVertices[i+1].texCoords = glm::vec2{ ch.tx,0.f };
		p_baseVertices[i+2].texCoords = glm::vec2{ ch.tx + ch.BitmapSize.x / m_font->m_atlasSize.x,0.f };
		p_baseVertices[i+3].texCoords = glm::vec2{ ch.tx + ch.BitmapSize.x / m_font->m_atlasSize.x,ch.BitmapSize.y / m_font->m_atlasSize.y };
		
		p_baseVertices[i + 0].texIndex = 1;
		p_baseVertices[i + 1].texIndex = 1;
		p_baseVertices[i + 2].texIndex = 1;
		p_baseVertices[i + 3].texIndex = 1;
		charCounter++;
    }

	for (int i = 0; i < p_vertexCount; i++) {
		p_transformedVertices[i] = p_baseVertices[i].position;
	}
}

void Text::UpdateVertices()
{
	if (p_updateVertices)
	{
		float sin = sinf(p_rotation);
		float cos = cosf(p_rotation);

		for (int i = 0; i < p_vertexCount; i++)
		{
			auto& v = p_baseVertices[i].position * p_size;
			float rx = cos * v.x - sin * v.y;
			float ry = sin * v.x + cos * v.y;

			p_transformedVertices[i] = glm::vec2(rx + p_position.x, ry + p_position.y);
		}
	}
	p_updateVertices = false;
}
