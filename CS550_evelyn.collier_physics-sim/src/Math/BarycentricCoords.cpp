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

bool BarycentricCoords(const glm::vec3 & point, const Line & line, uv & result, float epsilon)
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

  result = uv(u, v);

  float min = -epsilon;
  float max = 1 + epsilon;
  if (IsOutOfRange(u, min, max) || IsOutOfRange(v, min, max))
  {
    return false;
  }

  return true;
}

glm::vec3 ConstructPoint(const uv & coords, const Line & line)
{
  return coords.u * line.a + coords.v * line.b;
}

Triangle::Triangle(const glm::vec3 & pointA, const glm::vec3 & pointB, const glm::vec3 & pointC)
  : a(pointA)
  , b(pointB)
  , c(pointC)
  , n(glm::cross(b - a, c - a))
{
}

bool BarycentricCoords(const glm::vec3 & point, const Triangle & tri, uvw & result, float epsilon)
{
  glm::vec3 Npbc = glm::cross(tri.b - point, tri.c - point);
  glm::vec3 Npca = glm::cross(tri.c - point, tri.a - point);
  glm::vec3 N = tri.n;

  float area = glm::dot(N, N);

  if (area == 0)
  {
    result = uvw(0, 0, 0);
    return false;
  }

  float u = glm::dot(N, Npbc) / area;
  float v = glm::dot(N, Npca) / area;
  float w = 1 - u - v;

  result = uvw(u, v, w);
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

glm::vec3 ConstructPoint(const uvw & coords, const Triangle & tri)
{
  return coords.u * tri.a + coords.v * tri.b + coords.w * tri.c;
}
