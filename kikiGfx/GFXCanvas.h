#pragma once

#include <memory>
#include <CBGL/Fwd.h>
#include <CoreFwd.h>
#include <CoreBRect.h>

#include "GFXConsts.h"
#include "GFXCanvasVertex.h"
#include "GFXTextureAtlas.h"

namespace gfx {
  class CCanvas {
  private:
    std::vector<CCanvasVertex> mVertices;
    std::vector<cb::u16> mIndices;
    CTextureAtlas mTextureAtlas;

  public:
    CCanvas(CTextureAtlas const& textureAtlas);
    ~CCanvas();

    void SetTextureAtlas(CTextureAtlas const& atlas) { mTextureAtlas = atlas; }

    std::vector<CCanvasVertex> const& GetVertices() const { return mVertices; }
    std::vector<cb::u16> const& GetIndices() const { return mIndices; }
    CTextureAtlas const& GetTextureAtlas() const { return mTextureAtlas; }

    void DrawPoint(glm::vec2 const& pos,
                   float const size = 0.01f,
                   glm::vec4 const& color = glm::vec4(1.0f));
    void DrawLine(glm::vec2 const& posA,
                  glm::vec2 const& posB,
                  float const width = 0.01f,
                  glm::vec4 const& color = glm::vec4(1.0f));
    void DrawTriangle(glm::vec2 const& posA, glm::vec2 const& posB, glm::vec2 const& posC,
                      glm::vec4 const& color = glm::vec4(1.0f));
    void DrawRect(glm::vec2 const& pos, glm::vec2 const& size, glm::vec4 const& color = glm::vec4(1.0f));
    void DrawRect(glm::vec2 const& pos,
                  glm::vec2 const& size,
                  cb::string const& imgName,
                  glm::vec4 const& color = glm::vec4(1.0f),
                  core::RectFlip const imgFlip = core::RectFlip::None);
    void DrawRect(core::CBRect const& rect, glm::vec4 const& color = glm::vec4(1.0f));
    void DrawRect(core::CBRect const& rect, core::CBRect const& texRect, glm::vec4 const& color = glm::vec4(1.0f));
    void Print(glm::vec2 const& tpos, core::CFont const& font, cb::string const& text, 
               glm::vec4 const& color, glm::vec2 const& scale = glm::vec2(1.0f));

    void Clear();

  private:
    void internalDrawRect(core::CBRect const& prect, 
                          core::CBRect const& trect, 
                          glm::vec4 const& color,
                          core::RectFlip const flipPRect = core::RectFlip::None,
                          core::RectFlip const flipTRect = core::RectFlip::None);
    void AddVertex(glm::vec2 const& pos, glm::vec2 const& tex, glm::vec4 const& color);
    glm::u16 AddVertexFast(glm::vec2 const& pos, glm::vec2 const& tex, glm::vec4 const& color);
    glm::u16 AddVertexFast(CCanvasVertex&& vertex);
    void AddIndex(glm::u16 const index);
  };
}
