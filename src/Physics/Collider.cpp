#include "Collider.hpp"

#include "Core/Object.hpp"
#include "Physics/RigidBody.hpp"

bool Collider::Init(Object * object)
{
  return false;
}

void Collider::DebugUpdate()
{
  //mEngine->GetGraphicsSystem()->DebugDrawAABB(mAABB);
}

void Collider::Update(float dt)
{
  mAABB.Transform(mObject->mRigidBody->GetDeltaMatrix());
}

const AABB & Collider::GetAABB()
{
  return mAABB;
}
