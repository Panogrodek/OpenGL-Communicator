#include "pch.h"
#include "States/StateMachine.hpp"

using namespace priv;

void StateMachine::Update()
{
	if (m_states.empty())
		return;

	if (!m_states.front()->m_running) {
		Pop();
		if (!m_states.empty())
			m_states.front()->m_running = true;
		else
			return;
	}

	m_states.front()->Update();
}

void StateMachine::Render()
{
	if (m_states.empty())
		return;

	m_states.front()->Render();
}

void StateMachine::Push(State* state)
{
	if (m_states.empty())
		state->m_running = true;
	m_states.push(state);
}

void StateMachine::Pop()
{
	delete m_states.front();
	m_states.pop();
}

void StateMachine::Clear()
{
	while (!m_states.empty())
		Pop();

	std::queue<State*> empty;
	std::swap(m_states, empty);
}

State* StateMachine::GetTop()
{
	return m_states.front();
}
