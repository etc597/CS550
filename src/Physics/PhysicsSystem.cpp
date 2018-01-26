#include "PhysicsSystem.hpp"
#include "Core/Engine.hpp"

PhysicsSystem::PhysicsSystem(Engine * engine)
  : mEngine(engine)
{
}

RigidBody * PhysicsSystem::CreateRigidBody(Object * object, RigidBodyData & data)
{
  auto pair = mBodies.insert(std::make_pair(object, RigidBody(data))); // returns iter, bool pair
  return &pair.first->second;
}

bool PhysicsSystem::DeleteRigidBody(Object * object)
{
  auto it = mBodies.find(object);
  if (it == mBodies.end()) {
    return false;
  }
  mBodies.erase(it);
  return true;
}

bool PhysicsSystem::Init()
{
  return true;
}

void PhysicsSystem::Update(float dt)
{
  for (auto& body : mBodies) {
    body.second.Update(dt);
  }
}

void PhysicsSystem::Deinit()
{
  mBodies.clear();
}
