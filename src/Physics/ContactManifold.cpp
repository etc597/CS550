#include "ContactManifold.hpp"
#include "Math/BarycentricCoords.hpp"

Contact CreateContact(const Polytope & polytope)
{
  Contact newContact;
  CreateContact(polytope, newContact);
  return newContact;
}

void CreateContact(const Polytope & polytope, Contact & contactResults)
{
  auto& tri = polytope.GetClosestTriangle();
  auto p0 = polytope.GetVertex(tri.i);
  auto p1 = polytope.GetVertex(tri.j);
  auto p2 = polytope.GetVertex(tri.k);

  glm::vec3 resultPoint = tri.normal * tri.distance;
  uvw coords;
  BarycentricCoords(resultPoint, Triangle(p0.csoPoint, p1.csoPoint, p2.csoPoint), coords);
  
  // determine feature type here based on p0.a, ... and p0.b, ...
  
  contactResults.contacts[0].point = ConstructPoint(coords, Triangle(p0.pointA, p1.pointA, p2.pointA));
  contactResults.contacts[1].point = ConstructPoint(coords, Triangle(p0.pointB, p1.pointB, p2.pointB));

  contactResults.contacts[0].normal = -tri.normal;
  contactResults.contacts[1].normal = tri.normal;

  //auto normalA = -tri.normal; // recompute normal from points in a / b instead???
  // auto normalB = a2b * vec4(tri.normal, 0); ???

  // world space version of contact point
  contactResults.point = contactResults.bodies[0]->GetModelMatrix()
    * glm::vec4(contactResults.contacts[0].point, 1.0f);
  contactResults.normal = contactResults.bodies[0]->GetModelMatrix()
    * glm::vec4(contactResults.contacts[0].normal, 0.0f);
}
