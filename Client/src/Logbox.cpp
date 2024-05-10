#include "pch.h"
#include "ui/Logbox.hpp"
#include "Utilities/Mouse.hpp"

using namespace priv;

void LogBox::Init(pl::AABB2D aabb)
{
	glm::vec2 size = aabb.upperBound - aabb.lowerBound;
	m_body = new pl::RectangleShape(aabb.lowerBound + size / 2.f, size);

	m_text = new pl::Text("arial");
	m_text->SetSize({ 0.5f,0.5f });
	m_text->SetTextBounds(aabb);

	m_ceiling = aabb.upperBound.y;
	m_bottom = size.y;
	logBox.GetText().SetDrawingColor(glm::vec4(26, 92, 56, 255) / 255.f);
	AddMessage("Welcome! Use /h to show list of available commands!\n");
	logBox.GetText().SetDrawingColor(glm::vec4(1.0f));
}

LogBox::~LogBox()
{
	delete m_body;
	delete m_text;
}

void LogBox::Update()
{
	float offset = pl::mouse.GetOffset().y;
	if (offset != 0.f) {
		auto& bounds = m_text->GetAABB2D();
		glm::vec2 size = bounds.upperBound - bounds.lowerBound;
		pl::mouse.BlockOffset();
		m_text->Move(dt* glm::vec2{ 0.f,-1.f * (offset / abs(offset)) * 5500.f}); //-1 because y is inverted, (offset / abs(offset)) -> value is normalized from [-1,1]
		if (m_text->GetPosition().y < m_ceiling) { //the text ceiling
			m_text->SetPosition({ bounds.lowerBound.x, m_ceiling });
		}
		if (m_text->GetPosition().y - 325 > abs(m_text->GetTextBottomPos())) {
			m_text->SetPosition({ bounds.lowerBound.x, 325 + abs(m_text->GetTextBottomPos())});
		}
	}
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
	auto& bounds = m_text->GetAABB2D();
	m_text->SetPosition({ bounds.lowerBound.x, 325 + abs(m_text->GetTextBottomPos()) });
}
