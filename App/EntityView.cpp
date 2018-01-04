#include "stdafx.h"

#include "EntityView.h"

namespace trader {
  glm::mat4 CEntityView::GetTransform() const {
    return 
      glm::translate(glm::mat4(1.0f), GetPosition()) *
      glm::mat4_cast(GetRotation());
  }
}
