#include "Polytope.hpp"

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
    PolytopeTriangle tri;
    tri.i = simplexTris[i][0];
    tri.j = simplexTris[i][1];
    tri.k = simplexTris[i][2];
    UpdateTriangle(i);
    triangles.push_back(tri);
  }

  for (size_t i = 0; i < triangles.size(); ++i)
  {
    if (triangles[i].distance < triangles[closestTriIndex].distance)
    {
      closestTriIndex = i;
    }
  }
}

void Polytope::AddVertex(const SupportPoint & support)
{
  vertices.push_back(support);

  for (auto& tri : triangles)
  {
    if (glm::dot(tri.normal, support.csoPoint - vertices[tri.i].csoPoint) > 0.0f)
    {
      // mark this tri as needing to be removed
      // stores the edges of this triangle in an edge list (pairs of indices into vertex list)
      // if we are removing the current closest, make sure to recompute closest tri
    }
  }

  // for each edge
    // get the points of the triangle
    // skip degenerate triangles (tri normal is 0, dist b/t points is small)
    // create the new triangle (get index from a removed tri or create new index)
    // update the new triangle 
    // if tri was wrapped backwards, fix it and inver the normal

  // delete all the remaining old triangles
  // if the cloeset point was deleted, search for it again
}

const PolytopeTriangle & Polytope::GetClosestTriangle()
{
  return triangles[closestTriIndex];
}

void Polytope::UpdateTriangle(size_t triIndex)
{
  // index into vertices for points
  // compute normal
  // compute distance (tri.normal dot vertices[tri.i])
}
