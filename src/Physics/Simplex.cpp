#include "Simplex.hpp"
#include "Math/BarycentricCoords.hpp"

Simplex::Simplex(SupportPoint & initPoint, const glm::vec3 & initDir)
{
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
    ReduceTriangle(searchPoint, newIndices, size);
    break;
  case 4:
    ReduceTetrahedron(searchPoint, newIndices, size);
    break;
  }

  searchDir = searchPoint - closestPoint.csoPoint;

  for (size_t i = 0; i < newSize; ++i)
  {
    simplex[i] = simplex[newIndices[i]];
  }
  size = newSize;
}

const glm::vec3 & Simplex::GetClosestPoint() const
{
  return closestPoint.csoPoint;
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
  }
}

void Simplex::ReducePoint(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
  closestPoint = simplex[0];
  newSize = 1;
  newIndices[0] = 0;
}

void Simplex::ReduceLine(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
  uv coords;
  BarycentricCoords(searchPoint, Line(simplex[0].csoPoint, simplex[1].csoPoint), coords);

  if (coords.v <= 0)
  {
    closestPoint = simplex[0];
    newSize = 1;
    newIndices[0] = 0;
  }

  if (coords.v >= 1)
  {
    closestPoint = simplex[1];
    newSize = 1;
    newIndices[0] = 1;
  }

  closestPoint = 
}

void Simplex::ReduceTriangle(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
}

void Simplex::ReduceTetrahedron(const glm::vec3& searchPoint, int newIndices[4], size_t& newSize)
{
}

SupportPoint Simplex::ReconstructFromPoint() const
{
  return simplex[0];
}

SupportPoint Simplex::ReconstructFromLine() const
{
  return SupportPoint();
}

SupportPoint Simplex::ReconstructFromTriangle() const
{
  return SupportPoint();
}
