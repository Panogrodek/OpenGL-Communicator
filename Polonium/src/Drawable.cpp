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
}

void Drawable::SetPosition(glm::vec2 position)
{
	p_position = position;
	p_updateVertices = true;
}

void Drawable::SetSize(glm::vec2 size)
{
	p_size = size;
	p_updateVertices = true;
}

void Drawable::SetRotation(float rotation)
{
	p_rotation = rotation;
	p_updateVertices = true;
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
