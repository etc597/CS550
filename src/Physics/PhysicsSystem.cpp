#include "PhysicsSystem.hpp"
#include "Core/Engine.hpp"
#include "Graphics/GraphicsSystem.hpp"
#include "Physics/AABB.hpp"
#include "Physics/GJK.hpp"
#include "Physics/EPA.hpp"
#include "Physics/ContactManifold.hpp"

#include <iostream>

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
  const float fixedStep = (1.0f / 144.0f);
  while (dt > 0)
  {
    InternalUpdate(fixedStep);
    dt -= fixedStep;
  }
}

void PhysicsSystem::Deinit()
{
  mBodies.clear();
}

void PhysicsSystem::InternalUpdate(float dt)
{
  // TODO(Evan): multi thread body and col updates (can do both per job)
  for (auto& body : mBodies) {
    body.second.Update(dt);
  }

  for (auto& col : mColliders) {
    col.second.Update(dt);
    mBroadPhase.UpdateData(col.second.mKey, &col.second);
  }

  mBroadPhase.SelfQuery(mResults);
  std::vector<Contact> manifold;
  // TODO(Evan): multi-thread collision detection and contact manifold stuff
  // don't forget to make adding to the manifold thread safe!
  for (auto& result : mResults)
  {
    Simplex simplexResult;
    auto shapeA = result.mDataPair.first->GetCollisionShape();
    auto shapeB = result.mDataPair.second->GetCollisionShape();

    auto res = GJK::Intersect(shapeA, shapeB, simplexResult);
    
    // generate contact information
    if (res)
    {
      Polytope polytope(simplexResult);
      EPA::Expand(shapeA, shapeB, polytope);

      // probably need to be doing more than this
      Contact contactData;
      contactData.bodies[0] = result.mDataPair.first->mObject->mRigidBody;
      contactData.bodies[1] = result.mDataPair.second->mObject->mRigidBody;
      CreateContact(polytope, contactData);
      manifold.push_back(contactData);
      mEngine->GetGraphicsSystem()->DebugDrawLine(contactData.point, contactData.point + 0.5f * contactData.normal, glm::vec3(1, 0, 0.0f), true);
      //mEngine->GetGraphicsSystem()->DebugDrawLine(contactData.contacts[0].point, contactData.contacts[0].point + 0.5f * contactData.contacts[0].normal, glm::vec3(1, 0, 0.0f), true);
      //mEngine->GetGraphicsSystem()->DebugDrawLine(contactData.contacts[1].point, contactData.contacts[1].point + 0.5f * contactData.contacts[1].normal, glm::vec3(0, 1, 0.0f), true);
    }
  }

  // resolve contacts (probably thread this too once you do that)
  // ???????
  // look at cato stuff
}

void PhysicsSystem::DebugDrawBroadPhase()
{
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

  mBroadPhase.DebugDraw(drawfn);
}
