#include "PhysicsSystem.hpp"
#include "Core/Engine.hpp"

PhysicsSystem::PhysicsSystem(Engine * engine)
  : mEngine(engine)
{
}

bool PhysicsSystem::Init()
{
  return true;
}

void PhysicsSystem::Update(float dt)
{
}

void PhysicsSystem::Deinit()
{
}
