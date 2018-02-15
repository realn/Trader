#pragma once

#include <glm/vec3.hpp>

namespace core {
  class CBLine;

  class CBSphere {
  public:
    using PointsT = std::vector<glm::vec3>;

  private:
    glm::vec3 mOrigin;
    float mRadius;
    glm::vec3 mPos;

  public:
    CBSphere(glm::vec3 const& origin = glm::vec3(), float const radius = 0.0f) 
      : mOrigin(origin), mRadius(radius) {}

    void SetOrigin(glm::vec3 const& value) { mOrigin = value; }
    void SetRaidus(float const value) { mRadius = value; }
    void SetPos(glm::vec3 const& value) { mPos = value; }

    glm::vec3 GetOrigin() const { return mOrigin; }
    float GetRadius() const { return mRadius; }
    glm::vec3 GetPos() const { return mPos; }

    void AdjustRadius(glm::vec3 const& point);
    void AdjustRadius(float const radius);

    bool ContainsPoint(glm::vec3 const& point) const;
    bool ContainsLine(glm::vec3 const& beg, glm::vec3 const& end) const;
    bool ContainsLine(CBLine const& line) const;

    PointsT IntersectsLine(glm::vec3 const& beg, glm::vec3 const& end) const;
    PointsT IntersectsLine(CBLine const& line) const;

  };
}