#pragma once

#ifndef Physics_PhysicsSystem_hpp
#define Physics_PhysicsSystem_hpp

#include <unordered_map>
#include <vector>

#include "Physics/AABBTree.hpp"
#include "Physics/Collider.hpp"
#include "Physics/ContactManifold.hpp"
#include "Physics/RigidBody.hpp"

class Engine;

class PhysicsSystem
{
public:
  PhysicsSystem(Engine * engine);
  RigidBody* CreateRigidBody(Object* object, const RigidBodyData& data);
  bool DeleteRigidBody(Object* object);
  Collider* CreateCollider(Object* object);
  bool DeleteCollider(Object* object);
  bool Init();
  void Update(float dt);
  void Deinit();

private:
  void InternalUpdate(float dt);
  void ResolveContacts();
  void DebugDrawBroadPhase();

  Engine * mEngine;
  AABBTree mBroadPhase;
  QueryResults mResults;
  std::unordered_map<Object*, RigidBody> mBodies;
  std::unordered_map<Object*, Collider> mColliders;
  std::vector<Contact> mManifold;
};

#endif
