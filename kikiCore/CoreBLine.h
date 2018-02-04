#pragma once

#include <glm/vec3.hpp>

namespace core {
  class CBLine {
  private:
    glm::vec3 mBeg;
    glm::vec3 mEnd;

  public:
    CBLine(glm::vec3 const beg = glm::vec3(), 
           glm::vec3 const end = glm::vec3()) 
      : mBeg(beg), mEnd(end) {}

    void SetBeg(glm::vec3 const value) { mBeg = value; }
    void SetEnd(glm::vec3 const value) { mEnd = value; }

    glm::vec3 GetBeg() const { return mBeg; }
    glm::vec3 GetEnd() const { return mEnd; }
  };
}
