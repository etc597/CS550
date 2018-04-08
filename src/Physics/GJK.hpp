#pragma once;

#include <glm/glm.hpp>
#include "Physics/Simplex.hpp"
#include "Physics/SupportShape.hpp"

namespace GJK
{
  bool Intersect(const SupportShape& shapeA, const SupportShape& shapeB, Simplex& simplex, unsigned maxIter = 50, float epsilon = 0.001f);
}

