#include "plpch.h"
#include "Utilities/Mouse.hpp"

using namespace priv;

glm::vec2 Mouse::GetOffset() const
{
	return m_offset;
}

void Mouse::BlockOffset()
{
	m_offset = glm::vec2(0.f);
}
