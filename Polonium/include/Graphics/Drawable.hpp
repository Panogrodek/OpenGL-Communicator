#pragma once

namespace priv {
	class BatchRenderer;
}

namespace pl {
	enum class Shape {
		None = 0,
		Rectangle,
	};

	struct Vertex {
		glm::vec2 position{};
		glm::vec4 color = glm::vec4(1.0f);
	};

	class Drawable {
	public:
		Drawable() = default;
		virtual ~Drawable() = default;

		//Transform functions 
		virtual void Move(glm::vec2 position);
		virtual void IncreaseSize(glm::vec2 size);
		virtual void Rotate(float rotation);
		virtual void SetPosition(glm::vec2 position);
		virtual void SetSize(glm::vec2 size);
		virtual void SetRotation(float rotation);

		//getters
		virtual glm::vec2 GetPosition() const;
		virtual glm::vec2 GetSize() const;
		virtual float GetRotation() const;

		//Rendering
		virtual void SetColor(glm::vec4 color);
		virtual glm::vec4 GetColor() const;
		virtual void Draw();
	protected:
		friend class priv::BatchRenderer;
		virtual void InitVertices() = 0;
		virtual void UpdateVertices() = 0; //this determines what we do in the transform

		Vertex* p_baseVertices = nullptr;
		glm::vec2* p_transformedVertices = nullptr;
		uint8_t p_vertexCount = 0;

		Shape p_type = Shape::None;

		glm::vec2 p_position{};
		glm::vec2 p_size{};
		float p_rotation{};
		bool p_updateVertices = true;
	};
}