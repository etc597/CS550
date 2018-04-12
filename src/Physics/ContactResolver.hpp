#pragma once
#include "ContactManifold.hpp"

namespace ContactResolver
{
  void ResolveContact(Contact& contact, float dt);

  struct Jacobian
  {
    struct Pair
    {
      glm::vec3 linear;
      glm::vec3 angular;
    };

    void transform(float& lambda, const Pair v[2]) const;

    Pair pairs[2];
  };
}
