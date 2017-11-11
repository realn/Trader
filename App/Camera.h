#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace trader {
  class CCamera {
  private:
    float mFov = glm::radians(90.0f);
    float mAspectRatio = 4.0f / 3.0f;
    float mNearZ = 1.0f;
    float mFarZ = 100.0f;
    glm::vec3 mPosition;
    glm::vec3 mRotation = {glm::radians(45.0f), 0.0f, 0.0f};
    float mZoom = 3.0f;

  public:
    CCamera();

    void SetFov(float value) { mFov = value; }
    void SetAspectRatio(float value) { mAspectRatio = value; }
    void SetAspectRatio(float width, float height) { mAspectRatio = width / height; }
    void SetAspectRatio(glm::vec2 const& value) { SetAspectRatio(value.x, value.y); }
    void SetAspectRatio(glm::uvec2 const& value) { SetAspectRatio(glm::vec2(value)); }
    void SetDepthRange(float near, float far) { mNearZ = near; mFarZ = far; }
    void SetRotation(float x, float y, float z) { mRotation = {x, y, z}; }
    void SetRotation(glm::vec3 const& value) { mRotation = value; }

    void ModRotation(glm::vec3 const& value);
    void ModRotation(glm::quat const& value);

    glm::mat4 GetProjection() const;
    glm::mat4 GetTransform() const;
  };
}