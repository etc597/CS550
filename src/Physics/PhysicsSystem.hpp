#ifndef Physics_PhysicsSystem_hpp
#define Physics_PhysicsSystem_hpp

#include <vector>
#include "Physics/RigidBody.hpp"

class Engine;

class PhysicsSystem
{
public:
  PhysicsSystem(Engine * engine);
  bool Init();
  void Update(float dt);
  void Deinit();
private:
  Engine * mEngine;
  std::vector<RigidBody> mBodies;
};

#endif
