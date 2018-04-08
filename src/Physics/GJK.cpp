#include "GJK.hpp"

namespace GJK
{
  bool Intersect(const SupportShape& shapeA, const SupportShape& shapeB, Simplex& simplex, unsigned maxIter /*= 50*/, float epsilon /*= 0.001f*/)
  {
    unsigned iter = 0;
    glm::vec3 searchDir = shapeA.GetCenter() - shapeB.GetCenter();
    const glm::vec3 searchPoint = glm::vec3(0, 0, 0);
    SupportPoint initPoint = ComputeSupport(shapeA, shapeB, searchDir);
    searchDir = searchPoint - initPoint.csoPoint;
    simplex = Simplex(initPoint, searchDir);
    
    while (iter < maxIter)
    {
      simplex.Reduce(searchPoint);
      const glm::vec3& closestPoint = simplex.GetClosestPoint();
      if (closestPoint == searchPoint)
      {
        return true;
      }

      searchDir = simplex.GetSearchDir();

      // if the pointToAdd isn't closer, terminate early
      SupportPoint pointToAdd = ComputeSupport(shapeA, shapeB, searchDir);
      float dist = glm::dot(pointToAdd.csoPoint - closestPoint, searchDir);
      if (dist <= epsilon)
      {
        break;
      }

      simplex.AddPoint(pointToAdd);
      ++iter;
    }

    return false;
  }
}
