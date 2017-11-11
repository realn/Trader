#include "stdafx.h"
#include "Camera.h"

namespace trader {
  CCamera::CCamera() {}

  inline void CCamera::ModRotation(glm::vec3 const & value) { mRotation = glm::modf(mRotation, glm::two_pi<float>()); }

  glm::mat4 CCamera::GetProjection() const {
    return glm::perspective(mFov, mAspectRatio, mNearZ, mFarZ);
  }

  glm::mat4 CCamera::GetTransform() const {
    using namespace glm;
    return 
      translate(mat4(1.0f), vec3(0.0f, 0.0f, -mZoom)) *
      rotate(mat4(1.0f), mRotation.x, {1.0f, 0.0f, 0.0f}) *
      rotate(mat4(1.0f), mRotation.y, {0.0f, 1.0f, 0.0f}) *
      rotate(mat4(1.0f), mRotation.z, {0.0f, 0.0f, 1.0f}) *
      translate(mat4(1.0f), mPosition);
  }
}