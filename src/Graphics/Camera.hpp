#pragma once

#include <glm/vec3.hpp>


namespace ELBA
{

  class Camera
  {
  public:
    Camera();

    glm::vec3 mPosition;
    glm::vec3 mWorldUp;
    glm::vec3 mCameraFront;
    glm::vec3 mCameraUp;
    glm::vec3 mCameraRight;

    float mPitch = 0;
    float mYaw = 0;
    bool mMove = false;
    float mFov = 45.0f;
  };

}