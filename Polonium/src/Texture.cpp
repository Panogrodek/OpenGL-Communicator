#include "plpch.h"
#include "Graphics/Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "STB_IMAGE/stb_image.h"

using namespace pl;

Texture::Texture(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(const std::string& path) :
	m_Path(path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* data = nullptr;
	{
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	}
	if (!data) {
		spdlog::critical("Failed to load image: {}", path);
		ASSERT(true);
	}
	m_Width = width;
	m_Height = height;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	m_InternalFormat = internalFormat;
	m_DataFormat = dataFormat;

	//ENG_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

void Texture::SetData(void* data, glm::ivec2 size, glm::ivec2 pos)
{
	if (size.x > m_Width || size.y > m_Height)
		return;
	glTextureSubImage2D(m_RendererID, 0, pos.x, pos.y, size.x, size.y, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_RendererID);
}

