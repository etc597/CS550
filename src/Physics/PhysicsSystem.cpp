#include "PhysicsSystem.hpp"
#include "Core/Engine.hpp"
#include "Graphics/GraphicsSystem.hpp"
#include "Physics/AABB.hpp"
#include "Physics/ContactResolver.hpp"
#include "Physics/EPA.hpp"
#include "Physics/GJK.hpp"

#include <iostream>

PhysicsSystem::PhysicsSystem(Engine * engine)
  : mEngine(engine)
  , mDrawBPhase(false)
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
  if (dt < 0)
  {
    for (auto& col : mColliders) {
      col.second.Update(0);
      mBroadPhase.UpdateData(col.second.mKey, &col.second);
    }
  }
  const float fixedStep = (1.0f / 120.0f);
  while(dt > 0)
  {
    InternalUpdate(fixedStep);
    dt -= fixedStep;
  }

  DebugDrawBroadPhase();
}

void PhysicsSystem::Deinit()
{
  mBodies.clear();
}

void PhysicsSystem::InternalUpdate(float dt)
{
  auto jobSystem = mEngine->GetJobSystem();
  // TODO(Evan): multi thread body and col updates (can do both per job)
  std::vector<JobHandle> handles;
  handles.reserve(mBodies.size());
  for (auto& body : mBodies) {
    auto handle = jobSystem->QueueJobThisThread([&body, dt](JobHandle& handle)->void* {
      body.second.Update(dt);
      body.first->mCollider->Update(dt);
      return nullptr;
    });
    handles.push_back(handle);
  }

  while (!handles.empty()) {
    jobSystem->WaitThisThread(handles.back());
    handles.pop_back();
  }

  for (auto& col : mColliders) {
    mBroadPhase.UpdateData(col.second.mKey, &col.second);
  }

  mBroadPhase.SelfQuery(mResults);
  handles.reserve(mResults.size());
  std::vector<Contact> manifold;
  manifold.reserve(mResults.size());
  std::mutex manifoldLock;
  // TODO(Evan): multi-thread collision detection and contact manifold stuff
  // don't forget to make adding to the manifold thread safe!
  for (auto& result : mResults)
  {
    auto handle = jobSystem->QueueJobThisThread([&result, &manifold, &manifoldLock](JobHandle& handle)->void* {
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
        std::lock_guard<std::mutex> guard(manifoldLock);
        manifold.push_back(contactData);
      }
      return nullptr;
    });
    handles.push_back(handle);
  }

  while (!handles.empty()) {
    jobSystem->WaitThisThread(handles.back());
    handles.pop_back();
  }

  mManifold = manifold;

  // resolve contacts (probably thread this too once you do that)
  ResolveContacts(dt);
}

void PhysicsSystem::ResolveContacts(float dt)
{
  for (auto& contact : mManifold)
  {
    auto pair = std::make_pair(contact.bodies[0], contact.bodies[1]);
    auto it = std::find_if(mPreviousCollisions.begin(), mPreviousCollisions.end(), [contact](const Contact& it)->bool {
      if (contact.bodies[0] == it.bodies[0] && contact.bodies[1] == it.bodies[1])
      {
        return true;
      }

      if (contact.bodies[0] == it.bodies[1] && contact.bodies[1] == it.bodies[0])
      {
        return true;
      }
      return false;
    });

    if (it != mPreviousCollisions.end())
    {
      contact.lambda = it->lambda;
    }

    ContactResolver::ResolveContact(contact, dt);
  }

  mPreviousCollisions = mManifold;
}

void PhysicsSystem::DebugDrawBroadPhase()
{
  if (!mDrawBPhase)
  {
    return;
  }

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

void PhysicsSystem::DebugDrawBroadPhase(bool setting)
{
  mDrawBPhase = setting;
}
