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
    for (auto& c : m_stringText)
    {
        Character ch = m_font->m_characters[c];

        float xpos = base + ch.Bearing.x * p_size.x;
        float ypos = p_position.y - (ch.Size.y - ch.Bearing.y) * p_position.y;

        float w = ch.Size.x * p_size.x;
        float h = ch.Size.y * p_size.y;
        // update VBO for each character
		
		int i = charCounter * 4;
		p_baseVertices[i+0].position = glm::vec2{ xpos,ypos };
		p_baseVertices[i+1].position = glm::vec2{ xpos,ypos + h };
		p_baseVertices[i+2].position = glm::vec2{ xpos + w,ypos + h};
		p_baseVertices[i+3].position = glm::vec2{ xpos + w,ypos };

		p_baseVertices[i + 0].color = glm::vec4{1.0f};
		p_baseVertices[i + 1].color = glm::vec4{1.0f};
		p_baseVertices[i + 2].color = glm::vec4{1.0f};
		p_baseVertices[i + 3].color = glm::vec4{1.0f};

		p_baseVertices[i+0].texCoords = glm::vec2{ 0.f,0.f };
		p_baseVertices[i+1].texCoords = glm::vec2{ 0.f,1.f };
		p_baseVertices[i+2].texCoords = glm::vec2{ 1.f,1.f };
		p_baseVertices[i+3].texCoords = glm::vec2{ 1.f,0.f };

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        base += (ch.Advance >> 6) * p_size.x; // bitshift by 6 to get value in pixels (2^6 = 64)
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
