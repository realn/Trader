#include "stdafx.h"

#include <GFXCanvas.h>

#include "GUIContext.h"
#include "GUIImage.h"

namespace gui {
  CImage::CImage(cb::string const & id, cb::string const & imgName, glm::vec4 const & imgColor)
    : CRect(id, imgColor)
    , mImgName(imgName)
  {}

  CImage::~CImage() {}

  void CImage::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    if(mBackColor.a != 0.0f) {
      ctx.Canvas.DrawRect(pos, mSize, mImgName, mBackColor);
    }
  }
}
