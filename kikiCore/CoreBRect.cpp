#include "stdafx.h"
#include "CoreBRect.h"

namespace core {
  cb::u32 constexpr flipValue(cb::u32 const value, bool const flip) { 
    if(!flip) { return value; }
    return value == 0 ? 1 : 0; 
  }

  glm::uvec2 flipValue(glm::uvec2 const& value, RectFlip const flip) {
    return {
      flipValue(value.x, flip == RectFlip::Horizontal || flip == RectFlip::Both),
      flipValue(value.y, flip == RectFlip::Vertical || flip == RectFlip::Both)
    };
  }

  glm::vec2 CBRect::GetUCrd(glm::uvec2 const & xy, RectFlip const flip) const { 
    return mPos + glm::vec2(flipValue(xy, flip)) * mSize; 
  }

  CBRect CBRect::GetEdge(RectEdge side) const {
    switch(side) {
    case RectEdge::XMin: return CBRect(GetMin(), {0.0f, mSize.y});
    case RectEdge::XMax: return CBRect(GetMaxMin(), {0.0f, mSize.y});
    case RectEdge::YMin: return CBRect(GetMin(), {mSize.x, 0.0f});
    case RectEdge::YMax: return CBRect(GetMinMax(), {mSize.x, 0.0f});
    default:
      return *this;
    }
  }

  CBRect CBRect::GetCorner(RectCorner corner) const {
    switch(corner) {
    case RectCorner::Min:     return CBRect(GetMin(), {0.0f, 0.0f});
    case RectCorner::MinMax:  return CBRect(GetMinMax(), {0.0f, 0.0f});
    case RectCorner::Max:     return CBRect(GetMax(), {0.0f, 0.0f});
    case RectCorner::MaxMin:  return CBRect(GetMaxMin(), {0.0f, 0.0f});
    default:
      return *this;
    }
  }

  inline bool CBRect::Contains(glm::vec2 const & pos) const {
    return 
      glm::all(glm::greaterThan(pos, GetMin())) &&
      glm::all(glm::lessThan(pos, GetMax()));
  }

  bool CBRect::Contains(CBRect const & rect) const {
    return Contains(rect.GetMin()) && Contains(rect.GetMax());
  }

  RectEdge CBRect::ContainsEdgeOf(CBRect const & rect) const {
    for(auto side : {RectEdge::XMin, RectEdge::XMax, RectEdge::YMin, RectEdge::YMax}) {
      if(Contains(rect.GetEdge(side))) {
        return side;
      }
    }
    return RectEdge::None;
  }

  RectCorner CBRect::ContainsCornerOf(CBRect const & rect) const {
    for(auto corner : {RectCorner::Min, RectCorner::Max, RectCorner::MinMax, RectCorner::MaxMin}) {
      if(Contains(rect.GetCorner(corner))) {
        return corner;
      }
    }
    return RectCorner::None;
  }

  glm::vec2 CBRect::GetNormal(RectEdge edge) {
    switch(edge) {
    case RectEdge::XMin:  return {-1.0f, 0.0f};
    case RectEdge::XMax:  return {1.0f, 0.0f};
    case RectEdge::YMin:  return {0.0f, -1.0f};
    case RectEdge::YMax:  return {0.0f, 1.0f};
    default:
    case RectEdge::None:  return {0.0f, 0.0f};
    }
  }

  glm::vec2 CBRect::GetNormal(RectCorner corner) {
    switch(corner) {
    case RectCorner::Min:     return glm::normalize(glm::vec2{-1.0f, -1.0f});
    case RectCorner::MinMax:  return glm::normalize(glm::vec2{-1.0f, 1.0f});
    case RectCorner::Max:     return glm::normalize(glm::vec2{1.0f, 1.0f});
    case RectCorner::MaxMin:  return glm::normalize(glm::vec2{1.0f, -1.0f});
    default:
    case RectCorner::None:    return glm::vec2();
    }
  }
}
