#pragma once
#include "ContactManifold.hpp"

namespace ContactResolver
{
  void ResolveContact(Contact& contact);

  struct Jacobian
  {
    struct Pair
    {
      glm::vec3 linear;
      glm::vec3 angular;
    };

    Pair pairs[2];
  };
}
