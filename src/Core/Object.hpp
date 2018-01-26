#ifndef Core_Object_hpp
#define Core_Object_hpp

#include <string>

namespace ELBA {
  class Model;
}
class RigidBody;
struct RigidBodyData;
class Engine;

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
  Engine * mEngine;
  ELBA::Model *  mModel;
  RigidBody * mRigidBody;
  std::string mName;
  glm::vec3 mColor;
};

#endif