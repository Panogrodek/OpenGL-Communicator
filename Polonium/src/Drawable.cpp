#include "plpch.h"
#include "Graphics/Drawable.hpp"

#include "Graphics/BatchRenderer.hpp"

using namespace pl;


void Drawable::Move(glm::vec2 position)
{
	p_position += position;
	p_updateVertices = true;
}

void Drawable::IncreaseSize(glm::vec2 size)
{
	p_size += size;
	p_updateVertices = true;
}

void Drawable::Rotate(float rotation)
{
	p_rotation += rotation;
	p_updateVertices = true;
	p_updateAABB2D = true;
}

void Drawable::SetPosition(glm::vec2 position)
{
	p_position = position;
	p_updateVertices = true;
	p_updateAABB2D = true;
}

void Drawable::SetSize(glm::vec2 size)
{
	p_size = size;
	p_updateVertices = true;
	p_updateAABB2D = true;
}

void Drawable::SetRotation(float rotation)
{
	p_rotation = rotation;
	p_updateVertices = true;
	p_updateAABB2D = true;
}

glm::vec2 Drawable::GetPosition() const
{
	return p_position;
}

glm::vec2 Drawable::GetSize() const 
{
	return p_size;
}

float Drawable::GetRotation() const
{
	return p_rotation;
}

AABB2D Drawable::GetAABB2D()
{
	//this is a bad way
	if (p_updateAABB2D)
	{
		glm::vec2 min;
		glm::vec2 max;
		float minX = INFINITY;
		float minY = INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;
		for (int i = 0; i < p_vertexCount; i++)
		{
			auto& v = p_transformedVertices[i];
			if (v.x < minX) { minX = v.x; }
			if (v.x > maxX) { maxX = v.x; }
			if (v.y < minY) { minY = v.y; }
			if (v.y > maxY) { maxY = v.y; }
		}
		min = { minX, minY };
		max = { maxX, maxY };

		p_aabb.lowerBound = min;
		p_aabb.upperBound = max;
	}

	p_updateAABB2D = false;
	return p_aabb;
}

void Drawable::SetColor(glm::vec4 color) {
	for (int i = 0; i < p_vertexCount; i++)
		p_baseVertices[i].color = color;
}

glm::vec4 Drawable::GetColor() const {
	if (p_vertexCount > 0)
		return p_baseVertices[0].color;
	return glm::vec4{ 1.f };
}

void Drawable::Draw() {
	batchRenderer.Draw(this);
}
