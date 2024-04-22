#include "pch.h"
#include "States/Menu.hpp"
#include "Graphics/Graphics.hpp"

Menu::Menu()
{
	pl::fontManager.LoadFont("res/fonts/Pixeled.ttf", "test");
}

Menu::~Menu()
{
}

void Menu::Update()
{
}

void Menu::Render()
{
	
	for (int i = 0; i < 100; i++) {
		pl::RectangleShape* shape = new pl::RectangleShape({ 0.0f,0.0f }, { 1.5f,1.0f });
		shape->SetColor({ 0.0f + float(i) / 100.f,1.0f,0.f,1.f });
		shape->Draw();
		delete shape;
	}

	//pl::RectangleShape* shape2 = new pl::RectangleShape({ 1.0f,0.0f }, { 1.5f,0.5f });
	//shape2->SetColor({ 0.0f,0.0f,1.f,1.f });
	//shape2->Draw();
	//delete shape2;

	pl::Text* text = new pl::Text("kuba wita nas", &pl::fontManager.GetFont("test"), { -0.5f,0.f }, glm::vec2{ 0.025f });
	text->Draw();
	delete text;
}
