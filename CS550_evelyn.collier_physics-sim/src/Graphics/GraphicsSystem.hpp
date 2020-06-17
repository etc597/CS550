#ifndef Graphics_GraphicsSystem_hpp
#define Graphics_GraphicsSystem_hpp

#include <vector>
#include <unordered_map>
#include "Graphics/Shader.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "fs.hpp"

struct GLFWwindow;
class Engine;
class Object;
class EditorSystem;
class AABB;

using namespace ELBA;

class GraphicsSystem
{
public:
  GraphicsSystem(Engine * engine);
  bool Init(EditorSystem * editorSystem);
  float Update();
  void Deinit();

  void FramebufferSizeCallback(GLFWwindow * window, int width, int height);
  void MouseCallback(GLFWwindow * window, double xpos, double ypos);
  void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
  Model* GetModel(const std::string& name);

  bool GetDebug();
  void SetDebug(bool val);
  void DebugDrawLine(const glm::vec3& p1, const glm::vec3& p2, glm::vec3 color = glm::vec3(1, 1, 1), bool force = false);
  void DebugDrawAABB(const AABB& aabb, glm::vec3 color = glm::vec3(1,1,1), bool force = false);
private:
  bool DebugInit();
  void ProcessInput(GLFWwindow * window, float dt);
  void Draw();
  void DebugDraw();

  Engine * mEngine;
  EditorSystem * mEditor;
  GLFWwindow * mWindow;
  int mScreenWidth;
  int mScreenHeight;
  float mLastFrame;
  std::vector<Shader> mShaders;
  fs::path mAssetPath;
  Camera mCamera;
  std::unordered_map<std::string, Model> mModels;
  bool mDebug;
  std::vector<std::pair<glm::vec3, std::vector<glm::vec3>>> mDebugLines;
  unsigned int mDebugVAO;
  unsigned int mDebugVBO;
};

#endif
