#include "plpch.h"
#include "Utilities/AABB2D.hpp"

using namespace pl;

AABB2D::AABB2D(glm::vec2 _lowerBound, glm::vec2 _upperBound) :
	lowerBound(_lowerBound),upperBound(_upperBound)
{
}

bool AABB2D::contains(glm::vec2 point)
{
	if (point.x < lowerBound.x || point.x > upperBound.x)
		return false;
	if (point.y < lowerBound.y || point.y > upperBound.y)
		return false;
	return true;
}

bool AABB2D::contains(AABB2D other)
{
	return contains(other.lowerBound) && contains(other.upperBound);
}

bool AABB2D::intersects(AABB2D other)
{
	return (lowerBound.x <= other.upperBound.x && upperBound.x >= other.lowerBound.x) &&
		(lowerBound.y <= other.upperBound.y && upperBound.y >= other.lowerBound.y);
}

float AABB2D::GetPerimeter()
{
	glm::vec2 d = upperBound - lowerBound;
	return d.x * d.y;
}

float AABB2D::GetArea()
{
	glm::vec2 d = upperBound - lowerBound;

	return 2.0f * d.x * d.y;
}