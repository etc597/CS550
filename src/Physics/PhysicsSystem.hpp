#ifndef Physics_PhysicsSystem_hpp
#define Physics_PhysicsSystem_hpp

#include <unordered_map>
#include "Physics/RigidBody.hpp"

class Engine;

class PhysicsSystem
{
public:
  PhysicsSystem(Engine * engine);
  RigidBody* CreateRigidBody(Object* object, const RigidBodyData& data);
  bool DeleteRigidBody(Object* object);
  bool Init();
  void Update(float dt);
  void Deinit();
private:
  Engine * mEngine;
  std::unordered_map<Object*, RigidBody> mBodies;
};

#endif
