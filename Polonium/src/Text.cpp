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

void Text::AddChar(char newChar)
{
	std::string newData;
	newData += newChar;
	AddString(newData);
}

void Text::AddString(std::string newString)
{
	//We need to add the new data
	int length = m_stringText.length() + newString.length();
	if (length <= 0) {
		ASSERT(length <= 0); //this does not make sense
		return;
	}

	int newline = 0;
	for (auto& c : std::string(m_stringText + newString))
		if (c == '\n') newline++;

	int newVertexCount = (length - newline) * 4;

	if (newVertexCount == p_vertexCount) { //we added a new line, no vertices are added
		UpdateVertexFontData(newString);
		m_stringText += newString;
		return;
	}

	Vertex* newVertexArray = new Vertex[newVertexCount];
	glm::vec2* newTransformedArray = new glm::vec2[newVertexCount];

	if (p_baseVertices != nullptr) {
		std::memcpy(newVertexArray, p_baseVertices, sizeof(Vertex) * p_vertexCount);
		delete[] p_baseVertices;
	}
	if (p_transformedVertices != nullptr) {
		std::memcpy(newTransformedArray, p_transformedVertices, sizeof(glm::vec2) * p_vertexCount);
		delete[] p_transformedVertices;
	}

	p_baseVertices = newVertexArray;
	p_transformedVertices = newTransformedArray;
	p_vertexCount = newVertexCount;

	UpdateVertexFontData(newString);
	m_stringText += newString;
}

void Text::RemoveLast()
{
	if (m_stringText.empty())
		return;

	if (m_stringText.back() == '\n') {
		m_stringText.pop_back();
		InitVertices(); //here's the thing. This sucks. But now i know why text NEEDS to have line structs.
		return;
	}


	int newline = 0;
	for (auto& c : m_stringText)
		if (c == '\n') newline++;
	int newVertexCount = (m_stringText.length() -newline - 1) * 4;

	Vertex* newVertexArray = new Vertex[newVertexCount];
	glm::vec2* newTransformedArray = new glm::vec2[newVertexCount];

	if (p_baseVertices != nullptr) {
		std::memcpy(newVertexArray, p_baseVertices, sizeof(Vertex) * newVertexCount);
		delete[] p_baseVertices;
	}
	if (p_transformedVertices != nullptr) {
		std::memcpy(newTransformedArray, p_transformedVertices, sizeof(glm::vec2) * newVertexCount);
		delete[] p_transformedVertices;
	}

	p_baseVertices = newVertexArray;
	p_transformedVertices = newTransformedArray;
	p_vertexCount = newVertexCount;


	auto& ch = fontManager.GetFont(m_font).GetCharacter(m_stringText.back());
	m_cursorPos -= glm::vec2{ch.xAdvance,0.f};
	m_stringText.pop_back();
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
		InitVertices();
}

Font& Text::GetFont()
{
	return Font(); //this is wrong TODO: fix
}

float Text::GetTextBottomPos()
{
	return m_bottom * p_size.y;
}

void Text::SetDrawingColor(glm::vec4 color)
{
	m_drawingColor = color;
}

