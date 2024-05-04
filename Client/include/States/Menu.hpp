#pragma once
#include "StateMachine.hpp"
#include "Graphics/Graphics.hpp"
#include "ui/Textbox.hpp"
#include "ui/Logbox.hpp"

class Menu : public State {
public:
	Menu();
	virtual ~Menu();

	virtual void Update() override;
	virtual void Render() override;
private:
	pl::Camera* m_camera;
	Textbox* m_textbox;
};