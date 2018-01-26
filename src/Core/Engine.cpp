#include "Engine.hpp"
#include "Editor/EditorSystem.hpp"

Engine::Engine(int argc, char * argv[])
  : mGraphics(this)
  , mPhysics(this)
  , mShouldQuit(false)
{
}

Engine::~Engine()
{
}

bool Engine::Init()
{
  if (!mGraphics.Init(new EditorSystem(this))) {
    return false;
  }

  if (!mPhysics.Init()) {
    return false;
  }

  return true;
}

void Engine::Run()
{
  while (!ShouldQuit()) 
  {
    Update();
  }
}

void Engine::Quit()
{
  mShouldQuit = true;
}

void Engine::Deinit()
{
  mGraphics.Deinit();
  mPhysics.Deinit();
}

void Engine::CreateObject(const ObjectData & obj, const RigidBodyData & data)
{
  mObjects.push_back(Object(this));
  mObjects.back().Init(obj, data);
}

void Engine::DeleteObject(const std::string & objName)
{
  auto it = std::find_if(mObjects.begin(), mObjects.end(), [objName](const Object& obj) {
    if (obj.mName != objName) {
      return false;
    }
    return true;
  });
  if (it != mObjects.end()) {
    it->Deinit();
    mObjects.erase(it);
  }
}

const std::vector<Object>& Engine::GetObjects()
{
  return mObjects;
}

GraphicsSystem * Engine::GetGraphicsSystem()
{
  return &mGraphics;
}

PhysicsSystem * Engine::GetPhysicsSystem()
{
  return &mPhysics;
}

void Engine::Update()
{
  float dt = mGraphics.Update();
  mPhysics.Update(dt);
}

bool Engine::ShouldQuit()
{
  return mShouldQuit;
}
