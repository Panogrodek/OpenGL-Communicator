#include "pch.h"
#include "States/Menu.hpp"
#include "Graphics/Graphics.hpp"

Menu::Menu()
{
	pl::fontManager.LoadFont("res/fonts/arial.fnt", "arial");
	m_camera = new pl::OrthographicCamera(0.f, 1600.f, 0.f, 1000.f);

	m_textbox = new Textbox(pl::AABB2D({ 400.f,0 }, { 1600.f,300.f }));
	m_textbox->GetBody().SetColor(glm::vec4{ 50, 168, 162,255 } / 255.f);
	m_logbox = new LogBox(pl::AABB2D({400.f,300.f}, {1600.f,1000.f}));
	m_logbox->GetBody().SetColor(glm::vec4{ 165, 194, 192,255 } / 255.f);
}

Menu::~Menu()
{
}

void Menu::Update()
{
	m_textbox->Update();
	m_logbox->Update();
}

void Menu::Render()
{
	m_camera->Bind();
	m_textbox->Render();
	m_logbox->Render();
}
