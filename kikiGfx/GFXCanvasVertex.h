#pragma once

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <CBGL/VertexDefinition.h>

namespace gfx {
  class CCanvasVertex {
  public:
    glm::vec2 mPos;
    glm::vec2 mTex;
    glm::vec4 mColor = {1.0f, 1.0f, 1.0f, 1.0f};

    CCanvasVertex() {}
    CCanvasVertex(glm::vec2 const& pos, glm::vec2 const& tex, glm::vec4 const& color)
      : mPos(pos), mTex(tex), mColor(color) {}
    CCanvasVertex(float x, float y, float s = 0.0f, float t = 0.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
      : CCanvasVertex(glm::vec2(x, y), glm::vec2(s, t), glm::vec4(r, g, b, a)) {}
    CCanvasVertex(float x, float y, float s, float t, glm::vec4 const& color)
      : CCanvasVertex(glm::vec2(x, y), glm::vec2(s, t), color) {}
    CCanvasVertex(glm::vec2 const& pos, float const s, float const t, glm::vec4 const& color)
      : CCanvasVertex(pos, glm::vec2(s, t), color) {}

    bool operator==(CCanvasVertex const& other) const {
      return mPos == other.mPos && mTex == other.mTex && mColor == other.mColor;
    }
    bool operator!=(CCanvasVertex const& other) const { return !(*this == other); }

    static cb::gl::CVertexDefinition Def;
    static std::map<cb::u32, cb::string> Inputs;
  };
}
