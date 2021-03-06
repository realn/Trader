#pragma once

#include <memory>

#include <CoreBSphere.h>
#include <GFXMeshView.h>

namespace trader {
  class CEntityView {
  private:
    std::shared_ptr<gfx::CMeshView> mMesh;
    glm::vec3 mPosition;
    glm::quat mRotation;
    size_t mFrame = 0;
    glm::vec4 mColorOverride = glm::vec4(0.0f);
    core::CBSphere mBSphere;

  public:
    CEntityView() {}
    CEntityView(std::shared_ptr<gfx::CMeshView> mesh)
      : mMesh(mesh) {}

    void SetPosition(glm::vec3 const value) { mPosition = value; }
    void SetRotation(glm::quat const value) { mRotation = value; }
    void SetFrame(size_t const value) { mFrame = value; }
    void SetColorOverride(glm::vec4 const& value) { mColorOverride = value; }
    void SetBSphere(core::CBSphere const& value) { mBSphere = value; }

    std::shared_ptr<gfx::CMeshView> GetMesh() const { return mMesh; }
    glm::vec3 GetPosition() const { return mPosition; }
    glm::quat GetRotation() const { return mRotation; }
    size_t GetFrame() const { return mFrame; }
    glm::vec4 GetColorOverride() const { return mColorOverride; }
    core::CBSphere const& GetBSphere() const { return mBSphere; }

    glm::mat4 GetTransform() const;
  };
}