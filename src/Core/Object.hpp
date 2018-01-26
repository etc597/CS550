#ifndef Core_Object_hpp
#define Core_Object_hpp

#include <string>

namespace ELBA {
  class Model;
}
class RigidBody;
struct RigidBodyData;

struct ObjectData
{
  std::string mName;
  std::string mModelName;
};

class Object
{
public:
  Object();
  bool Init(const RigidBodyData& data);

  std::string mName;
  ELBA::Model *  mModel;
  RigidBody * mRigidBody;
};

#endif