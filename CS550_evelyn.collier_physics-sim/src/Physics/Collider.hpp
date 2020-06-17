#ifndef Physics_Collider_hpp
#define Physics_Collider_hpp

#include "Physics/AABB.hpp"
#include "Physics/SupportShape.hpp"

class Engine;
class Object;
namespace ELBA {
  class Model;
}

class Collider
{
public:
  Collider() = default;
  bool Init(Object * object);
  void DebugUpdate();
  void Update(float dt);
  const AABB& GetAABB();
  const std::vector<glm::vec3>& GetLocalVerts();
  const SupportShape& GetCollisionShape() const;

  unsigned mKey;
  Object * mObject;
private:
  Engine * mEngine;
  ELBA::Model * mModel;
  std::vector<glm::vec3> localVerts;

  AABB mAABB;
  SupportShape collisionShape;
};

#endif
