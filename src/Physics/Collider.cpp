#include "Collider.hpp"

#include "Core/Engine.hpp"
#include "Core/Object.hpp"
#include "Graphics/GraphicsSystem.hpp"
#include "Physics/RigidBody.hpp"

#include "Graphics/Model.hpp"
using namespace ELBA;

bool Collider::Init(Object * object)
{
  mObject = object;
  mEngine = mObject->mEngine;
  mModel = mObject->mModel;

  std::vector<glm::vec3> points;
  auto initPos = mObject->mRigidBody->GetDeltaMatrix();
  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    glm::vec4 affinePt(vertex.mPos, 1);
    affinePt = initPos * affinePt;
    points.push_back(glm::vec3(affinePt.x, affinePt.y, affinePt.z));
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
