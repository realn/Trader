#include "stdafx.h"

#include "EntityView.h"

namespace trader {
  glm::vec3 CEntityView::GetPosition() const {
    return glm::vec3(mEntity->GetPosition(), 0.0f);
  }
  glm::quat CEntityView::GetRotation() const {
    return glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  glm::mat4 CEntityView::GetTransform() const {
    return 
      glm::translate(glm::mat4(1.0f), GetPosition()) *
      glm::mat4_cast(GetRotation());
  }
}
