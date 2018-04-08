#pragma once
#include <glm/glm.hpp>

class Collider;
class RigidBody;

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
