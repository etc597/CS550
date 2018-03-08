#include "PhysicsSystem.hpp"
#include "Core/Engine.hpp"
#include "Graphics/GraphicsSystem.hpp"
#include "Physics/AABB.hpp"

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
    col.second.Update(dt);
    mBroadPhase.UpdateData(col.second.mKey, &col.second);
  }

  mBroadPhase.SelfQuery(mResults);

  auto drawfn = [this](const AABB& aabb, unsigned key) {
    auto it = std::find_if(mResults.begin(), mResults.end(), [key](const QueryResult& pair) {
      if (pair.mKeyPair.first == key || pair.mKeyPair.second == key) {
        return true;
      }
      return false;
    });
    if (it != mResults.end()) {
      mEngine->GetGraphicsSystem()->DebugDrawAABB(aabb, glm::vec3(1, 0, 0));
    }
    else {
      mEngine->GetGraphicsSystem()->DebugDrawAABB(aabb);
    }
  };

  for (auto& result : mResults)
  {
    // GJK(result.mDataPair);
  }

  mBroadPhase.DebugDraw(drawfn);
}

void PhysicsSystem::Deinit()
{
  mBodies.clear();
}
