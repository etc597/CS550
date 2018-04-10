#include "Polytope.hpp"
#include <queue>

Polytope::Polytope(const Simplex & simplex)
  : closestTriIndex(0)
{
  vertices.reserve(4);
  triangles.reserve(4);

  for (size_t i = 0; i < 4; ++i)
  {
    vertices.push_back(simplex[i]);
  }

  static const size_t simplexTris[4][3] = { 
    { 2, 1, 0 },
    { 1, 3, 0 },
    { 1, 2, 3 },
    { 3, 2, 0 }
  };

  for (size_t i = 0; i < 4; ++i)
  {
    Triangle tri;
    tri.i = simplexTris[i][0];
    tri.j = simplexTris[i][1];
    tri.k = simplexTris[i][2];
    triangles.push_back(tri);
    RecalculateTriangle(i);
  }

  RecalculateClosest();
}

void Polytope::AddVertex(const SupportPoint & support)
{
  vertices.push_back(support);
}

void Polytope::Reduce()
{
  auto& newestPoint = vertices.back();
  std::queue<unsigned> trisToRemove;
  std::vector<Edge> edges;
  bool dirtyClosest = false;
  for (unsigned i = 0; i < triangles.size(); ++i)
  {
    auto& tri = triangles[i];
    if (glm::dot(tri.normal, newestPoint.csoPoint - vertices[tri.i].csoPoint) > 0.0f)
    {
      // mark this tri as needing to be removed
      trisToRemove.push(i);

      // stores the edges of this triangle in an edge list (pairs of indices into vertex list)
      edges.push_back(Edge(tri.i, tri.j));
      edges.push_back(Edge(tri.j, tri.k));
      edges.push_back(Edge(tri.k, tri.i));

      // if we are removing the current closest, make sure to recompute closest tri
      if (i == closestTriIndex)
      {
        dirtyClosest = true;
      }
    }
  }

  for (auto& edge : edges)
  {
    // get the points of the triangle (edge i, j are points a and b. support is point c)
    const glm::vec3& a = vertices[edge.i].csoPoint;
    const glm::vec3& b = vertices[edge.j].csoPoint;
    const glm::vec3& c = newestPoint.csoPoint;

    // skip degenerate triangles (tri normal is 0, dist b/t points is small)
    auto n = glm::cross(b - a, c - a);
    if (glm::dot(n, n) < epsilon)
    {
      continue;
    }

    if (glm::dot(a, b) < epsilon
      || glm::dot(b, c) < epsilon
      || glm::dot(c, a) < epsilon)
    {
      continue;
    }

    // create the new triangle (get index from a removed tri or create new index)
    unsigned newIndex;
    if (trisToRemove.empty())
    {
      newIndex = triangles.size();
      triangles.push_back(Triangle());
    }
    else
    {
      newIndex = trisToRemove.front();
      trisToRemove.pop();
    }

    triangles[newIndex] = Triangle(edge.i, edge.j, vertices.size());

    // update the new triangle 
    RecalculateTriangle(newIndex);
  }

  // delete all the remaining old triangles
  while (!trisToRemove.empty())
  {
    triangles.erase(triangles.begin() + trisToRemove.front());
    trisToRemove.pop();
  }

  // if the cloeset point was deleted, search for it again
  if (dirtyClosest)
  {
    RecalculateClosest();
  }
}

const Polytope::Triangle& Polytope::GetClosestTriangle() const
{
  return triangles[closestTriIndex];
}

const SupportPoint & Polytope::GetVertex(unsigned vertIndex) const
{
  return vertices[vertIndex];
}

void Polytope::RecalculateTriangle(size_t triIndex)
{
  auto& tri = triangles[triIndex];
  glm::vec3 a = vertices[tri.i].csoPoint;
  glm::vec3 b = vertices[tri.j].csoPoint;
  glm::vec3 c = vertices[tri.k].csoPoint;

  tri.normal = glm::cross(c - a, b - a);

  auto length = glm::length(tri.normal);
  if (length != 0)
  {
    tri.normal /= length;
  }

  tri.distance = glm::dot(tri.normal, a);

  if (tri.distance < 0.0f) {
    std::swap(tri.i, tri.j);
    tri.distance = -tri.distance;
    tri.normal = -tri.normal;
  }
}

void Polytope::RecalculateClosest()
{
  for (unsigned i = 0; i < triangles.size(); ++i)
  {
    if (triangles[i].distance < triangles[closestTriIndex].distance)
    {
      closestTriIndex = i;
    }
  }
}
