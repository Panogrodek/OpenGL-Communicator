#include "pch.h"
#include "States/Menu.hpp"
#include "Graphics/Graphics.hpp"

#include "ui/ClientList.hpp"

Menu::Menu()
{
	pl::fontManager.LoadFont("res/fonts/arial.fnt", "arial");
	m_camera = new pl::OrthographicCamera(0.f, 1600.f, 0.f, 1000.f);

	m_textbox = new Textbox(pl::AABB2D({ 400.f,0 }, { 1600.f,300.f }));
	m_textbox->GetBody().SetColor(glm::vec4{ 50, 168, 162,255 } / 255.f);
	logBox.Init(pl::AABB2D({400.f,300.f}, {1600.f,1000.f}));
	logBox.GetBody().SetColor(glm::vec4{ 165, 194, 192,255 } / 255.f);

	clientList.Init(pl::AABB2D({ 0.f,0.f },{400.f,1000.f}));
	clientList.GetBody().SetColor(glm::vec4(90, 109, 125, 255)/255.f);
}

Menu::~Menu()
{
}

void Menu::Update()
{
	m_textbox->Update();
	logBox.Update();
}

void Menu::Render()
{
	m_camera->Bind();
	logBox.Render();
	clientList.Render();
	m_textbox->Render();
}
