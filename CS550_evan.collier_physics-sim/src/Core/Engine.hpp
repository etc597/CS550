#ifndef Core_Engine_hpp
#define Core_Engine_hpp

#include "Graphics/GraphicsSystem.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Core/Object.hpp"
#include "Core/Threading/JobSystem.hpp"

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
  void Play();
  void Pause();
  void Rewind();

  bool CreateObject(const ObjectData& obj, const RigidBodyData& data);
  void DeleteObject(const std::string& objName);

  const std::vector<Object>& GetObjects() const;
  std::vector<Object>& GetObjects();

  GraphicsSystem* GetGraphicsSystem();
  PhysicsSystem* GetPhysicsSystem();
  JobSystem* GetJobSystem();
private:
  enum class State
  {
    Pause,
    Play,
    Rewind,
    Count
  };

  void Update();
  void Cache();
  void RewindUpdate();
  bool ShouldQuit();

  State mState;
  GraphicsSystem mGraphics;
  PhysicsSystem mPhysics;
  JobSystem mJobSystem;
  std::vector<Object> mObjects;
  std::stack<FrameData> mFrameData;
  bool mShouldQuit;
};

#endif
