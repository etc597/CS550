#include "Camera.hpp"
#include <glm/glm.hpp>

namespace ELBA
{

  Camera::Camera()
  {
    mPosition = glm::vec3(0.0f, 0.0f, 6.0f);
    mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mWorldUp = glm::vec3(0.0f, 0.1f, 0.0f);
    mCameraRight = glm::normalize(glm::cross(mWorldUp, mCameraFront));
    mCameraUp = glm::cross(mCameraFront, mCameraRight);
  }

}