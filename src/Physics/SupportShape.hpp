#pragma once
#include <glm/glm.hpp>
#include <vector>

class Collider;
class RigidBody;

struct SupportPoint
{
  glm::vec3 csoPoint;
  glm::vec3 pointA;
  glm::vec3 pointB;
};

class SupportShape
{
public:
  SupportShape(Collider* supportModel, RigidBody* rigidBody);
  glm::vec3 GetCenter() const;
  glm::vec3 Support(const glm::vec3& worldDirection) const;

private:
  glm::vec3 GetCenter(const std::vector<glm::vec3>& localPoints, const glm::mat4& loc2World) const;
  glm::vec3 Support(const glm::vec3& worldDirection, const std::vector<glm::vec3>& localPoints, const glm::mat4& loc2World) const;
  Collider* obj;
  RigidBody* body;
  glm::vec3 center;
};

SupportPoint ComputeSupport(const SupportShape& shapeA, const SupportShape& shapeB, const glm::vec3& worldDir);
