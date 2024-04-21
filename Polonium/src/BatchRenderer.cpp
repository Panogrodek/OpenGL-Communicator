#include "plpch.h"
#include "Graphics/BatchRenderer.hpp"
#include <GL/glew.h>

using namespace pl;
using namespace priv;

void BatchRenderer::Init(uint32_t maxQuads)
{
	m_maxVertices = maxQuads * 4;
	m_maxIndicies = maxQuads * 6;

	m_QuadVertexAttribBase = nullptr;
	m_QuadVertexAttribPtr = nullptr;

	m_QuadVertexArray = new VertexArray;

	m_QuadVertexBuffer = new VertexBuffer(m_maxVertices * sizeof(QuadVertexAttrib));
	BufferLayout layout = {
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float4, "a_Color"},
	};
	m_QuadVertexBuffer->SetLayout(layout);
	m_QuadVertexArray->AddVertexBuffer(m_QuadVertexBuffer);

	m_QuadVertexAttribBase = new QuadVertexAttrib[m_maxVertices];
	uint32_t* indicies = new uint32_t[m_maxIndicies];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < m_maxIndicies; i += 6)
	{
		indicies[i + 0] = offset + 0;
		indicies[i + 1] = offset + 1;
		indicies[i + 2] = offset + 2;
		
		indicies[i + 3] = offset + 2;
		indicies[i + 4] = offset + 3;
		indicies[i + 5] = offset + 0;

		offset += 4;
	}

	m_QuadIndexBuffer = new IndexBuffer(indicies, m_maxIndicies);
	m_QuadVertexArray->SetIndexBuffer(m_QuadIndexBuffer);
	delete[] indicies;

	m_QuadVertexAttribPtr = m_QuadVertexAttribBase;
	for (int i = 0; i < m_maxVertices; i++) {
		m_QuadVertexAttribPtr->Position = glm::vec3(0.f);
		m_QuadVertexAttribPtr->Color = glm::vec4(0.f);
		m_QuadVertexAttribPtr++;
	}

	m_QuadShader = new Shader("res/shaders/FlatColor.glsl");
}

void BatchRenderer::Destroy()
{
	delete m_QuadVertexArray;
	delete m_QuadVertexBuffer;
	delete m_QuadIndexBuffer;
	delete m_QuadShader;
	//delete[] m_QuadVertexAttribBase; //?
}

void BatchRenderer::Draw(Drawable* object)
{
	object->UpdateVertices();
	switch (object->p_type)
	{
	case Shape::Rectangle:
		for (int i = 0; i < object->p_vertexCount; i++) {
			m_QuadVertexAttribPtr->Position = glm::vec3(object->p_transformedVertices[i], 1.f);
			m_QuadVertexAttribPtr->Color = object->p_baseVertices[i].color;
			m_QuadVertexAttribPtr++;
		}
		m_QuadIndexCount += 6;
		break;
	default:
		break;
	}

}

void BatchRenderer::Flush()
{
	uint32_t dataSize = (uint8_t*)m_QuadVertexAttribPtr - (uint8_t*)m_QuadVertexAttribBase;
	m_QuadVertexBuffer->SetData(m_QuadVertexAttribBase, dataSize);

	//for (uint32_t i = 0; i < m_TextureSlotIndex; i++) {
	//	m_TextureSlots[i]->Bind(i);
	//}

	m_QuadShader->Bind();

	m_QuadVertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_QuadIndexCount, GL_UNSIGNED_INT, nullptr);

	m_QuadIndexCount = 0;
	m_QuadVertexAttribPtr = m_QuadVertexAttribBase;
}