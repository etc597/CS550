#include "Simplex.hpp"
#include "Math/BarycentricCoords.hpp"

Simplex::Simplex(const SupportPoint& initPoint, const glm::vec3& initDir) 
  : closestPoint(initPoint.csoPoint)
  , searchDir(initDir)
{
  size = 1;
  simplex[0] = initPoint;
}

bool Simplex::AddPoint(SupportPoint& pointToAdd)
{
  if (size == 4)
    return false;

  simplex[size] = pointToAdd;
  ++size;
  return true;
}

void Simplex::Reduce(const glm::vec3 & searchPoint)
{
  int newIndices[4];
  size_t newSize;
  switch (size)
  {
  case 1:
    ReducePoint(searchPoint, newIndices, newSize);
    break;
  case 2:
    ReduceLine(searchPoint, newIndices, newSize);
    break;
  case 3:
    ReduceTriangle(searchPoint, newIndices, newSize);
    break;
  case 4:
    ReduceTetrahedron(searchPoint, newIndices, newSize);
    break;
  }

  searchDir = searchPoint - closestPoint;
  auto length = glm::length(searchDir);
  if (length != 0)
  {
    searchDir /= length;
  }

  for (size_t i = 0; i < newSize; ++i)
  {
    simplex[i] = simplex[newIndices[i]];
  }
  size = newSize;
}

const glm::vec3 & Simplex::GetClosestPoint() const
{
  return closestPoint;
}

const glm::vec3 & Simplex::GetSearchDir() const
{
  return searchDir;
}

SupportPoint Simplex::ReconstructPoint() const
{
  switch (size)
  {
  case 1:
    return ReconstructFromPoint();
    break;
  case 2:
    return ReconstructFromLine();
    break;
  case 3:
    return ReconstructFromTriangle();
    break;
  default:
    return SupportPoint();
  }
}

const SupportPoint & Simplex::operator[](size_t i) const
{
  return simplex[i];
}

void Simplex::ReducePoint(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
  // reduce to a point
  closestPoint = simplex[0].csoPoint;
  newSize = 1;
  newIndices[0] = 0;
}

void Simplex::ReduceLine(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
  uv coords;
  Line line(simplex[0].csoPoint, simplex[1].csoPoint);
  BarycentricCoords(searchPoint, line, coords);

  // reduce to a point
  if (coords.v <= 0)
  {
    closestPoint = simplex[0].csoPoint;
    newSize = 1;
    newIndices[0] = 0;
    return;
  }

  if (coords.v >= 1)
  {
    closestPoint = simplex[1].csoPoint;
    newSize = 1;
    newIndices[0] = 1;
    return;
  }
  
  // reduce to a line
  closestPoint = ConstructPoint(coords, line);
  newSize = 2;
  newIndices[0] = 0;
  newIndices[1] = 1;
}

void Simplex::ReduceTriangle(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
  Line line_p0p1(simplex[0].csoPoint, simplex[1].csoPoint);
  Line line_p1p2(simplex[0].csoPoint, simplex[1].csoPoint);
  Line line_p2p0(simplex[2].csoPoint, simplex[0].csoPoint);

  uv uv_p0p1;
  uv uv_p1p2;
  uv uv_p2p0;

  BarycentricCoords(searchPoint, line_p0p1, uv_p0p1);
  BarycentricCoords(searchPoint, line_p1p2, uv_p1p2);
  BarycentricCoords(searchPoint, line_p2p0, uv_p2p0);

  // reduce to a point
  if (uv_p0p1.v <= 0 && uv_p2p0.v <= 0)
  {
    closestPoint = simplex[0].csoPoint;
    newSize = 1;
    newIndices[0] = 0;
    return;
  }

  if (uv_p1p2.v <= 0 && uv_p0p1.u <= 0)
  {
    closestPoint = simplex[1].csoPoint;
    newSize = 1;
    newIndices[0] = 1;
    return;
  }

  if (uv_p2p0.v <= 0 && uv_p1p2.u <= 0)
  {
    closestPoint = simplex[2].csoPoint;
    newSize = 1;
    newIndices[0] = 2;
    return;
  }

  uvw coords;
  Triangle tri(simplex[0].csoPoint, simplex[1].csoPoint, simplex[2].csoPoint);

  BarycentricCoords(searchPoint, tri, coords);

  // reduce to a line
  if (coords.w < 0 && uv_p0p1.u > 0 && uv_p0p1.v > 0)
  {
    closestPoint = ConstructPoint(uv_p0p1, line_p0p1);
    newSize = 2;
    newIndices[0] = 0;
    newIndices[1] = 1;
    return;
  }

  if (coords.u < 0 && uv_p1p2.u > 0 && uv_p1p2.v > 0)
  {
    closestPoint = ConstructPoint(uv_p1p2, line_p1p2);
    newSize = 2;
    newIndices[0] = 1;
    newIndices[1] = 2;
    return;
  }

  if (coords.v < 0 && uv_p2p0.u > 0 && uv_p2p0.v > 0)
  {
    closestPoint = ConstructPoint(uv_p2p0, line_p2p0);
    newSize = 2;
    newIndices[0] = 0;
    newIndices[1] = 2;
    return;
  }

  // reduce to a tri
  closestPoint = ConstructPoint(coords, tri);
  newSize = 3;
  newIndices[0] = 0;
  newIndices[1] = 1;
  newIndices[2] = 2;
}

