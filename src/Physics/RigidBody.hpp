#ifndef Physics_RigidBody_hpp
#define Physics_RigidBody_hpp

#include <vector>

#include <glm/glm.hpp>

class Object;
class Model;

class RigidBody
{
public:
  RigidBody();
  void Init(Object * object);
  void Update(float dt);
private:
  Model * mModel;
};

#endif
