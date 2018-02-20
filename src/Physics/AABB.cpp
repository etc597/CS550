#include "AABB.hpp"

#include <algorithm>

AABB::AABB(const glm::vec3 & min, const glm::vec3 & max)
  : mMin(min)
  , mMax(max)
{
}

float AABB::GetVolume() const
{
  glm::vec3 dim = mMax - mMin;
  return dim[0] * dim[1] * dim[2];
}

float AABB::GetSurfaceArea() const
{
  glm::vec3 dim = mMax - mMin;
  return 2 * (dim[0] * dim[1] + dim[0] * dim[2] + dim[1] * dim[2]);
}

void AABB::Compute(const std::vector<glm::vec3>& points)
{
  if (points.empty()) {
    return;
  }

  mMin = points[0];
  mMax = points[0];
  for (auto& pt : points) {
    for (unsigned i = 0; i < 3; ++i) {
      mMin[i] = std::min(mMin[i], pt[i]);
      mMax[i] = std::max(mMax[i], pt[i]);
    }
  }
}

bool AABB::Contains(const AABB & aabb) const
{
  for (unsigned i = 0; i < 3; ++i) {
    if (aabb.GetMax()[i] > mMax[i] || aabb.GetMin()[i] < mMin[i]) {
      return false;
    }
  }
  return true;
}

bool AABB::Collides(const AABB & aabb) const
{
  for (int i = 0; i < 3; ++i) {
    if (mMin[i] > aabb.mMax[i]
      || aabb.mMin[i] > mMax[i]) {
      return false;
    }
  }

  return true;
}

AABB AABB::operator+(const glm::vec3 & point) const
{
  AABB ret(mMin, mMax);
  
  return ret += point;
}

AABB & AABB::operator+=(const glm::vec3 & point)
{
  for (unsigned i = 0; i < 3; ++i) {
    mMin[i] = std::min(mMin[i], point[i]);
    mMax[i] = std::max(mMax[i], point[i]);
  }
}

AABB AABB::operator+(const AABB & lhs) const
{
  AABB ret(*this);
  return ret += lhs;
}

AABB & AABB::operator+=(const AABB & lhs)
{
  for (unsigned i = 0; i < 3; ++i) {
    mMin[i] = std::min(mMin[i], lhs.mMin[i]);
    mMax[i] = std::max(mMax[i], lhs.mMax[i]);
  }
}

void AABB::Transform(const glm::mat4 & transform)
{
  glm::vec3 c = GetCenter();
  glm::vec3 r = GetHalfExtents();

  glm::vec4 center(c, 1);
  glm::vec4 radius(r, 0);

  center = transform * center;
  glm::mat4 absTransform = transform;
  for (unsigned i = 0; i < 4; ++i) {
    for (unsigned j = 0; j < 4; ++j) {
      absTransform[i][j] = std::abs(absTransform[i][j]);
    }
  }

  radius = absTransform * radius;

  c = glm::vec3(center[0], center[1], center[2]);
  r = glm::vec3(radius[0], radius[1], radius[2]);

  mMax = c + r;
  mMin = c - r;
}

glm::vec3 AABB::GetMin() const
{
  return mMin;
}

glm::vec3 AABB::GetMax() const
{
  return mMax;
}

glm::vec3 AABB::GetCenter() const
{
  return (mMin + mMax) * 0.5f;
}

glm::vec3 AABB::GetHalfExtents() const
{
  return (mMax - mMin) * 0.5f;
}
