#include "plpch.h"
#include "Graphics/RectangleShape.hpp"

using namespace pl;

RectangleShape::RectangleShape()
{
	InitVertices();
	p_type = Shape::Rectangle;
}

RectangleShape::RectangleShape(glm::vec2 pos, glm::vec2 size, float rotation)
{
	InitVertices();
	p_type = Shape::Rectangle;

	p_position = pos;
	p_size = size;
	p_rotation = rotation;
}

RectangleShape::~RectangleShape()
{
	delete[] p_baseVertices;
	delete[] p_transformedVertices;
}

void RectangleShape::InitVertices()
{
	p_baseVertices = new Vertex[4];
	p_transformedVertices = new glm::vec2[4];
	p_vertexCount = 4;

	p_baseVertices[0].position = { -0.5f,-0.5f };
	p_baseVertices[0].texCoords = { 0.0f, 0.0f };

	p_baseVertices[1].position = { -0.5f,+0.5f };
	p_baseVertices[1].texCoords = { 0.0f, 1.0f };
	
	p_baseVertices[2].position = { +0.5f,+0.5f };
	p_baseVertices[2].texCoords = { 1.0f, 1.0f };
	
	p_baseVertices[3].position = { +0.5f,-0.5f };
	p_baseVertices[3].texCoords = { 1.0f, 0.0f };

	for (int i = 0; i < p_vertexCount; i++) {
		p_transformedVertices[i] = p_baseVertices[i].position;
	}
}

void RectangleShape::UpdateVertices()
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
