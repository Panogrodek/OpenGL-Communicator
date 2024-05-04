#include "plpch.h"
#include "Graphics/BatchRenderer.hpp"
#include "Graphics/Renderer.hpp"
#include <GL/glew.h>

using namespace pl;
using namespace priv;

void BatchRenderer::Init(uint32_t maxQuads)
{
	m_maxVertices = maxQuads * 4;
	m_maxIndicies = maxQuads * 6;

	m_QuadVertexAttribBase = nullptr;
	m_QuadVertexAttribPtr = nullptr;
	m_QuadIndexCount = 0;
	m_QuadVertexArray = new VertexArray;

	m_QuadVertexBuffer = new VertexBuffer(m_maxVertices * sizeof(QuadVertexAttrib));
	BufferLayout layout = {
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float4, "a_Color"},
		{ShaderDataType::Float2, "a_TexCoords"},
		{ShaderDataType::Float, "a_TexIndex"},
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
		m_QuadVertexAttribPtr->Position = { 0.f,0.f,0.f };
		m_QuadVertexAttribPtr->Color = { 0.f,0.f,0.f,0.f };
		m_QuadVertexAttribPtr->TexCoords = { 0.f,0.f };
		m_QuadVertexAttribPtr->TextureIndex = 0.f;
		m_QuadVertexAttribPtr++;
	}

	m_QuadShader = new Shader("res/shaders/FlatColor.glsl");

	int32_t samplers[32]{};
	for (uint32_t i = 0; i < 32; i++)
		samplers[i] = i;

	/*m_Textures[0] = new Texture("res/textures/tymon.png");*/
	m_Textures[0] = new Texture(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	m_Textures[0]->SetData(&whiteTextureData,sizeof(whiteTextureData));

	m_QuadShader->Bind();
	m_QuadShader->SetIntArray("u_Textures", samplers, 32);
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
	if (object->p_type != Shape::Rectangle && object->p_type != Shape::Text)
		return;

	for (int i = 0; i < object->p_vertexCount; i++) {
		m_QuadVertexAttribPtr->Position = glm::vec3(object->p_transformedVertices[i], 0.f);
		m_QuadVertexAttribPtr->Color = object->p_baseVertices[i].color;
		m_QuadVertexAttribPtr->TexCoords = object->p_baseVertices[i].texCoords;
		m_QuadVertexAttribPtr->TextureIndex = object->p_baseVertices[i].texIndex;
		m_QuadVertexAttribPtr++;
	}
	m_QuadIndexCount += 6 * object->p_vertexCount / 4;

//this would be the correct way for rendering other primitive types (triangles, lines, circles)
#pragma region other way
	//switch (object->p_type)
	//{
	//case Shape::Rectangle:
	//	for (int i = 0; i < object->p_vertexCount; i++) {
	//		m_QuadVertexAttribPtr->Position = glm::vec3(object->p_transformedVertices[i], 1.f);
	//		m_QuadVertexAttribPtr->Color = object->p_baseVertices[i].color;
	//		m_QuadVertexAttribPtr->TexCoords = object->p_baseVertices[i].texCoords;
	//		m_QuadVertexAttribPtr->TextureIndex = object->p_baseVertices[i].texIndex;
	//		m_QuadVertexAttribPtr++;
	//	}
	//	m_QuadIndexCount += 6;
	//	break;
	//case Shape::Text:
	//	for (int i = 0; i < object->p_vertexCount; i++) {
	//		m_QuadVertexAttribPtr->Position = glm::vec3(object->p_transformedVertices[i], 1.f);
	//		m_QuadVertexAttribPtr->Color = object->p_baseVertices[i].color;
	//		m_QuadVertexAttribPtr->TexCoords = object->p_baseVertices[i].texCoords;
	//		m_QuadVertexAttribPtr++;
	//	}
	//	m_QuadIndexCount += 6 * object->p_vertexCount/4;
	//	break;
	//default:
	//	break;
	//}
#pragma endregion other way
}

void BatchRenderer::SceneBegin()
{
	m_QuadIndexCount = 0;
	m_QuadVertexAttribPtr = m_QuadVertexAttribBase;
}

void BatchRenderer::Flush()
{
	uint32_t dataSize = (uint8_t*)m_QuadVertexAttribPtr - (uint8_t*)m_QuadVertexAttribBase;
	m_QuadVertexBuffer->SetData(m_QuadVertexAttribBase, dataSize);

	for (uint32_t i = 0; i < 1; i++) {
		m_Textures[i]->Bind(i);
	}

	auto* camera = pl::renderer.GetCamera();
	if (camera == nullptr)
		m_QuadShader->SetMat4("u_ViewProjection", glm::mat4(1.f));
	else
		m_QuadShader->SetMat4("u_ViewProjection",camera->GetViewProjection());
	glBindTextureUnit(1,2);

	m_QuadShader->Bind();

	m_QuadVertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_QuadIndexCount, GL_UNSIGNED_INT, nullptr);
}
