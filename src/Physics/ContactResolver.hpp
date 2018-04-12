#pragma once
#include "ContactManifold.hpp"

namespace ContactResolver
{
  struct Jacobian
  {
    struct Pair
    {
      glm::vec3 linear;
      glm::vec3 angular;
    };

    void Transform(float& lambda, const Pair v[2]) const;
    void Transform(Pair v[2], float lambda) const;

    Pair pairs[2];
  };

  void ResolveContact(Contact& contact, float dt);
  void ApplyImpulses(Contact & contact, float delta_lambda, glm::mat3 tensorInv[2], float massInv[2], const Jacobian & jacobian);
}
