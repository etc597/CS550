#include "PhysicsSystem.hpp"
#include "Core/Engine.hpp"

PhysicsSystem::PhysicsSystem(Engine * engine)
  : mEngine(engine)
{
}

RigidBody* PhysicsSystem::CreateRigidBody(Object* object, const RigidBodyData& data)
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

Collider * PhysicsSystem::CreateCollider(Object * object)
{
  auto pair = mColliders.insert(std::make_pair(object, Collider()));
  auto col = &pair.first->second;

  mBroadPhase.InsertData(col->mKey, col);

  return col;
}

bool PhysicsSystem::DeleteCollider(Object * object)
{
  auto it = mColliders.find(object);
  if (it == mColliders.end()) {
    return false;
  }
  auto col = &it->second;
  mBroadPhase.RemoveData(col->mKey);
  mColliders.erase(it);
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

  for (auto& col : mColliders) {
    mBroadPhase.UpdateData(col.second.mKey, &col.second);
  }

  mBroadPhase.SelfQuery(mResults);
}

void PhysicsSystem::Deinit()
{
  mBodies.clear();
}