glm::vec4 Text::GetDrawingColor() const
{
	return m_drawingColor;
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

void Text::UpdateVertexFontData(std::string newData)
{
	float xmax = INFINITY;
	glm::vec2 pos = { 0.f,0.f }; //origin in the top left
	if (!m_stringText.empty()) {
		pos = m_cursorPos;
	}
	if (m_textBounds.GetArea() != 0.f) {
		xmax = 0.f + (m_textBounds.upperBound.x - m_textBounds.lowerBound.x) * 1.f / p_size.x; //wrapping for line x
	}

	//some text attribs
	float lineSkip = 95.f; //line height (should not be here)
	float pngSize = 512.f; //should not be here, also should be a vec2
	int newline = 0;
	for (auto& c : m_stringText)
		if (c == '\n') newline++;

	int charVertexCount = (m_stringText.length()-newline) * 4; //dont forget the newlines

	//update vertices only for the new data //WARNING! SPACE FOR THE VERTICES SHOULD ALREADY BE RESERVED!
	for (auto& c : newData)
	{
		if (c == '\n') {
			pos.x = 0.f;
			pos.y -= lineSkip;
			m_cursorPos = pos;
			continue;
		}
		auto& ch = fontManager.GetFont(m_font).GetCharacter(c);

		float x = pos.x + ch.offset.x;
		float y = pos.y - ch.offset.y;
		float w = ch.pngSize.x;
		float h = ch.pngSize.y;

		if (x + w > xmax) {
			pos.x = 0.f;
			pos.y -= lineSkip;

			x = pos.x + ch.offset.x;
			y = pos.y - ch.offset.y;
		}

		if (x < 0.f) { //do not go out of bounds when starting
			float diff = 0.f - x;
			x += diff;
			pos.x += diff;
		}
		pos.x += ch.xAdvance;

		/* Skip glyphs that have no pixels */
		//if (!w || !h)
		//	continue;
		// update VBO for each character
		p_baseVertices[charVertexCount + 0].position = glm::vec2{ x		,y - h };
		p_baseVertices[charVertexCount + 1].position = glm::vec2{ x		,y };
		p_baseVertices[charVertexCount + 2].position = glm::vec2{ x + w	,y };
		p_baseVertices[charVertexCount + 3].position = glm::vec2{ x + w	,y - h };

		p_baseVertices[charVertexCount + 0].color = m_drawingColor;
		p_baseVertices[charVertexCount + 1].color = m_drawingColor;
		p_baseVertices[charVertexCount + 2].color = m_drawingColor;
		p_baseVertices[charVertexCount + 3].color = m_drawingColor;

		p_baseVertices[charVertexCount + 1].texCoords = 1.f / pngSize * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 0.0f, pngSize - (ch.pngPos.y + ch.pngSize.y * 0.0f) };
		p_baseVertices[charVertexCount + 0].texCoords = 1.f / pngSize * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 0.0f, pngSize - (ch.pngPos.y + ch.pngSize.y * 1.0f) };
		p_baseVertices[charVertexCount + 3].texCoords = 1.f / pngSize * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 1.0f, pngSize - (ch.pngPos.y + ch.pngSize.y * 1.0f) };
		p_baseVertices[charVertexCount + 2].texCoords = 1.f / pngSize * glm::vec2{ ch.pngPos.x + ch.pngSize.x * 1.0f, pngSize - (ch.pngPos.y + ch.pngSize.y * 0.0f) };

		p_baseVertices[charVertexCount + 0].texIndex = 1;
		p_baseVertices[charVertexCount + 1].texIndex = 1;
		p_baseVertices[charVertexCount + 2].texIndex = 1;
		p_baseVertices[charVertexCount + 3].texIndex = 1;
		charVertexCount += 4;
		m_cursorPos = pos;
		m_bottom = m_cursorPos.y - h;
	}

	for (int i = 0; i < p_vertexCount; i++) {
		p_transformedVertices[i] = p_baseVertices[i].position;
	}
	p_updateVertices = true;
	UpdateVertices();
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
	glm::vec2 pos = { 0.f,0.f }; //origin
	if (m_textBounds.GetArea() != 0.f) {
		xmax = pos.x + (m_textBounds.upperBound.x - m_textBounds.lowerBound.x) * 1.f/p_size.x; //wrapping for line x
	}

	//some text attribs
	float lineSkip = 95; //line height
	int charVertexCount = 0;

    for (auto& c : m_stringText)
    {
		if (c == '\n') {
			pos.x = 0.f;
			pos.y -= lineSkip;
			m_cursorPos = pos;
			continue;
		}
        auto& ch = fontManager.GetFont(m_font).GetCharacter(c);

		float x = pos.x + ch.offset.x;
		float y = pos.y - ch.offset.y;
		float w = ch.pngSize.x;
		float h = ch.pngSize.y;

		if (x + w > xmax) {
			pos.x = 0.f;
			pos.y -= lineSkip;
			
			x = pos.x + ch.offset.x;
			y = pos.y - ch.offset.y;
		}

		if (x < 0.f) { //do not go out of bounds when starting
			float diff = 0.f - x;
			x += diff;
			pos.x += diff;
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
		m_cursorPos = pos;
		m_bottom = m_cursorPos.y + h;
    }

	for (int i = 0; i < p_vertexCount; i++) {
		p_transformedVertices[i] = p_baseVertices[i].position;
	}
	p_updateVertices = true;
	UpdateVertices();
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

			p_transformedVertices[i] = p_position + p_baseVertices[i].position * p_size/*glm::vec2(rx + p_position.x, ry + p_position.y)*/;
		}
	}
	p_updateVertices = false;
}
