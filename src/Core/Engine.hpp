#ifndef Core_Engine_hpp
#define Core_Engine_hpp

#include "Graphics/GraphicsSystem.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Core/Object.hpp"

#include <stack>

struct FrameData
{
  std::unordered_map<Object*, RigidBodyData> mData;
};

class Engine
{
public:
  Engine(int argc, char* argv[]);
  ~Engine();
  bool Init();
  void Run();
  void Quit();
  void Deinit();

  void CreateObject(const ObjectData& obj, const RigidBodyData& data);
  void DeleteObject(const std::string& objName);

  const std::vector<Object>& GetObjects();
  GraphicsSystem* GetGraphicsSystem();
  PhysicsSystem* GetPhysicsSystem();
private:
  void Update();
  bool ShouldQuit();

  GraphicsSystem mGraphics;
  PhysicsSystem mPhysics;
  std::vector<Object> mObjects;
  std::stack<FrameData> mFrameData;
  bool mShouldQuit;
};

#endif
