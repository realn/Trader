#include "stdafx.h"

#include <glm/gtx/intersect.hpp>

#include "CoreBSphere.h"

namespace core {
  void CBSphere::AdjustRadius(glm::vec3 const & point) {
    mRadius = glm::max(mRadius, glm::length(point - mOrigin));
  }
  void CBSphere::AdjustRadius(float const radius) {
    mRadius = glm::max(mRadius, radius);
  }
  bool CBSphere::ContainsPoint(glm::vec3 const & point) const {
    return mRadius > glm::length(point - mOrigin);
  }
  bool CBSphere::ContainsLine(glm::vec3 const & beg, glm::vec3 const & end) const {
    return ContainsPoint(beg) && ContainsPoint(end);
  }
  CBSphere::PointsT CBSphere::IntersectsLine(glm::vec3 const & beg, glm::vec3 const & end) const {
    using namespace glm;
    auto pos1 = vec3();
    auto pos2 = vec3();

    if(!intersectLineSphere(beg, end, mOrigin, mRadius, pos1, vec3(), pos2, vec3())) {
      return PointsT();
    }
    if(pos1 == pos2) {
      return { pos1 };
    }
    return { pos1, pos2 };
  }
}
