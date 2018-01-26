#ifndef Graphics_GraphicsSystem_hpp
#define Graphics_GraphicsSystem_hpp

#include <vector>
#include "Graphics/Shader.hpp"
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
private:
  void ProcessInput(GLFWwindow * window);

  Engine * mEngine;
  EditorSystem * mEditor;
  GLFWwindow * mWindow;
  int mScreenWidth;
  int mScreenHeight;
  float mLastFrame;
  std::vector<Shader> mShaders;
  fs::path mShaderPath;
};

#endif
