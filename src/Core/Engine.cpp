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
