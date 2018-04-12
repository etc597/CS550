#include "ContactResolver.hpp"
#include <algorithm>

void ContactResolver::ResolveContact(Contact& contact, float dt)
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
  float vRel = -1.0f;
  Jacobian::Pair velocity[2];
  for (unsigned i = 0; i < 2; ++i)
  {
    velocity[i].linear = bodies[i]->GetLinearVelocity();
    velocity[i].angular = bodies[i]->GetAngularVelocity();

  // jacobian.transform(vRel, velocity);
  if(vRel < 0.0f)
  {
    // bias += std::min(-vRel, 1.0f) * vRel * restitution;
  }
  // baumgarte stability
  // bias += std::min(penetration + slop, 0.0f) * baumgarte * mass_inv[0];

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
  // jacobian.transform(externalAccel, a[0], a[1]);
  float eta = -externalAccel;// -(bias / dt);

  float delta_lambda;
  // solve the normal constraint
    // evaluate delta lambda and add it to lambda
    float jImpulse;
    Jacobian::Pair impulse[2];
    for (unsigned i = 0; i < 2; ++i)
    {
      impulse[i].linear = bodies[i]->GetLinearImpulse();
      impulse[i].angular = bodies[i]->GetAngularImpulse();
    }
    // jacobian.transform(jImpulse, impulse[0], impulse[1]);
    delta_lambda = (eta - jImpulse) * effectiveMass;
    float old_lambda; // = lambda;
    // lambda += delta_lambda;
    // clamp lambda
    delta_lambda;// = lambda - old_lambda;
    // when delta lambda is small or we hit max iter, apply impulse
    if (delta_lambda == 0.0f)
    {
      // apply impulse and bail
    }

  // if we want friction here, repeat process but jacobian uses tangent vecs t1 and t2
}

// apply impulse
  // jacobian.transform(delta_impulse, delta_lambda)
  // bodies[i]->add_impulse(mass_matrix_inv[i] * delta_impulse[i]);
