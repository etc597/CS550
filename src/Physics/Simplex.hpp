#pragma once;

#include <glm/glm.hpp>
#include "Physics/SupportShape.hpp"

class Simplex
{
public:
  Simplex() = default;
  Simplex(const SupportPoint& initPoint, const glm::vec3& initDir);
  bool AddPoint(SupportPoint& pointToAdd);
  void Reduce(const glm::vec3& searchPoint = glm::vec3());
  const glm::vec3& GetClosestPoint() const;
  const glm::vec3& GetSearchDir() const;
  SupportPoint ReconstructPoint() const;
  const SupportPoint& operator[](size_t i) const;

private:
  void ReducePoint(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize);
  void ReduceLine(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize);
  void ReduceTriangle(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize);
  void ReduceTetrahedron(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize);

  SupportPoint ReconstructFromPoint() const;
  SupportPoint ReconstructFromLine() const;
  SupportPoint ReconstructFromTriangle() const;

  glm::vec3 closestPoint;
  glm::vec3 searchDir;
  SupportPoint simplex[4];
  size_t size;
};

