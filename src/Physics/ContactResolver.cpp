#include "ContactResolver.hpp"
#include <algorithm>

namespace ContactResolver
{
  void Jacobian::Transform(float& lambda, const Pair v[2]) const
  {
    lambda = 0.0f;
    for (unsigned i = 0; i < 2; ++i)
    {
      lambda += glm::dot(v[i].linear, pairs[i].linear)
        + dot(v[i].angular, pairs[i].angular);
    }
  }

  void Jacobian::Transform(Pair v[2], float lambda) const
  {
    for (unsigned i = 0; i < 2; ++i)
    {
      v[i].linear = pairs[i].linear * lambda;
      v[i].angular = pairs[i].angular * lambda;
    }
  }

  void Jacobian::CalculateEffectiveMassInverse(float& effectiveMassInv, float massInv[2], glm::mat3 tensorInv[2])
  {
    effectiveMassInv = 0.0f;
    for (unsigned i = 0; i < 2; ++i)
    {
      effectiveMassInv += glm::dot(pairs[i].linear, massInv[i] * pairs[i].linear)
        + glm::dot(pairs[i].angular, tensorInv[i] * pairs[i].angular);
    }
  }

  void ResolveContact(Contact& contact, float dt)
  {
    // make sure penetration is valid and then cross normals with world pos points
    float penetration = contact.depth;
    glm::vec3 n = contact.normal;
    auto bodyPos0 = contact.bodies[0]->GetPos();
    auto bodyPos1 = contact.bodies[1]->GetPos();
    glm::vec3 axn = glm::cross(contact.contacts[0].point - bodyPos0, n); // should be point - objPos
    glm::vec3 bxn = glm::cross(contact.contacts[1].point - bodyPos1, n); // should be point - objPos

    // create the jacobian from the contact
    Jacobian jacobian;
    jacobian.pairs[0].linear = -n;
    jacobian.pairs[0].angular = axn;
    jacobian.pairs[1].linear = n;
    jacobian.pairs[1].angular = bxn;

    // get the mass matrix / inertia tensor thing M

    float massInv[2];
    glm::mat3 tensorInv[2];

    for (unsigned i = 0; i < 2; ++i)
    {
      massInv[i] = contact.bodies[i]->GetMassInverse();
      tensorInv[i] = contact.bodies[i]->GetInertiaTensorInverse();
    }
    float effectiveMassInv;
    jacobian.CalculateEffectiveMassInverse(effectiveMassInv, massInv, tensorInv);
    float effectiveMass = effectiveMassInv != 0.0f ? 1.0f / effectiveMassInv : 0.0f;

    auto& bodies = contact.bodies;
    // get the bias value b
    float bias = 0.0f;
    float vRel;
    Jacobian::Pair velocity[2];
    for (unsigned i = 0; i < 2; ++i)
    {
      velocity[i].linear = bodies[i]->GetLinearVelocity();
      velocity[i].angular = bodies[i]->GetAngularVelocity();
    }
    jacobian.Transform(vRel, velocity);
    float restitution = 0.1f;
    float baumgarte = 0.5f;
    if (vRel > 0.0f)
    {
      bias += vRel * restitution;
    }
    // baumgarte stability
    bias += penetration * baumgarte;

    // get the acceleration value (needs dt) - [J * v0 / dt] + [J * M-1 * Fext] 
    Jacobian::Pair a[2];
    for (unsigned i = 0; i < 2; ++i)
    {
      a[i].linear = massInv[i] * bodies[i]->GetForces();
      a[i].angular = tensorInv[i] * bodies[i]->GetTorques();
      a[i].linear += bodies[i]->GetLinearVelocity() / dt;
      a[i].angular += bodies[i]->GetAngularVelocity() / dt;
    }

    // apply impulse from stored solution values (don't have currently so skip)

    // determine target impulse
    float externalAccel;
    jacobian.Transform(externalAccel, a);
    float eta = -externalAccel + (bias / dt);

    float lambda = 0.0f;
    float delta_lambda;
    // solve the normal constraint
    unsigned iter = 0;
    while (iter < 50) // 50 could potentially be a bit high
    {
      // evaluate delta lambda and add it to lambda
      float jImpulse;
      Jacobian::Pair impulse[2];
      for (unsigned i = 0; i < 2; ++i)
      {
        impulse[i].linear = bodies[i]->GetLinearImpulse();
        impulse[i].angular = bodies[i]->GetAngularImpulse();
      }
      jacobian.Transform(jImpulse, impulse);
      delta_lambda = (eta - jImpulse) * effectiveMass;
      float old_lambda = lambda;
      lambda += delta_lambda;
      if (lambda < 0.0f)
      {
        lambda = 0.0f;
      }
      delta_lambda = lambda - old_lambda;
      // when delta lambda is small or we hit max iter, apply impulse
      if (delta_lambda != 0.0f)
      {
        ApplyImpulses(contact, delta_lambda, tensorInv, massInv, jacobian);
      }
      else
      {
        break;
      }
      ++iter;
    }

    // friction is same, just jacobian with tangent and bitangent and then clamp to [-constant, +constant]

    // if we want friction here, repeat process but jacobian uses tangent vecs t1 and t2
  }

  void ApplyImpulses(Contact & contact, float delta_lambda, glm::mat3 tensorInv[2], float massInv[2], const Jacobian & jacobian)
  {
    Jacobian::Pair deltaImpulse[2];
    jacobian.Transform(deltaImpulse, delta_lambda);
    for (unsigned i = 0; i < 2; ++i)
    {
      contact.bodies[i]->ApplyLinearImpulse(massInv[i] * deltaImpulse[i].linear);
      contact.bodies[i]->ApplyAngularImpulse(tensorInv[i] * deltaImpulse[i].angular);
    }
  }
}