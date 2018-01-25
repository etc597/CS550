#pragma once

#include <glm/vec3.hpp>


namespace ELBA
{

  class Camera
  {
  public:
    Camera();

    glm::vec3 mPosition;
    glm::vec3 mTarget;
    glm::vec3 mDirection;
    glm::vec3 mWorldUp;
    glm::vec3 mCameraUp;
    glm::vec3 mCameraRight;

  };

}