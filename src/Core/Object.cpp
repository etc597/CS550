#include "Core/Object.hpp"
#include "Core/Engine.hpp"
#include <iostream>

Object::Object(Engine * engine)
  : mEngine(engine)
{

}

bool Object::Init(const ObjectData& obj, const RigidBodyData& data)
{
  if (!mEngine) {
    std::cout << "Can't have an object without an engine!" << std::endl;
    return false;
  }
  mName = obj.mName;
  auto graphics = mEngine->GetGraphicsSystem();
  if (!graphics) {
    std::cout << "Can't have an object without graphics!" << std::endl;
    return false;
  }

  mModel = graphics->GetModel(obj.mModelName);
  if (!mModel) {
    std::cout << "Invalid model name, model " << obj.mModelName << "does not exist" << std::endl;
    return false;
  }
}
