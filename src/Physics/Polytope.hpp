#pragma once
#include "Physics/Simplex.hpp"
#include "Physics/SupportShape.hpp"
#include <vector>

struct PolytopeTriangle
{
  size_t i, j, k; // indices into polytope vertices
  glm::vec3 normal;
  float distance;
};

class Polytope
{
public:
  Polytope() = default;
  Polytope(const Simplex& simplex);
  void AddVertex(const SupportPoint& support);
  const PolytopeTriangle& GetClosestTriangle();
private:
  void UpdateTriangle(size_t triIndex);
  std::vector<SupportPoint> vertices;
  std::vector<PolytopeTriangle> triangles;
  size_t closestTriIndex;

};
