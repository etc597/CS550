#include "SupportShape.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"
#include <cfloat>
#include <algorithm>

SupportShape::SupportShape(Collider * supportModel, RigidBody * rigidBody)
{
}

glm::vec3 SupportShape::GetCenter() const
{
  return GetCenter(obj->GetLocalVerts(), body->GetModelMatrix());
}

glm::vec3 SupportShape::Support(const glm::vec3 & worldDirection) const
{
  return Support(worldDirection, obj->GetLocalVerts(), body->GetModelMatrix());
}

glm::vec3 SupportShape::GetCenter(const std::vector<glm::vec3>& localPoints, const glm::mat4 & loc2World) const
{
  glm::vec3 center;
  glm::vec3 min(FLT_MAX);
  glm::vec3 max(-FLT_MAX);

  for (auto& pt : localPoints)
  {
    for (unsigned i = 0; i < 3; ++i)
    {
      min[i] = std::min(pt[i], min[i]);
      max[i] = std::min(pt[i], max[i]);
    }
  }
  center = 0.5f * (max + min);
  glm::vec4 largeCenter(center, 1);
  largeCenter = loc2World * largeCenter;
  return largeCenter;
}

glm::vec3 SupportShape::Support(const glm::vec3 & worldDirection, const std::vector<glm::vec3>& localPoints, const glm::mat4 & loc2World) const
{
  glm::vec3 result;
  glm::mat4 world2Loc = glm::inverse(loc2World);

  glm::vec4 largeDir = world2Loc * glm::vec4(worldDirection, 0);
  glm::vec3 locDir = largeDir;

  float furthest = -FLT_MAX;
  for (auto& pt : localPoints)
  {
    float dist = glm::dot(locDir, pt);
    if (dist > furthest)
    {
      furthest = dist;
      result = pt;
    }
  }

  glm::vec4 largeResult = glm::vec4(result, 1);
  return loc2World * largeResult;
}

SupportPoint ComputeSupport(const SupportShape& shapeA, const SupportShape& shapeB, const glm::vec3& worldDir)
{
  SupportPoint result;
  result.pointA = shapeA.Support(worldDir);
  result.pointB = shapeB.Support(-worldDir);
  result.csoPoint = result.pointA - result.pointB;
  return result;
}
