#include "stdafx.h"
#include "Camera.h"

namespace trader {
  CCamera::CCamera() {}

  void CCamera::ModRotation(glm::quat const & value) {
    mRotation += glm::eulerAngles(value);
  }


  glm::mat4 CCamera::GetTransform() const {
    using namespace glm;
    return 
      translate(mat4(1.0f), mOffset) *
      rotate(mat4(1.0f), mRotation.x, {1.0f, 0.0f, 0.0f}) *
      rotate(mat4(1.0f), mRotation.y, {0.0f, 1.0f, 0.0f}) *
      rotate(mat4(1.0f), mRotation.z, {0.0f, 0.0f, 1.0f}) *
      translate(mat4(1.0f), mPosition);
  }
}