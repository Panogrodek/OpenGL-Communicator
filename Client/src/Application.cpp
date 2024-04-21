#include "pch.h"
#include "Application.hpp"

#include "Global.hpp"

#include "Graphics/Renderer.hpp"

#include "States/StateMachine.hpp"
#include "States/Menu.hpp"

Application::Application()
{
    pl::renderer.InitGL();
    stateMachine.Push(new Menu);
}

Application::~Application()
{
    pl::renderer.Destroy();
}

void Application::Run()
{
    while (!glfwWindowShouldClose(pl::renderer.GetWindow()))
    {
        if (dt > 0.25f) //we are setting max delta time value to avoid spiraling into void
            dt = 0.25f;

        Update();
        Render();

        m_timeAccumulator += dt;

        float currentFrame = glfwGetTime();
        dt = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
    }
}

void Application::Update()
{
    stateMachine.Update();
}

void Application::Render()
{
    pl::renderer.BeginDraw();

    stateMachine.Render();

    pl::renderer.EndDraw();
}