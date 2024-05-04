#include "pch.h"
#include "ui/Logbox.hpp"

using namespace priv;

void LogBox::Init(pl::AABB2D aabb)
{
	glm::vec2 size = aabb.upperBound - aabb.lowerBound;
	m_body = new pl::RectangleShape(aabb.lowerBound + size / 2.f, size);

	m_text = new pl::Text("arial");
	m_text->SetSize({ 0.5f,0.5f });
	m_text->SetTextBounds(aabb);
}

LogBox::~LogBox()
{
	delete m_body;
	delete m_text;
}

void LogBox::Update()
{
}

void LogBox::Render()
{
	m_body->Draw();
	m_text->Draw();
}

pl::RectangleShape& LogBox::GetBody()
{
	return *m_body;
}

pl::Text& priv::LogBox::GetText()
{
	return *m_text;
}

void LogBox::AddMessage(std::string message)
{
	s_incomingMessages.push_back(message);
	m_text->AddString(message);
}
