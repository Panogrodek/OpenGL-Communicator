#include "pch.h"
#include "ui/Logbox.hpp"

std::vector<std::string> LogBox::s_messages;

LogBox::LogBox(pl::AABB2D aabb)
{
	glm::vec2 size = aabb.upperBound - aabb.lowerBound;
	m_body = new pl::RectangleShape(aabb.lowerBound + size/2.f,size);

	m_text = new pl::Text("arial");
	m_text->Update();
	m_text->SetString("kuba na tescia");
	m_text->SetTextBounds(aabb);
}

LogBox::~LogBox()
{
	delete m_body;
	delete m_text;
}

void LogBox::Update()
{
	std::string all;
	for (auto& s : s_messages) {
		all += s;
		all += '\n';
	}
	m_text->SetString(all);
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

void LogBox::AddMessage(std::string message)
{
	s_messages.push_back(message);
}
