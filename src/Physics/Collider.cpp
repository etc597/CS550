#include "Collider.hpp"

#include "Core/Object.hpp"
#include "Physics/RigidBody.hpp"

#include "Graphics/Model.hpp"
using namespace ELBA;

bool Collider::Init(Object * object)
{
  mObject = object;
  mEngine = mObject->mEngine;
  mModel = mObject->mModel;

  std::vector<glm::vec3> points;
  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    points.push_back(vertex.mPos);
  }
  mAABB.Compute(points);

  return true;
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
