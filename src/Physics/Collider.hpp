#ifndef Physics_Collider_hpp
#define Physics_Collider_hpp

#include "Physics/AABB.hpp"

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

  unsigned mKey;
private:
  Object * mObject;
  Engine * mEngine;
  ELBA::Model * mModel;

  AABB mAABB;
};

#endif
