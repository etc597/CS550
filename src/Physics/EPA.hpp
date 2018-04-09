#pragma once
#include <glm/glm.hpp>
#include "Physics/SupportShape.hpp"
#include "Physics/Polytope.hpp"

namespace EPA
{
  void Expand(const SupportShape& shapeA, const SupportShape& shapeB, Polytope& polytope, unsigned maxIter = 50, float epsilon = 0.001f);
}
