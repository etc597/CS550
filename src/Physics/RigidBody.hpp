#ifndef Physics_RigidBody_hpp
#define Physics_RigidBody_hpp

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


class Object;
namespace ELBA {
  class Model;
}

class RigidBody
{
public:
  RigidBody();
  void Init(Object * object);
  void Update(float dt);
private:
  ELBA::Model * mModel;
  glm::vec3 cm;
  glm::mat3 Ibody;
  float mass;

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
};

#endif
