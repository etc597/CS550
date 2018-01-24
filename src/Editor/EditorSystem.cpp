#include "EditorSystem.hpp"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

EditorSystem::EditorSystem(Engine * engine)
  : mEngine(engine)
{
}

bool EditorSystem::Init(GLFWwindow * window)
{
  return ImGui_ImplGlfwGL3_Init(window, true);
}

void EditorSystem::Update()
{
  ImGui_ImplGlfwGL3_NewFrame();

  ImGui::Render();
}

void EditorSystem::Deinit()
{
}
