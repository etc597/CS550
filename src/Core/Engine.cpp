#include "Engine.hpp"

Engine::Engine(int argc, char * argv[])
{
}

Engine::~Engine()
{
}

void Engine::Run()
{
  while (ShouldRun()) 
  {
    Update();
  }
}

void Engine::Update()
{
}

bool Engine::ShouldRun()
{
  return false;
}
