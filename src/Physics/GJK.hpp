#pragma once;

#include <glm/glm.hpp>
#include "Physics/Simplex.hpp"
#include "Physics/SupportShape.hpp"

bool Intersect(const SupportShape& shapeA, const SupportShape& shapeB, Simplex& simplex, float epsilon)

