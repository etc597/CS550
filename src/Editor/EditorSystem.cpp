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
  auto& objects = mEngine->GetObjects();
  static int item = 0;
  static int lastItem = -1;
  if (objects.empty()) {
    item = -1;
  }

  ImGui::Combo("Objects", &item, [](void * data, int idx, const char **out_text)->bool {
    auto& objects = *static_cast<std::vector<Object>*>(data);
    *out_text = objects[idx].mName.c_str();
    return true;
  }, &objects, (int)objects.size());

  if (item >= 0) {
    auto& obj = objects[item];
    float col[3];
    for (auto i = 0; i < 3; ++i) {
      col[i] = obj.mColor[i];
    }
    ImGui::ColorEdit3("Color", col);
    for (auto i = 0; i < 3; ++i) {
      obj.mColor[i] = col[i];
    }
    ImGui::Text("Object modification");
    static float f[3] = { 0 };
    static float p[3];
    static bool edited = false;
    if (lastItem != item || !edited) {
      for (unsigned i = 0; i < 3; ++i) {
        p[i] = obj.mRigidBody->GetPos()[i];
      }
    }
    ImGui::DragFloat3("Force", f, 10.0f);
    ImGui::DragFloat3("WorldPos", p, 0.01f);
    if (!edited) {
      for (unsigned i = 0; i < 3; ++i) {
        if (p[i] != obj.mRigidBody->GetPos()[i]) {
          edited = true;
        }
      }
    }

    glm::vec3 force = glm::vec3(f[0], f[1], f[2]);
    glm::vec3 pos = glm::vec3(p[0], p[1], p[2]);

    mEngine->GetGraphicsSystem()->DebugDrawLine(pos, pos + force);

    if (ImGui::Button("Apply")) {
      obj.mRigidBody->ApplyForce(force, pos);
      edited = false;
    }
  }

  lastItem = item;
}
