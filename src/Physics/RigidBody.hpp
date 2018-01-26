#ifndef Physics_RigidBody_hpp
#define Physics_RigidBody_hpp

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


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
  void Update(float dt);
  void ApplyForce(glm::vec3 force, glm::vec3 pos);
  void ApplyForce(glm::vec3 force);
  void ApplyTorque(glm::vec3 torque);
  void SetState(const RigidBodyData& data);
  RigidBodyData GetState();
  glm::mat4 GetModelMatrix();
private:
  ELBA::Model * mModel;

  std::vector<AppliedForce> mAppliedForces;
  std::vector<glm::vec3> mAppliedTorques;

  glm::vec3 cm;
  glm::mat3 Ibody;

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

  float mass;
};

#endif
