#pragma once
#include "Physics/Simplex.hpp"
#include "Physics/SupportShape.hpp"
#include <vector>

class Polytope
{
public:
  struct Edge
  {
    Edge() = default;
    Edge(unsigned idx1, unsigned idx2) : i(idx1), j(idx2) {};
    unsigned i;
    unsigned j;
  };

  struct Triangle
  {
    Triangle() = default;
    Triangle(unsigned idx1, unsigned idx2, unsigned idx3) : i(idx1), j(idx2), k(idx3) {};
    unsigned i;
    unsigned j;
    unsigned k;
    glm::vec3 normal;
    float distance;
  };

  Polytope() = default;
  Polytope(const Simplex& simplex);
  void AddVertex(const SupportPoint& support);
  void Reduce();
  const Triangle& GetClosestTriangle() const;
  const SupportPoint& GetVertex(unsigned vertIndex) const;
private:
  void RecalculateTriangle(size_t triIndex);
  void RecalculateClosest();
  std::vector<SupportPoint> vertices;
  std::vector<Triangle> triangles;
  size_t closestTriIndex;
  float epsilon = 0.001f;
};
