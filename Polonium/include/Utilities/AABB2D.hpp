#pragma once
#include <GLM/glm.hpp>

namespace pl {
	struct AABB2D {
		AABB2D(glm::vec2 lowerBound = glm::vec2{0.f}, glm::vec2 upperBound = glm::vec2{0.f});
		glm::vec2 lowerBound{};
		glm::vec2 upperBound{};

		bool contains(glm::vec2 point);
		bool contains(AABB2D other);
		bool intersects(AABB2D other);
		float GetPerimeter();
		float GetArea();
	};
}