#include "ContactResolver.hpp"
#include <algorithm>

void ContactResolver::ResolveContact(Contact & contact)
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

  /*
  contact.bodies[0]->GetMassInverse();
  contact.bodies[0]->GetInertiaTensorInverse();

  contact.bodies[1]->GetMassInverse();
  contact.bodies[1]->GetInertiaTensorInverse();

  // store these and compute stuff
  */

  // get the bias value b
  float bias = 0.0f;
  float vRel = -1.0f;
  // auto vel_a = bodies[0]->GetVelocity();
  // auto vel_b = bodies[1]->GetVelocity();
  // jacobian.transform(vRel, vel_a, vel_b, 0);
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
    // a[i] = mass_matrix_inv[i] * bodies[i]->GetForces();
    // something something bodies[i]->GetTorques();
    // a[i] += bodies[i]->GetVelocity() / dt;
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
    // impulse[0] = bodies[0]->GetImpulse();
    // impulse[1] = bodies[1]->GetImpulse();
    // jacobian.transform(jImpulse, impulse[0], impulse[1]);
    delta_lambda = (eta - jImpulse);// *mass;
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
