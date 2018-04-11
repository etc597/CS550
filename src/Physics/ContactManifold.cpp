#include "ContactManifold.hpp"
#include "Math/BarycentricCoords.hpp"

Contact CreateContact(const Polytope & polytope)
{
  Contact newContact;
  CreateContact(polytope, newContact);
  return newContact;
}

ContactType DetermineContactType(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
  auto ab = glm::dot(a - b, a - b);
  auto ac = glm::dot(a - c, a - c);
  auto bc = glm::dot(b - c, b - c);

  float epsilon = 0.001f;
  if (ab > epsilon)
  {
    // if the other edges have length, we are a face
    if (ac > epsilon && bc > epsilon)
    {
      return ContactType::Face;
    }
    return ContactType::Edge;
  }
  else
  {
    // if at least one edge has length, we are a edge
    if (ac > epsilon || bc > epsilon)
    {
      return ContactType::Edge;
    }
    return ContactType::Point;
  }
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
  contactResults.contacts[0].type = DetermineContactType(p0.pointA, p1.pointA, p2.pointA);
  contactResults.contacts[1].type = DetermineContactType(p0.pointB, p1.pointB, p2.pointB);
  
  // all of these are in world space and not in local space
  // aka if we need them to be in local space, we need to transform them with the inverse model matrix
  // TODO(Evan): make these local space and calculate the separating dist
  contactResults.contacts[0].point = ConstructPoint(coords, Triangle(p0.pointA, p1.pointA, p2.pointA));
  contactResults.contacts[1].point = ConstructPoint(coords, Triangle(p0.pointB, p1.pointB, p2.pointB));

  contactResults.contacts[0].normal = tri.normal;
  contactResults.contacts[1].normal = -tri.normal;

  // world space version of contact point (ironic that they're already in world space)
  contactResults.point = contactResults.contacts[0].point;
  contactResults.normal = contactResults.contacts[0].normal;
}
