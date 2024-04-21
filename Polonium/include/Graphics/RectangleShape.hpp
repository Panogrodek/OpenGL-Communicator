#pragma once
#include "Drawable.hpp"

namespace pl {
	class RectangleShape : public Drawable {
	public:
		RectangleShape();
		RectangleShape(glm::vec2 pos, glm::vec2 size, float rotation = 0);
		~RectangleShape();

	private:
		virtual void InitVertices();
		virtual void UpdateVertices();
	};
}