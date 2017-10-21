#include "stdafx.h"
#include "Entity.h"

namespace trader {
  CEntity::CEntity(glm::vec3 pos, glm::quat rot) 
    : mPosition(pos), mRotation(rot)
  {}

  glm::mat4 CEntity::GetTransform() const {
    using namespace glm;
    return translate(mat4(1.0f), mPosition) * mat4_cast(mRotation);
  }

}
