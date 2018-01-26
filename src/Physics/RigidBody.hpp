#ifndef Physics_RigidBody_hpp
#define Physics_RigidBody_hpp

#include <vector>

#include <glm/glm.hpp>

class Object;

struct PhysicsData
{
  PhysicsData();
  PhysicsData(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, glm::vec3 impulse);
  glm::vec3 mPosition;
  glm::vec3 mVelocity;
  glm::vec3 mAcceleration;
};

struct RigidBodyData
{
  PhysicsData mLinear;
  PhysicsData mAngular;
  float mMass = 1.0f;
};

struct AppliedForce
{
  glm::vec3 mPosition;
  glm::vec3 mForce;
};

class RigidBody
{
public:
  RigidBody();
  void Init(Object * object, RigidBodyData data = RigidBodyData());
  void Update(float dt);
private:
  glm::quat qDotDot(glm::quat q, glm::quat qDot, glm::vec3 torque);
  PhysicsData mLinear;
  PhysicsData mAngular;
  glm::mat3 mInertiaTensor;
  std::vector<AppliedForce> mForces;
  float mMass;
};

#endif
