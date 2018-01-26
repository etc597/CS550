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
class EditorSystem;

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
private:
  void ProcessInput(GLFWwindow * window, float dt);

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
};

#endif
