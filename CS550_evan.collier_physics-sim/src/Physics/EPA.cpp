#include "EPA.hpp"

void EPA::Expand(const SupportShape & shapeA, const SupportShape & shapeB, Polytope & polytope, unsigned maxIter, float epsilon)
{
  unsigned iter = 0;
  while (iter < maxIter)
  {
    Polytope::Triangle tri = polytope.GetClosestTriangle();
    SupportPoint point = ComputeSupport(shapeA, shapeB, tri.normal);

    // if the new point is no closer than the closest tri, stop
    float dist = glm::dot(point.csoPoint, tri.normal);
    if (dist - tri.distance < epsilon)
    {
      break;
    }

    polytope.AddVertex(point);
    polytope.Reduce();
    ++iter;
  }
}
