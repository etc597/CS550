#ifndef Core_Engine_hpp
#define Core_Engine_hpp

class Engine
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
