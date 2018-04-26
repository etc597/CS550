#pragma once
#include "Physics/Polytope.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Simplex.hpp"

enum ContactType { Point, Edge, Face };

struct ContactPoint
{
  glm::vec3 point;
  glm::vec3 normal;
  ContactType type;
};

struct Contact
{
  float depth;
  RigidBody* bodies[2];
  ContactPoint contacts[2];
  glm::vec3 point;
  glm::vec3 normal;
  float lambda = 0.0f;
};

Contact CreateContact(const Polytope& polytope);
void CreateContact(const Polytope& polytope, Contact& contactResults);
