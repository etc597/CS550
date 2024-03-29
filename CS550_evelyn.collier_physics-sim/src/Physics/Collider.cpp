#include "Collider.hpp"

#include "Core/Engine.hpp"
#include "Core/Object.hpp"
#include "Graphics/GraphicsSystem.hpp"
#include "Physics/RigidBody.hpp"

#include "Graphics/Model.hpp"
using namespace ELBA;

bool Collider::Init(Object * object)
{
  localVerts.clear();
  mObject = object;
  mEngine = mObject->mEngine;
  mModel = mObject->mModel;

  std::vector<glm::vec3> points;
  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    localVerts.push_back(vertex.mPos);
    glm::vec4 affinePt(vertex.mPos, 1);
    points.push_back(glm::vec3(affinePt.x, affinePt.y, affinePt.z));
  }
  mAABB.Compute(points);

  collisionShape = SupportShape(this, mObject->mRigidBody);

  return true;
}

void Collider::DebugUpdate()
{
 // mEngine->GetGraphicsSystem()->DebugDrawAABB(mAABB, glm::vec3(0.5f, 1.0f, 0));
}

void Collider::Update(float dt)
{
  mAABB.Transform(mObject->mRigidBody->GetModelMatrix());
}

const AABB & Collider::GetAABB()
{
  return mAABB;
}

const std::vector<glm::vec3>& Collider::GetLocalVerts()
{
  return localVerts;
}

const SupportShape & Collider::GetCollisionShape() const
{
  return collisionShape;
}
