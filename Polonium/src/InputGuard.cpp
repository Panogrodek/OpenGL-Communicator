#include "plpch.h"
#include "Utilities/InputGuard.hpp"
#include <GLFW/glfw3.h>

using namespace priv;
using namespace pl;

void InputGuard::Init()
{
	for (int c = 32; c < 94; c++) {
		m_keys[c] = KeyCode::NONE;
	}
}

void InputGuard::Lock(int c)
{
	m_keys[c] = KeyCode::NONE;
}

KeyCode InputGuard::GetKeyCode(int c)
{
	return m_keys[c];
}

pl::KeyCode& InputGuard::operator[](int index)
{
	return m_keys[index];
}

void InputGuard::UpdateKey(int key, int action)
{
	if (action == GLFW_PRESS) {
		m_keys[key] = KeyCode::PRESS;
	}
	else if (action == GLFW_RELEASE) {
		m_keys[key] = KeyCode::RELEASE;
	}
	else if (action == GLFW_REPEAT) {
		m_keys[key] = KeyCode::REPEAT;
	}
	else
		m_keys[key] = KeyCode::NONE;
}
