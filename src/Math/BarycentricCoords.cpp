#include "BarycentricCoords.hpp"

bool IsOutOfRange(float val, float minRange, float maxRange)
{
  return val < minRange || val > maxRange;
}

Line::Line(const glm::vec3 & pointA, const glm::vec3 & pointB)
  : a(pointA)
  , b(pointB)
{
}

bool BarycentricCoords(const glm::vec3 & point, Line & line, uv & result, float epsilon)
{
  glm::vec3 BA = line.a - line.b;
  glm::vec3 BP = point - line.b;
  float denom = glm::dot(BA, BA);

  if (denom == 0)
  {
    result = uv(0, 0);
    return false;
  }

  float u = glm::dot(BA, BP) / denom;
  float v = 1 - u;

  float min = -epsilon;
  float max = 1 + epsilon;
  if (IsOutOfRange(u, min, max) || IsOutOfRange(v, min, max))
  {
    return false;
  }

  return true;
}

Triangle::Triangle(const glm::vec3 & pointA, const glm::vec3 & pointB, const glm::vec3 & pointC)
  : a(pointA)
  , b(pointB)
  , c(pointC)
{
}

bool BarycentricCoords(const glm::vec3 & point, Triangle & tri, uvw & result, float epsilon)
{
  glm::vec3 Npbc = glm::cross(tri.b - point, tri.c - point);
  glm::vec3 Npca = glm::cross(tri.c - point, tri.a - point);
  glm::vec3 N = glm::cross(tri.b - tri.a, tri.c - tri.a);

  float area = glm::dot(N, N);

  if (area == 0)
  {
    result = uvw(0, 0, 0);
    return false;
  }

  float u = glm::dot(N, Npbc) / area;
  float v = glm::dot(N, Npca) / area;
  float w = 1 - u - v;

  float min = -epsilon;
  float max = 1 + epsilon;

  if (IsOutOfRange(u, min, max) 
    || IsOutOfRange(v, min, max)
    || IsOutOfRange(w, min, max))
  {
    return false;
  }

  return true;
}
