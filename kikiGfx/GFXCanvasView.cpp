#include "stdafx.h"
#include "GFXCanvasView.h"
#include "GFXCanvas.h"
#include "GFXCanvasVertex.h"
#include "GFXConsts.h"

#include <CBGL/State.h>
#include <CBGL/Texture.h>
#include <CBGL/Program.h>

namespace gfx {
  auto constexpr TEX_IDX_CANVAS_BASE = 0;
  auto constexpr TEX_IDX_CANVAS_FONT = 1;

  auto const TEX_IN_CANVAS_BASE = L"texBase"s;
  auto const TEX_IN_CANVAS_FONT = L"texFont"s;

  auto const MIN_CANVAS_TRANSFORM = L"mTransform"s;

  CCanvasView::CCanvasView(std::shared_ptr<cb::gl::CProgram> program, 
                           std::shared_ptr<cb::gl::CTexture> baseTexture, 
                           std::shared_ptr<cb::gl::CTexture> fontTexture) 
    : mProgram(program), mBaseTexture(baseTexture), mFontTexture(fontTexture) 
  {
    mVertexBuffer = std::make_unique<cb::gl::CBuffer>();
    mIndexBuffer = std::make_unique<cb::gl::CBuffer>(cb::gl::BufferTarget::ELEMENT_ARRAY);
  }

  CCanvasView::~CCanvasView() {}
  
  void CCanvasView::UpdateRender(CCanvas const& canvas) {
    if(mNumberOfVertices < canvas.GetVertices().size()) {
      mVertexBuffer->SetData(canvas.GetVertices());
      mNumberOfVertices = canvas.GetVertices().size();
    }
    else {
      mVertexBuffer->SetSubData(canvas.GetVertices());
    }
    
    if(mNumberOfIndices < canvas.GetIndices().size()) {
      mIndexBuffer->SetData(canvas.GetIndices());
      mNumberOfIndices = canvas.GetIndices().size();
    }
    else {
      mIndexBuffer->SetSubData(canvas.GetIndices());
    }

    mNumberOfRenderIndices = static_cast<cb::u32>(canvas.GetIndices().size());
  }

  void CCanvasView::Render(glm::mat4 const & transform) const {
    if(mNumberOfIndices == 0 || mNumberOfVertices == 0) {
      return;
    }

    auto gprog = cb::gl::bind(*mProgram);
    auto gtex0 = cb::gl::bind(*mBaseTexture, TEX_IDX_CANVAS_BASE);
    auto gtex1 = cb::gl::bind(*mFontTexture, TEX_IDX_CANVAS_FONT);
    auto gvbuf = cb::gl::bind(*mVertexBuffer);
    auto gdef = cb::gl::bind(gfx::CCanvasVertex::Def);

    mProgram->SetUniform(MIN_CANVAS_TRANSFORM, transform);
    mProgram->SetUniform(TEX_IN_CANVAS_BASE, TEX_IDX_CANVAS_BASE);
    mProgram->SetUniform(TEX_IN_CANVAS_FONT, TEX_IDX_CANVAS_FONT);

    {
      auto blend = cb::gl::CBlendState();
      blend.SrcFactor = cb::gl::BlendFactor::SRC_ALPHA;
      blend.DstFactor = cb::gl::BlendFactor::ONE_MINUS_SRC_ALPHA;
      cb::gl::setState(blend);
    }

    auto gstate = cb::gl::bindStateEnabled(cb::gl::State::BLEND, true);
    auto gibuf = cb::gl::bind(*mIndexBuffer);
    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, mNumberOfRenderIndices);
  }
}
