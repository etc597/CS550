#include "Core/Object.hpp"
#include "Core/Engine.hpp"

#include <iostream>

Object::Object(Engine * engine)
  : mEngine(engine)
  , mModel(nullptr)
  , mRigidBody(nullptr)
  , mCollider(nullptr)
  , mName()
  , mColor()
{

}

bool Object::Init(const ObjectData& obj, const RigidBodyData& data)
{
  mColor = obj.mColor;
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

  auto physics = mEngine->GetPhysicsSystem();
  if (!physics) {
    std::cout << "Can't have an object without physics!" << std::endl;
  }

  mRigidBody = physics->CreateRigidBody(this, data);
  if (!mRigidBody) {
    std::cout << "Couldn't create a rigid body with the given data" << std::endl;
    return false;
  }

  if (!mRigidBody->Init(this)) {
    std::cout << "Failed to init the rigid body" << std::endl;
    return false;
  }

  mCollider = physics->CreateCollider(this);
  if (!mCollider) {
    std::cout << "Failed to create collider" << std::endl;
    return false;
  }

  if (!mCollider->Init(this)) {
    std::cout << "Failed to init the collider" << std::endl;
    return false;
  }

  return true;
}

void Object::Deinit()
{
  mEngine->GetPhysicsSystem()->DeleteRigidBody(this);
  mEngine->GetPhysicsSystem()->DeleteCollider(this);
}

glm::mat4 Object::GetModelMatrix()
{
  return mRigidBody->GetModelMatrix();
}
