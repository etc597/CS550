#ifndef Editor_EditorSystem_hpp
#define Editor_EditorSystem_hpp

struct GLFWwindow;
class Engine;

class EditorSystem
{
public:
  EditorSystem(Engine * engine);
  bool Init(GLFWwindow * window);
  void Update();
  void Deinit();
private:
  Engine * mEngine;
};

#endif