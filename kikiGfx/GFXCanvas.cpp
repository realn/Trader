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

  void CCanvas::DrawRect(glm::vec2 const & pos, 
                         glm::vec2 const & size, 
                         cb::string const & imgName, 
                         glm::vec4 const & color,
                         core::RectFlip const imgFlip) {
    using namespace core;

    auto tex = mTextureAtlas[imgName];
    auto prect = CBRect(pos, size);
    auto trect = CBRect(tex.TexMin, tex.TexMax - tex.TexMin);
    auto rcol = glm::clamp(color, 0.0f, 1.0f) + glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    internalDrawRect(prect, trect, rcol, core::RectFlip::None, imgFlip);
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
    auto vcoords = std::vector<ivec2>{
      { 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 }
    };

    auto pos = tpos;
    auto col = vec4(color.r, color.g, color.b, color.a + 2.0f);

    for(auto& item : text) {
      auto& glyph = font.GetChar(item);

      auto i = static_cast<cb::u16>(mVertices.size());

      for(auto& coord : vcoords) {
        AddVertexFast(glyph.getVPos(coord, scale) + pos,
                      glyph.getVTex(coord),
                      col);
      }

      for(auto& item : idx) { 
        AddIndex(i + item);
      }

      pos += glyph.mAdv * scale;
    }
  }

  void CCanvas::Clear() {
    mVertices.clear();
    mIndices.clear();
  }

  void CCanvas::internalDrawRect(core::CBRect const & prect, 
                                 core::CBRect const & trect, 
                                 glm::vec4 const & color,
                                 core::RectFlip const flipPRect,
                                 core::RectFlip const flipTRect) {
    auto coords = std::vector<glm::uvec2>{
      {0, 0}, {1, 0}, {1, 1}, 
      {0, 0}, {1, 1}, {0, 1}
    };

    for(auto& coord : coords) {
      AddVertex(prect.GetUCrd(coord, flipPRect), trect.GetUCrd(coord, flipTRect), color);
    }
  }

  void CCanvas::AddVertex(glm::vec2 const & pos, glm::vec2 const & tex, glm::vec4 const & color) {
    auto vert = CCanvasVertex(pos, tex, color);
    auto it = std::find(mVertices.begin(), mVertices.end(), vert);
    if(it != mVertices.end()) {
      AddIndex(static_cast<cb::u16>(it - mVertices.begin()));
    }
    else {
      auto index = AddVertexFast(std::move(vert));
      AddIndex(index);
    }
  }

  glm::u16 CCanvas::AddVertexFast(glm::vec2 const & pos, glm::vec2 const & tex, glm::vec4 const & color) {
    return AddVertexFast({ pos, tex, color });
  }

  glm::u16 CCanvas::AddVertexFast(CCanvasVertex && vertex) {
    if(mVertices.size() + 1 > mVertices.capacity()) {
      mVertices.reserve(mVertices.capacity() + 200);
    }
    mVertices.push_back(vertex);
    return static_cast<cb::u16>(mVertices.size() - 1);
  }

  void CCanvas::AddIndex(glm::u16 const index) {
    if(mIndices.size() + 1 > mIndices.capacity()) {
      mIndices.reserve(mIndices.capacity() + 200);
    }
    mIndices.push_back(index);
  }
}
