#include "pch.h"
#include "States/Menu.hpp"
#include "Graphics/Graphics.hpp"

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::Update()
{
}

void Menu::Render()
{

	pl::RectangleShape* shape = new pl::RectangleShape({0.0f,0.0f}, {1.5f,1.0f});
	shape->SetColor({ 1.0f,1.0f,0.f,1.f });
	shape->Draw();
	delete shape;

	pl::RectangleShape* shape2 = new pl::RectangleShape({ 1.0f,0.0f }, { 1.5f,0.5f });
	shape2->SetColor({ 0.0f,0.0f,1.f,1.f });
	shape2->Draw();
	delete shape2;

}
