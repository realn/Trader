#include "stdafx.h"
#include "GFXCanvas.h"
#include "GFXCanvasVertex.h"
#include <CoreFont.h>
#include <CoreBRect.h>

namespace gfx {
  CCanvas::CCanvas(CTextureAtlas const& textureAtlas)
    : mTextureAtlas(textureAtlas)
  {}

  CCanvas::~CCanvas() {}

  void CCanvas::DrawPoint(glm::vec2 const & pos, float const size, glm::vec4 const & color) {
    using namespace core;

    auto half = glm::vec2(size) / 2.0f;
    auto tex = glm::vec2();
    internalDrawRect(CBRect(pos - half, glm::vec2(size)), CBRect(), glm::clamp(color, 0.0f, 1.0f));
  }

  void CCanvas::DrawLine(glm::vec2 const & posA, glm::vec2 const & posB, float const width, glm::vec4 const & color) {
    throw std::exception("Unimplemented");
  }

  void CCanvas::DrawTriangle(glm::vec2 const & posA, glm::vec2 const & posB, glm::vec2 const & posC, glm::vec4 const & color) {
    auto tex = glm::vec2();
    AddVertex(posA, tex, color);
    AddVertex(posB, tex, color);
    AddVertex(posC, tex, color);
  }

  void CCanvas::DrawRect(glm::vec2 const & pos, glm::vec2 const & size, glm::vec4 const & color) {
    internalDrawRect(core::CBRect(pos, size), core::CBRect(), glm::clamp(color, 0.0f, 1.0f));
  }

  void CCanvas::DrawRect(glm::vec2 const & pos, glm::vec2 const & size, cb::string const & imgName, glm::vec4 const & color) {
    using namespace core;

    auto tex = mTextureAtlas[imgName];
    auto prect = CBRect(pos, size);
    auto trect = CBRect(tex.TexMin, tex.TexMax - tex.TexMin);

    internalDrawRect(CBRect(pos, size),
                     CBRect(tex.TexMin, tex.TexMax - tex.TexMin),
                     glm::clamp(color, 0.0f, 1.0f) + glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }

  void CCanvas::DrawRect(core::CBRect const & rect, glm::vec4 const & color) {
    internalDrawRect(rect, core::CBRect(), color);
  }

  void CCanvas::DrawRect(core::CBRect const & rect, core::CBRect const & texRect, glm::vec4 const & color) {
    internalDrawRect(rect, texRect, glm::clamp(color, 0.0f, 1.0f) + glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }

  void CCanvas::Print(glm::vec2 const& tpos, 
                      core::CFont const & font, 
                      cb::string const & text, 
                      glm::vec4 const& color,
                      glm::vec2 const& scale) {
    using namespace glm;

    auto idx = {0, 1, 2, 0, 2, 3};
    auto pos = tpos;
    auto col = vec4(color.r, color.g, color.b, color.a + 2.0f);
    auto vcoords = std::vector<ivec2>{
      {0, 0}, {1, 0}, {1, 1}, {0, 1}
    };

    for(auto& item : text) {
      auto& glyph = font.GetChar(item);

      auto i = static_cast<cb::u16>(mVertices.size());

      for(auto& coord : vcoords) {
        AddVertexFast(glyph.getVPos(coord, scale) + pos,
                      glyph.getVTex(coord),
                      col);
      }

      for(auto& item : idx) { mIndices.push_back(static_cast<cb::u16>(i + item)); }

      pos += glyph.mAdv * scale;
    }
  }

  void CCanvas::Clear() {
    mVertices.clear();
    mIndices.clear();
  }

  void CCanvas::internalDrawRect(core::CBRect const & prect, core::CBRect const & trect, glm::vec4 const & color) {
    AddVertex(prect.GetUCrd(0, 0), trect.GetUCrd(0, 0), color);
    AddVertex(prect.GetUCrd(1, 0), trect.GetUCrd(1, 0), color);
    AddVertex(prect.GetUCrd(1, 1), trect.GetUCrd(1, 1), color);

    AddVertex(prect.GetUCrd(0, 0), trect.GetUCrd(0, 0), color);
    AddVertex(prect.GetUCrd(1, 1), trect.GetUCrd(1, 1), color);
    AddVertex(prect.GetUCrd(0, 1), trect.GetUCrd(0, 1), color);
  }

  void CCanvas::AddVertex(glm::vec2 const & pos, glm::vec2 const & tex, glm::vec4 const & color) {
    auto vert = CCanvasVertex(pos, tex, color);
    auto it = std::find(mVertices.begin(), mVertices.end(), vert);
    if(it != mVertices.end()) {
      mIndices.push_back(static_cast<cb::u16>(it - mVertices.begin()));
    }
    else {
      auto index = mVertices.size();
      mVertices.push_back(vert);
      mIndices.push_back(static_cast<cb::u16>(index));
    }
  }

  void CCanvas::AddVertexFast(glm::vec2 const & pos, glm::vec2 const & tex, glm::vec4 const & color) {
    mVertices.push_back({ pos, tex, color });
  }
}