#pragma once
#include "StateMachine.hpp"

class Menu : public State {
public:
	Menu();
	virtual ~Menu();

	virtual void Update() override;
	virtual void Render() override;
private:
};