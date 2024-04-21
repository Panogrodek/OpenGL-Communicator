#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Graphics/Shader.hpp"
#include "Graphics/VertexArray.hpp"
#include "Drawable.hpp"

namespace priv {
	class Renderer {
	public:
		void InitGL();
		void Destroy();
		
		void Draw(pl::Drawable* object);

		void BeginDraw();
		void EndDraw();

		glm::vec2 GetMousePosition();

		GLFWwindow* const GetWindow();

		void UpdateImguiPosition();
	private:
		GLFWwindow* m_window;
		glm::vec2 m_mousePos{};

	};
}
namespace pl {
	inline priv::Renderer renderer{};
}