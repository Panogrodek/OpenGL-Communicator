#pragma once
#include <GL/glew.h>
#include <string>

namespace pl {
	class Texture {
	public:
		Texture(const std::string& path);
		Texture(uint32_t width, uint32_t height);
		virtual ~Texture();

		uint32_t GetWidth()  const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		void SetData(void* data, glm::ivec2 size, glm::ivec2 pos = glm::ivec2(0));

		void Bind(uint32_t slot = 0) const;
		bool operator==(const Texture& other) const
		{
			return m_RendererID == other.m_RendererID;
		}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}