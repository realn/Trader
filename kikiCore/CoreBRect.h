#pragma once

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace core {
  enum class RectEdge {
    None, XMin, XMax, YMin, YMax
  };
  enum class RectCorner {
    None, Min, MinMax, Max, MaxMin
  };

  class CBRect {
  private:
    glm::vec2 mPos;
    glm::vec2 mSize;

  public:
    CBRect() = default;
    CBRect(glm::vec2 const& pos, glm::vec2 const& size)
      : mPos(pos), mSize(size) {}

    glm::vec2 GetPosition() const { return mPos; }
    glm::vec2 GetSize() const { return mSize; }

    glm::vec2 GetMin() const { return mPos; }
    glm::vec2 GetMax() const { return mPos + mSize; }
    glm::vec2 GetMinMax() const { return {mPos.x, mPos.y + mSize.y}; }
    glm::vec2 GetMaxMin() const { return {mPos.x + mSize.x, mPos.y}; }

    glm::vec2 GetCenter() const { return mPos + (mSize / 2.0f); }
    glm::vec2 GetUCrd(glm::uvec2 const& xy) const { return mPos + glm::vec2(xy) * mSize; }
    glm::vec2 GetUCrd(unsigned const x, unsigned const y) const { return GetUCrd({x, y}); }

    CBRect GetEdge(RectEdge side) const;
    CBRect GetCorner(RectCorner corner) const;

    bool Contains(glm::vec2 const& pos) const;
    bool Contains(CBRect const& rect) const;

    RectEdge ContainsEdgeOf(CBRect const& rect) const;
    RectCorner ContainsCornerOf(CBRect const& rect) const;

    static glm::vec2 GetNormal(RectEdge edge);
    static glm::vec2 GetNormal(RectCorner corner);
  };
}