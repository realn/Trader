#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace trader {
  // Projection * View * Model

  class CViewportConfig {
  private:
    float mFov = glm::radians(45.0f);
    float mAspectRatio = 16.0f / 9.0f;
    float mNearZ = 1.0f;
    float mFarZ = 100.0f;

  public:
    CViewportConfig() = default;
    CViewportConfig(float fov, float aspRatio, float nearZ, float farZ) :
      mFov(fov), mAspectRatio(aspRatio), mNearZ(nearZ), mFarZ(farZ) {}

    void SetAspectRatio(float const value) { mAspectRatio = value; }
    void SetAspectRatio(glm::uvec2 const& size) { 
      mAspectRatio = (size.y != 0 ? static_cast<float>(size.x) / static_cast<float>(size.y) : mAspectRatio);
    }

    float GetAspectRatio() const { return mAspectRatio; }

    glm::mat4 GetProjection() const {
      return glm::perspective(mFov, mAspectRatio, mNearZ, mFarZ);
    }

    glm::vec2 CreateAspectCorrectSize(float const height) const {
      return glm::vec2(mAspectRatio * height, height);
    }
  };

  class CCamera {
  private:
    glm::vec3 mPosition;
    glm::vec3 mRotation = {glm::radians(45.0f), 0.0f, 0.0f};
    glm::vec3 mOffset = { 0.0f, 0.0f, -3.0f };

  public:
    CCamera();

    void SetPosition(glm::vec3 const& value) { mPosition = value; }
    void SetRotation(glm::vec3 const& value) { mRotation = value; }
    void SetOffset(glm::vec3 const& value) { mOffset = value; }

    void ModPosition(glm::vec3 const& value) { mPosition += value; }
    void ModRotation(glm::quat const& value);
    void ModOffset(glm::vec3 const& value) { mOffset += value; }

    glm::vec3 GetPosition() const { return mPosition; }
    glm::vec3 GetRotation() const { return mRotation; }
    glm::vec3 GetOffset() const { return mOffset; }

    glm::mat4 GetTransform() const;
  };
}
