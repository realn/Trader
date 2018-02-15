#include "stdafx.h"

#include <glm/gtx/intersect.hpp>

#include "CoreBLine.h"
#include "CoreBSphere.h"

namespace core {
  void CBSphere::AdjustRadius(glm::vec3 const & point) {
    mRadius = glm::max(mRadius, glm::length(point - (mOrigin + mPos)));
  }
  void CBSphere::AdjustRadius(float const radius) {
    mRadius = glm::max(mRadius, radius);
  }
  bool CBSphere::ContainsPoint(glm::vec3 const & point) const {
    return mRadius > glm::length(point - (mOrigin + mPos));
  }
  bool CBSphere::ContainsLine(glm::vec3 const & beg, glm::vec3 const & end) const {
    return ContainsPoint(beg) && ContainsPoint(end);
  }
  bool CBSphere::ContainsLine(CBLine const & line) const {
    return ContainsLine(line.GetBeg(), line.GetEnd());
  }

  CBSphere::PointsT CBSphere::IntersectsLine(glm::vec3 const & beg, glm::vec3 const & end) const {
    using namespace glm;
    auto pos1 = vec3();
    auto pos2 = vec3();

    if(!intersectLineSphere(beg, end, (mOrigin + mPos), mRadius, pos1, vec3(), pos2, vec3())) {
      return PointsT();
    }
    if(pos1 == pos2) {
      return { pos1 };
    }
    return { pos1, pos2 };
  }

  CBSphere::PointsT CBSphere::IntersectsLine(CBLine const & line) const {
    return IntersectsLine(line.GetBeg(), line.GetEnd());
  }
}
