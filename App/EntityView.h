#pragma once

#include <memory>

#include <ECOEntity.h>
#include <GFXMeshView.h>

namespace trader {
  class CEntityView {
  private:
    std::shared_ptr<eco::CEntity> mEntity;
    std::shared_ptr<gfx::CMeshView> mMesh;

  public:
    CEntityView(std::shared_ptr<eco::CEntity> entity,
                std::shared_ptr<gfx::CMeshView> mesh)
      : mEntity(entity), mMesh(mesh) {}

    gfx::CMeshView& GetMesh() const { return *mMesh; }

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::mat4 GetTransform() const;
  };
}