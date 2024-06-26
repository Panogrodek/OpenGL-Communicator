#include "plpch.h"
#include "Graphics/Renderer.hpp"

//IMGUI TODO:Move/not to implement
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/euler_angles.hpp>

#include "Graphics/BatchRenderer.hpp"
#include "Graphics/Font.hpp"
#include "Utilities/InputGuard.hpp"
#include "Utilities/Mouse.hpp"


using namespace pl;
using namespace priv;

void Renderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    inputGuard.UpdateKey(key,action);
}

void Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mouse.m_offset = glm::vec2(xoffset, yoffset);
}

void Renderer::InitGL()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    m_window = glfwCreateWindow(1080, 720, "Client", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    //// Set the required callback functions
    //glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(m_window, key_callback); //this has been a grave mistake to do this
    glfwSetScrollCallback(m_window, scroll_callback);

    //mouse input
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glEnable(GL_BLEND);
    //glEnable(GL_DEPTH_TEST); //this is for 3d or layers
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(renderer.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    //this should not be here
    batchRenderer.Init(10000);
}

void Renderer::Destroy()
{
    batchRenderer.Destroy();
    glfwTerminate();

    //TODO: move /not to implement
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Renderer::Draw(pl::Drawable* object)
{
    batchRenderer.Draw(object);
}

void Renderer::BeginDraw()
{
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents(); //TODO: this NEEDS to be moved into application.hpp of some sort

    glClearColor(0,0,0,255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    // Clear the colorbuffer

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    batchRenderer.SceneBegin();
}

void Renderer::EndDraw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    batchRenderer.Flush();

    // Swap the screen buffers
    glfwSwapBuffers(renderer.GetWindow());
}

glm::vec2 Renderer::GetMousePosition()
{
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    m_mousePos = glm::vec2(float(xpos), float(ypos));
    return m_mousePos;
}

GLFWwindow* const Renderer::GetWindow()
{
    return m_window;
}

Camera* Renderer::GetCamera()
{
    return m_camera;
}

void Renderer::SetCamera(pl::Camera* camera)
{
    m_camera = camera;
}
