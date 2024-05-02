#include "pch.h"
#include "States/Menu.hpp"
#include "Graphics/Graphics.hpp"

Menu::Menu()
{
	pl::fontManager.LoadFont("res/fonts/arial.fnt", "test");
	m_camera = new pl::OrthographicCamera(-800.f, 800.f, -500.f, 500.f);

	m_text = new pl::Text("test","", { -200.5f,0.f });
	m_text->SetString("this is a test text that i made in opengl");
	m_text->SetTextBounds(pl::AABB2D({ -150.f,-100.f }, { 150.f,100.f }));
}

Menu::~Menu()
{
}

void Menu::Update()
{
}

void Menu::Render()
{
	m_camera->Bind();
	
	for (int i = 0; i < 100; i++) {
		pl::RectangleShape* shape = new pl::RectangleShape({ 0.0f,0.0f }, { 100.5f,100.0f });
		shape->SetColor({ 0.0f + float(i) / 100.f,1.0f,0.f,1.f });
		shape->Draw();
		delete shape;
	}

	//pl::RectangleShape* shape2 = new pl::RectangleShape({ 1.0f,0.0f }, { 1.5f,0.5f });
	//shape2->SetColor({ 0.0f,0.0f,1.f,1.f });
	//shape2->Draw();
	//delete shape2;


	m_text->Draw();
}
