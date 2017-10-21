#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GFXFwd.h>

namespace trader {
  class CEntity {
  private:
    glm::vec3 mPosition;
    glm::quat mRotation;
    std::shared_ptr<gfx::CMeshView> mMesh;

  public:
    CEntity() = default;
    CEntity(glm::vec3 pos, glm::quat rot);

    glm::mat4 GetTransform() const;
  };
}