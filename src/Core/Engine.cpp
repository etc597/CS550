#include "Engine.hpp"
#include "Editor/EditorSystem.hpp"

Engine::Engine(int argc, char * argv[])
  : mGraphics(this)
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
}

void Engine::Update()
{
  mGraphics.Update();
}

bool Engine::ShouldQuit()
{
  return mShouldQuit;
}
