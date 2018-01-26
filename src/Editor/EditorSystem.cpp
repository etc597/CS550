#include "EditorSystem.hpp"

#include "Core/Engine.hpp"

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

  bool open = true;
  //ImGui::ShowTestWindow(&open);
  ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::Begin("Physics Sim", &open);

  RewindPausePlayBar();

  DebugDrawCheckbox();

  ObjectEditor();

  ImGui::End();

  ImGui::Render();
}

void EditorSystem::Deinit()
{
}

void EditorSystem::RewindPausePlayBar()
{
  if (ImGui::Button("<<")) {
    mEngine->Rewind();
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Rewind");
  }
  ImGui::SameLine();
  if (ImGui::Button("||")) {
    mEngine->Pause();
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Pause");
  }
  ImGui::SameLine();
  if (ImGui::Button(" >")) {
    mEngine->Play();
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Play");
  }
}

void EditorSystem::DebugDrawCheckbox()
{
  static bool debug = false;
  ImGui::Checkbox("Debug Draw", &debug);

  mEngine->GetGraphicsSystem()->SetDebug(debug);
}

void EditorSystem::ObjectEditor()
{
  //static int item = -1;
  //ImGui::Combo("Objects", item, )
}