bool CheckNormal(const glm::vec3& checkPoint, const Triangle& tri, const glm::vec3& interiorPoint)
{
  auto normal = tri.n;
  if (glm::dot(normal, interiorPoint - tri.a) > 0)
  {
    normal *= -1;
  }
  return glm::dot(normal, checkPoint - tri.a) > 0;
}

void Simplex::ReduceTetrahedron(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
  uv uv_p0p1;
  Line line_p0p1(simplex[0].csoPoint, simplex[1].csoPoint);
  uv uv_p0p2;
  Line line_p0p2(simplex[0].csoPoint, simplex[2].csoPoint);
  uv uv_p0p3;
  Line line_p0p3(simplex[0].csoPoint, simplex[3].csoPoint);
  uv uv_p1p2;
  Line line_p1p2(simplex[1].csoPoint, simplex[2].csoPoint);
  uv uv_p1p3;
  Line line_p1p3(simplex[1].csoPoint, simplex[3].csoPoint);
  uv uv_p2p3;
  Line line_p2p3(simplex[2].csoPoint, simplex[3].csoPoint);

  BarycentricCoords(searchPoint, line_p0p1, uv_p0p1);
  BarycentricCoords(searchPoint, line_p0p2, uv_p0p2);
  BarycentricCoords(searchPoint, line_p0p3, uv_p0p3);
  BarycentricCoords(searchPoint, line_p1p2, uv_p1p2);
  BarycentricCoords(searchPoint, line_p1p3, uv_p1p3);
  BarycentricCoords(searchPoint, line_p2p3, uv_p2p3);

  // reduce to a point
  if (uv_p0p1.v <= 0 && uv_p0p2.v <= 0 && uv_p0p3.v <= 0)
  {
    closestPoint = simplex[0].csoPoint;
    newSize = 1;
    newIndices[0] = 0;
    return;
  }

  if (uv_p0p1.u <= 0 && uv_p1p2.v <= 0 && uv_p1p3.v <= 0)
  {
    closestPoint = simplex[1].csoPoint;
    newSize = 1;
    newIndices[0] = 1;
    return;
  }

  if (uv_p1p2.u <= 0 && uv_p0p2.u <= 0 && uv_p2p3.v <= 0)
  {
    closestPoint = simplex[2].csoPoint;
    newSize = 1;
    newIndices[0] = 2;
    return;
  }

  if (uv_p0p3.u <= 0 && uv_p2p3.u <= 0 && uv_p1p3.u <= 0)
  {
    closestPoint = simplex[3].csoPoint;
    newSize = 1;
    newIndices[0] = 3;
    return;
  }

  uvw uvw_p0p1p2;
  Triangle tri_p0p1p2(simplex[0].csoPoint, simplex[1].csoPoint, simplex[2].csoPoint);
  uvw uvw_p0p1p3;
  Triangle tri_p0p1p3(simplex[0].csoPoint, simplex[1].csoPoint, simplex[3].csoPoint);
  uvw uvw_p0p2p3;
  Triangle tri_p0p2p3(simplex[0].csoPoint, simplex[2].csoPoint, simplex[3].csoPoint);
  uvw uvw_p1p2p3;
  Triangle tri_p1p2p3(simplex[1].csoPoint, simplex[2].csoPoint, simplex[3].csoPoint);

  BarycentricCoords(searchPoint, tri_p0p1p2, uvw_p0p1p2);
  BarycentricCoords(searchPoint, tri_p0p1p3, uvw_p0p1p3);
  BarycentricCoords(searchPoint, tri_p0p2p3, uvw_p0p2p3);
  BarycentricCoords(searchPoint, tri_p1p2p3, uvw_p1p2p3);

  // reduce to a line
  if (uv_p0p1.u > 0 && uv_p0p1.v > 0 && uvw_p0p1p2.w < 0 && uvw_p0p1p3.w < 0)
  {
    closestPoint = ConstructPoint(uv_p0p1, line_p0p1);
    newSize = 2;
    newIndices[0] = 0;
    newIndices[1] = 1;
    return;
  }

  if (uv_p0p2.u > 0 && uv_p0p2.v > 0 && uvw_p0p1p2.v < 0 && uvw_p0p2p3.w < 0)
  {
    closestPoint = ConstructPoint(uv_p0p2, line_p0p2);
    newSize = 2;
    newIndices[0] = 0;
    newIndices[1] = 2;
    return;
  }

  if (uv_p0p3.u > 0 && uv_p0p3.v > 0 && uvw_p0p1p3.v < 0 && uvw_p0p2p3.v < 0)
  {
    closestPoint = ConstructPoint(uv_p0p3, line_p0p3);
    newSize = 2;
    newIndices[0] = 0;
    newIndices[1] = 3;
    return;
  }

  if (uv_p1p2.u > 0 && uv_p1p2.v > 0 && uvw_p0p1p2.u < 0 && uvw_p1p2p3.w < 0)
  {
    closestPoint = ConstructPoint(uv_p1p2, line_p1p2);
    newSize = 2;
    newIndices[0] = 1;
    newIndices[1] = 2;
    return;
  }

  if (uv_p1p3.u > 0 && uv_p1p3.v > 0 && uvw_p0p1p3.u < 0 && uvw_p1p2p3.v < 0)
  {
    closestPoint = ConstructPoint(uv_p1p3, line_p1p3);
    newSize = 2;
    newIndices[0] = 1;
    newIndices[1] = 3;
    return;
  }

  if (uv_p2p3.u > 0 && uv_p2p3.v > 0 && uvw_p0p2p3.u < 0 && uvw_p1p2p3.u < 0)
  {
    closestPoint = ConstructPoint(uv_p2p3, line_p2p3);
    newSize = 2;
    newIndices[0] = 2;
    newIndices[1] = 3;
    return;
  }

  // reduce to tri
  if (uvw_p0p1p2.u > 0 && uvw_p0p1p2.v > 0 && uvw_p0p1p2.w > 0 && CheckNormal(searchPoint, tri_p0p1p2, simplex[3].csoPoint))
  {
    closestPoint = ConstructPoint(uvw_p0p1p2, tri_p0p1p2);
    newSize = 3;
    newIndices[0] = 0;
    newIndices[1] = 1;
    newIndices[2] = 2;
    return;
  }

  if (uvw_p0p2p3.u > 0 && uvw_p0p2p3.v > 0 && uvw_p0p2p3.w > 0 && CheckNormal(searchPoint, tri_p0p2p3, simplex[1].csoPoint))
  {
    closestPoint = ConstructPoint(uvw_p0p2p3, tri_p0p2p3);
    newSize = 3;
    newIndices[0] = 0;
    newIndices[1] = 2;
    newIndices[2] = 3;
    return;
  }

  if (uvw_p0p1p3.u > 0 && uvw_p0p1p3.v > 0 && uvw_p0p1p3.w > 0 && CheckNormal(searchPoint, tri_p0p1p3, simplex[2].csoPoint))
  {
    closestPoint = ConstructPoint(uvw_p0p1p3, tri_p0p1p3);
    newSize = 3;
    newIndices[0] = 0;
    newIndices[1] = 1;
    newIndices[2] = 3;
    return;
  }

  if (uvw_p1p2p3.u > 0 && uvw_p1p2p3.v > 0 && uvw_p1p2p3.w > 0 && CheckNormal(searchPoint, tri_p1p2p3, simplex[0].csoPoint))
  {
    closestPoint = ConstructPoint(uvw_p1p2p3, tri_p1p2p3);
    newSize = 3;
    newIndices[0] = 1;
    newIndices[1] = 2;
    newIndices[2] = 3;
    return;
  }

  // reduce to tetrahedron
  closestPoint = searchPoint;
  newSize = 4;
  newIndices[0] = 0;
  newIndices[1] = 1;
  newIndices[2] = 2;
  newIndices[3] = 3;
}

SupportPoint Simplex::ReconstructFromPoint() const
{
  return simplex[0];
}

SupportPoint Simplex::ReconstructFromLine() const
{
  SupportPoint ret;
  uv coords;
  BarycentricCoords(closestPoint, Line(simplex[0].csoPoint, simplex[1].csoPoint), coords);
  ret.csoPoint = closestPoint;
  ret.pointA = ConstructPoint(coords, Line(simplex[0].pointA, simplex[1].pointA));
  ret.pointB = ConstructPoint(coords, Line(simplex[0].pointB, simplex[1].pointB));
  return ret;
}

SupportPoint Simplex::ReconstructFromTriangle() const
{
  SupportPoint ret;
  uvw coords;
  BarycentricCoords(closestPoint, Triangle(simplex[0].csoPoint, simplex[1].csoPoint, simplex[2].csoPoint), coords);
  ret.csoPoint = closestPoint;
  ret.pointA = ConstructPoint(coords, Triangle(simplex[0].pointA, simplex[1].pointA, simplex[2].pointA));
  ret.pointB = ConstructPoint(coords, Triangle(simplex[0].pointB, simplex[1].pointB, simplex[2].pointB));
  return ret;
}
