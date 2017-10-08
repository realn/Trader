#include "stdafx.h"
#include "GFXCanvasView.h"
#include "GFXCanvas.h"
#include "GFXVertex.h"
#include "GFXConsts.h"

#include <CBGL/State.h>
#include <CBGL/Texture.h>
#include <CBGL/Program.h>

namespace gfx {
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
    auto gtex0 = cb::gl::bind(*mBaseTexture, 0);
    auto gtex1 = cb::gl::bind(*mFontTexture, 1);
    auto gvbuf = cb::gl::bind(*mVertexBuffer);
    auto gdef = cb::gl::bind(gfx::CVertex::Def);

    mProgram->SetUniform(gfx::UNI_TRANSFORM, transform);
    mProgram->SetUniform(gfx::UNI_BASE_TEXTURE, 0);
    mProgram->SetUniform(gfx::UNI_FONT_TEXTURE, 1);

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
