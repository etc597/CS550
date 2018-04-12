#include "ContactResolver.hpp"
#include <algorithm>

namespace ContactResolver
{
  void ResolveContact(Contact& contact, float dt)
  {
    // make sure penetration is valid and then cross normals with world pos points
    float penetration = contact.depth;
    glm::vec3 n = contact.normal;
    glm::vec3 axn = glm::cross(contact.contacts[0].point, n);
    glm::vec3 bxn = glm::cross(contact.contacts[1].point, n);

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
    float effectiveMassInv;// = jacobian.effectiveMass(massInv, tensorInv);
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
    jacobian.transform(vRel, velocity);
    float restitution = 0.0f;
    float baumgarte = 0.5f;
    if (vRel < 0.0f)
    {
      bias += std::min(-vRel, 1.0f) * vRel * restitution;
    }
    // baumgarte stability
    bias += std::min(penetration, 0.0f) * baumgarte * effectiveMassInv;

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
     jacobian.transform(externalAccel, a);
    float eta = -externalAccel - (bias / dt);

    float lambda = 0.0f;
    float delta_lambda;
    // solve the normal constraint
    unsigned iter = 0;
    while (iter < 50) 
    {
      // evaluate delta lambda and add it to lambda
      float jImpulse;
      Jacobian::Pair impulse[2];
      for (unsigned i = 0; i < 2; ++i)
      {
        impulse[i].linear = bodies[i]->GetLinearImpulse();
        impulse[i].angular = bodies[i]->GetAngularImpulse();
      }
      jacobian.transform(jImpulse, impulse);
      delta_lambda = (eta - jImpulse) * effectiveMass;
      float old_lambda = lambda;
      lambda += delta_lambda;
      if (lambda < 0.0f) lambda = 0.0f;
      delta_lambda = lambda - old_lambda;
      // when delta lambda is small or we hit max iter, apply impulse
      if (delta_lambda != 0.0f)
      {
        // apply impulse
      }
      else
      {
        break;
      }
    }

    // if we want friction here, repeat process but jacobian uses tangent vecs t1 and t2
  }

  // apply impulse
    // jacobian.transform(delta_impulse, delta_lambda)
    // bodies[i]->add_impulse(mass_matrix_inv[i] * delta_impulse[i]);
  
  void Jacobian::transform(float & lambda, const Pair v[2]) const
  {
    lambda = 0.0f;
    for (unsigned i = 0; i < 2; ++i)
    {
      lambda += glm::dot(v[i].linear, pairs[i].linear)
        + dot(v[i].angular, pairs[i].angular);
    }
  }
}