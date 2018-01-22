#ifndef Core_Engine_hpp
#define Core_Engine_hpp

#include "Graphics/GraphicsSystem.hpp"

class Engine
{
public:
  Engine(int argc, char* argv[]);
  ~Engine();
  bool Init();
  void Run();
  void Quit();
  void Deinit();
private:
  void Update();
  bool ShouldQuit();

  GraphicsSystem mGraphics;
  bool mShouldQuit;
};

#endif
