#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Graphics/Shader.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/Camera.hpp"
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

		pl::Camera* GetCamera();
		void SetCamera(pl::Camera* camera);
		
	private:
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		GLFWwindow* m_window;
		glm::vec2 m_mousePos{};
		pl::Camera* m_camera = nullptr;
	};
}
namespace pl {
	inline priv::Renderer renderer{};
}