#include "plpch.h"
#include "Graphics/Text.hpp"
#include <IMGUI/imgui.h>
#include <GLM/gtc/type_ptr.hpp>

using namespace pl;

Text::Text()
{
}

Text::Text(std::string font, std::string stringText, glm::vec2 position, glm::vec2 size) :
	m_stringText(stringText), m_font(font)
{
    p_position = position;
    p_size = size;
	p_type = Shape::Text;
	if(m_font != "")
		InitVertices();
}

Text::~Text()
{
	if (p_baseVertices != nullptr)
		delete[] p_baseVertices;
	if (p_transformedVertices != nullptr)
		delete[] p_transformedVertices;
}

void Text::SetString(std::string stringText)
{
	m_stringText = stringText;
	if(m_font != "")
		InitVertices();
}

std::string Text::GetString() const
{
	return m_stringText;
}

void Text::SetFont(Font& font)
{
	if (m_font != "")
		UpdateVertices();
}

Font& Text::GetFont()
{
	return Font();
}

void Text::SetTextBounds(AABB2D bounds)
{
	m_textBounds = bounds;
	if (bounds.GetArea() != 0.f) {
		glm::vec2 size = bounds.upperBound - bounds.lowerBound;
		p_position = glm::vec2(bounds.lowerBound.x, bounds.upperBound.y);
	}
	InitVertices();
}

AABB2D Text::GetTextBounds(AABB2D bounds) const
{
	return m_textBounds;
}

void Text::InitVertices()
{
	//cleaning previous vertices
	int newline = 0;
	for (auto& c : m_stringText)
		if (c == '\n') newline++;

	if (p_baseVertices != nullptr)
		delete[] p_baseVertices;
	if (p_transformedVertices != nullptr)
		delete[] p_transformedVertices;

	p_vertexCount = (m_stringText.length()-newline) * 4; //each char quad has 4 vertices
	p_baseVertices = new Vertex[p_vertexCount]{};
	p_transformedVertices = new glm::vec2[p_vertexCount]{};

	float xmax = INFINITY;
	glm::vec2 pos = p_position;
	if (m_textBounds.GetArea() != 0.f) {
		xmax = pos.x + (m_textBounds.upperBound.x - m_textBounds.lowerBound.x);
	}

	//some text attribs
	float lineSkip = 64;
	int charVertexCount = 0;

    for (auto& c : m_stringText)
    {
		if (c == '\n') {
			pos.x = p_position.x;
			pos.y -= lineSkip;
			continue;
		}
        auto& ch = fontManager.GetFont(m_font).GetCharacter(c);

		float x = pos.x + ch.offset.x;
		float y = pos.y - ch.offset.y;
		float w = ch.pngSize.x;
		float h = ch.pngSize.y;

		if (x + w > xmax) {
			pos.x = p_position.x;
			pos.y -= lineSkip;
			
			x = pos.x + ch.offset.x;
			y = pos.y - ch.offset.y;
		}
		pos.x += ch.xAdvance;

		/* Skip glyphs that have no pixels */
		//if (!w || !h)
		//	continue;
        // update VBO for each character
		p_baseVertices[charVertexCount + 0].position = glm::vec2{ x		,y - h	};
		p_baseVertices[charVertexCount + 1].position = glm::vec2{ x		,y };
		p_baseVertices[charVertexCount + 2].position = glm::vec2{ x + w	,y };
		p_baseVertices[charVertexCount + 3].position = glm::vec2{ x + w	,y - h	};
					   
		p_baseVertices[charVertexCount + 0].color = glm::vec4{1.0f};
		p_baseVertices[charVertexCount + 1].color = glm::vec4{1.0f};
		p_baseVertices[charVertexCount + 2].color = glm::vec4{1.0f};
		p_baseVertices[charVertexCount + 3].color = glm::vec4{1.0f};
					   
		p_baseVertices[charVertexCount + 1].texCoords = 1.f/512.f * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 0.0f, 512 - (ch.pngPos.y + ch.pngSize.y * 0.0f)};
		p_baseVertices[charVertexCount + 0].texCoords = 1.f/512.f * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 0.0f, 512 - (ch.pngPos.y + ch.pngSize.y * 1.0f)};
		p_baseVertices[charVertexCount + 3].texCoords = 1.f/512.f * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 1.0f, 512 - (ch.pngPos.y + ch.pngSize.y * 1.0f)};
		p_baseVertices[charVertexCount + 2].texCoords = 1.f/512.f * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 1.0f, 512 - (ch.pngPos.y + ch.pngSize.y * 0.0f)};
					   
		p_baseVertices[charVertexCount + 0].texIndex = 1;
		p_baseVertices[charVertexCount + 1].texIndex = 1;
		p_baseVertices[charVertexCount + 2].texIndex = 1;
		p_baseVertices[charVertexCount + 3].texIndex = 1;
		charVertexCount += 4;
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
			//float rx = 0.f;
			//float ry = 0.f;

			p_transformedVertices[i] = glm::vec2(rx + p_position.x, ry + p_position.y);
		}
	}
	p_updateVertices = false;
}
