#include "glm/glm.hpp"

bool IsOutOfRange(float val, float minRange, float maxRange);

struct Line
{
  Line(const glm::vec3& pointA, const glm::vec3& pointB);
  glm::vec3 a;
  glm::vec3 b;
};

typedef glm::vec2 uv;
bool BarycentricCoords(const glm::vec3& point, Line& line, uv& result, float epsilon = 0.001f); 

struct Triangle
{
  Triangle(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC);
  glm::vec3 a;
  glm::vec3 b;
  glm::vec3 c;
};

typedef glm::vec3 uvw;
bool BarycentricCoords(const glm::vec3& point, Triangle& tri, uvw& result, float epsilon = 0.001f);

