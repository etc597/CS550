#ifndef Core_Engine_hpp
#define Core_Engine_hpp

#include "Core/EventHandler.hpp"

class Engine : public EventHandler
{
public:
  Engine(int argc, char* argv[]);
  ~Engine();
  void Run();
private:
  void Update();
  bool ShouldRun();
};

#endif
