#pragma once
#include "VertexArray.hpp"
#include "Drawable.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace priv {
	class Renderer;

	struct QuadVertexAttrib {
		glm::vec3 Position{};
		glm::vec4 Color{};
		glm::vec2 TexCoords{};
		float TextureIndex{};
	};

	class BatchRenderer {
	public:
		void Init(uint32_t maxQuads);
		void Destroy();

		void Draw(pl::Drawable* object);
	private:
		void SceneBegin();
		void Flush();
		friend class priv::Renderer;
		uint32_t m_maxVertices = 0;
		uint32_t m_maxIndicies = 0;

		uint32_t m_QuadIndexCount = 0;

		QuadVertexAttrib* m_QuadVertexAttribBase = nullptr;
		QuadVertexAttrib* m_QuadVertexAttribPtr = nullptr;

		pl::Shader* m_QuadShader = nullptr;
		pl::VertexArray* m_QuadVertexArray = nullptr;
		pl::VertexBuffer* m_QuadVertexBuffer = nullptr;
		pl::IndexBuffer* m_QuadIndexBuffer = nullptr;

		pl::Texture* m_Textures[32];
	};
}

namespace pl {
	inline priv::BatchRenderer batchRenderer;
}