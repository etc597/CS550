#ifndef Core_Object_hpp
#define Core_Object_hpp

#include <string>
#include <glm/glm.hpp>

namespace ELBA {
  class Model;
}
class RigidBody;
struct RigidBodyData;
class Engine;
class Collider;

struct ObjectData
{
  std::string mName;
  std::string mModelName;
  glm::vec3 mColor;
};

class Object
{
public:
  Object(Engine * engine);
  bool Init(const ObjectData& obj, const RigidBodyData& data);
  void Deinit();
  glm::mat4 GetModelMatrix();
  Engine * mEngine;
  ELBA::Model *  mModel;
  RigidBody * mRigidBody;
  Collider * mCollider;
  std::string mName;
  glm::vec3 mColor;
};

#endif