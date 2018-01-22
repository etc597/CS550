#ifndef Graphics_GraphicsSystem_hpp
#define Graphics_GraphicsSystem_hpp

struct GLFWwindow;
class Engine;

class GraphicsSystem
{
public:
  GraphicsSystem(Engine * engine);
  bool Init();
  float Update();
  void Deinit();

  void FramebufferSizeCallback(GLFWwindow * window, int width, int height);
private:
  void ProcessInput(GLFWwindow * window);

  Engine * mEngine;
  GLFWwindow * mWindow;
  int mScreenWidth;
  int mScreenHeight;
  float mLastFrame;
};

#endif
