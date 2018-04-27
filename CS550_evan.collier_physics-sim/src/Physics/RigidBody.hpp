#ifndef Physics_RigidBody_hpp
#define Physics_RigidBody_hpp

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Engine;
class Object;
namespace ELBA {
  class Model;
}

struct AppliedForce
{
  AppliedForce(glm::vec3 f, glm::vec3 p)
    : force(f)
    , pos(p) { };
  glm::vec3 force;
  glm::vec3 pos;
};

struct RigidBodyData
{
  RigidBodyData() 
  : x(0)
  , q()
  , P(0)
  , L(0)
  , mass(0.01f)
  , force(0)
  , torque(0) {

  }
  glm::vec3 x;
  glm::quat q;
  glm::vec3 P;
  glm::vec3 L;
  float mass;
  glm::vec3 force;
  glm::vec3 torque;
};

class RigidBody
{
public:
  RigidBody();
  RigidBody(const RigidBodyData& data);
  bool Init(Object * object);
  void DebugUpdate();
  void Update(float dt);
  void ApplyForce(glm::vec3 force, glm::vec3 pos);
  void ApplyForce(glm::vec3 force);
  void ApplyTorque(glm::vec3 torque);
  void ApplyLinearImpulse(glm::vec3 impulse);
  void ApplyAngularImpulse(glm::vec3 impulse);
  void SetState(const RigidBodyData& data);
  RigidBodyData GetState();
  glm::mat4 GetModelMatrix();
  glm::vec3 GetPos();
  glm::mat4 GetDeltaMatrix();
  float GetMassInverse();
  glm::mat3 GetInertiaTensorInverse();
  glm::vec3 GetLinearVelocity();
  glm::vec3 GetAngularVelocity();
  glm::vec3 GetForces();
  glm::vec3 GetTorques();
  glm::vec3 GetLinearImpulse();
  glm::vec3 GetAngularImpulse();
private:
  Object * mObject;
  Engine * mEngine;
  ELBA::Model * mModel;

  std::vector<AppliedForce> mAppliedForces;
  std::vector<glm::vec3> mAppliedTorques;

  glm::vec3 cm;
  glm::mat3 IbodyInv;
  glm::mat4 lastModel;

  glm::vec3 x; // position
  glm::quat q; // orientation
  glm::vec3 P; // Linear Momentum
  glm::vec3 L; // Angular Momentum

  glm::vec3 v;
  glm::vec3 w;
  glm::vec3 a;
  glm::vec3 wp;

  glm::vec3 force;
  glm::vec3 torque;

  glm::vec3 linearImpulse;
  glm::vec3 angularImpulse;

  float mass;

  glm::vec3 deltaX;
  glm::quat deltaQ;
};

#endif
