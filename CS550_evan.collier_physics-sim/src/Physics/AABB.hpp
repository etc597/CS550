#ifndef Physics_AABB_hpp
#define Physics_AABB_hpp

#include "glm/glm.hpp"
#include <vector>

class AABB
{
public:
  AABB() = default;
  AABB(const glm::vec3& min, const glm::vec3& max);

  float GetVolume() const;
  float GetSurfaceArea() const;
  void Compute(const std::vector<glm::vec3>& points);
  bool Contains(const AABB& aabb) const;
  bool Collides(const AABB& aabb) const;
  AABB operator+(const glm::vec3& point) const;
  AABB& operator+=(const glm::vec3& point);
  AABB operator+(const AABB& lhs) const;
  AABB& operator+=(const AABB& lhs);
  void Transform(const glm::mat4& transform);
  void Pad(float aPad);

  glm::vec3 GetMin() const;
  glm::vec3 GetMax() const;
  glm::vec3 GetCenter() const;
  glm::vec3 GetHalfExtents() const;

private:
  glm::vec3 mOrigMin;
  glm::vec3 mOrigMax;
  glm::vec3 mMin;
  glm::vec3 mMax;
};

#endif
