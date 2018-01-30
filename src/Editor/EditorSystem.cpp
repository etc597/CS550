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
  ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);
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
  static bool debug = mEngine->GetGraphicsSystem()->GetDebug();
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

    if (ImGui::CollapsingHeader("Object State (Read Only)")) {
      static bool notQuat = true;
      ImGui::Checkbox("Quat as Euler", &notQuat);
      auto state = obj.mRigidBody->GetState();
      ImGui::Value("Mass", state.mass);
      ImGui::Text("%s: %.3f %.3f %.3f", "Position", state.x[0], state.x[1], state.x[2]);
      if (notQuat) {
        glm::vec3 euler = glm::eulerAngles(state.q);
        ImGui::Text("%s: %.3f %.3f %.3f", "Orientation", euler[0], euler[1], euler[2]);
      }
      else {
        ImGui::Text("%s: %.3f %.3f %.3f %.3f", "Orientation", state.q[0], state.q[1], state.q[2], state.q[3]);

      }
      ImGui::Text("%s: %.3f %.3f %.3f", "Linear Momentum", state.P[0], state.P[1], state.P[2]);
      ImGui::Text("%s: %.3f %.3f %.3f", "Angular Momentum", state.L[0], state.L[1], state.L[2]);
    }

    if (ImGui::CollapsingHeader("Object modification")) {
      static bool showVector = true;
      ImGui::Checkbox("Show Vectors", &showVector);

      static float f[3] = { 0 };
      static float p[3];
      static bool edited = false;
      if (lastItem != item || !edited) {
        for (unsigned i = 0; i < 3; ++i) {
          p[i] = obj.mRigidBody->GetPos()[i];
        }
      }

      // Force
      ImGui::InputFloat3("Force", f);
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
      glm::vec3 lineDir = glm::vec3(0);
      float length = glm::length(force);
      if (length != 0) {
        lineDir = glm::normalize(force);
      }

      float s = 4 * 1.0f / (1 + std::exp((-length + 10000) / 10000));

      if (showVector) {
        mEngine->GetGraphicsSystem()->DebugDrawLine(pos, pos + s * lineDir);
      }

      if (ImGui::Button("Apply Force")) {
        obj.mRigidBody->ApplyForce(force, pos);
        edited = false;
      }

      // Torque
      static float t[3];
      ImGui::InputFloat3("Torque", t);

      glm::vec3 torque = glm::vec3(t[0], t[1], t[2]);
      glm::vec3 objPos = obj.mRigidBody->GetPos();

      length = glm::length(objPos);
      if (length != 0) {
        lineDir = glm::normalize(objPos);
      }

      s = 5 * 1.0f / (1 + std::exp((-length + 10000) / 10000));

      if (showVector) {
        mEngine->GetGraphicsSystem()->DebugDrawLine(objPos, objPos + s * lineDir);
      }
      
      if (ImGui::Button("Apply Torque")) {
        obj.mRigidBody->ApplyTorque(torque);
      }
    }
  }

  lastItem = item;
}
