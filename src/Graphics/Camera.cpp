#include "Camera.hpp"
#include <glm/glm.hpp>

namespace ELBA
{

  Camera::Camera()
  {
    mPosition = glm::vec3(0.0f, 0.0f, 6.0f);
    mTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    mDirection = glm::normalize(mPosition - mTarget);
    mWorldUp = glm::vec3(0.0f, 0.1f, 0.0f);
    mCameraRight = glm::normalize(glm::cross(mWorldUp, mDirection));
    mCameraUp = glm::cross(mDirection, mCameraRight);
  }

}