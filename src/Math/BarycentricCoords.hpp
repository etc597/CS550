#include "glm/glm.hpp"

bool IsOutOfRange(float val, float minRange, float maxRange);

struct Line
{
  Line(const glm::vec3& pointA, const glm::vec3& pointB);
  const glm::vec3 a;
  const glm::vec3 b;
};

struct uv
{
  uv(float uVal = 0, float vVal = 0) : u(uVal), v(vVal) {};
  float u;
  float v;
};
bool BarycentricCoords(const glm::vec3& point, const Line& line, uv& result, float epsilon = 0.0f); 
glm::vec3 ConstructPoint(const uv& coords, const Line& line);


struct Triangle
{
  Triangle(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC);
  const glm::vec3 a;
  const glm::vec3 b;
  const glm::vec3 c;
  const glm::vec3 n;
};


struct uvw
{
  uvw(float uVal = 0, float vVal = 0, float wVal = 0) : u(uVal), v(vVal), w(wVal) {};
  float u;
  float v;
  float w;
};
bool BarycentricCoords(const glm::vec3& point, const Triangle& tri, uvw& result, float epsilon = 0.0f);
glm::vec3 ConstructPoint(const uvw& coords, const Triangle& tri);
