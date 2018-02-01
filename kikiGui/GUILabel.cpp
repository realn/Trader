#include "stdafx.h"
#include "GUILabel.h"
#include "GUIContext.h"
#include "GUIConsts.h"
#include "GFXCanvas.h"

#include <CoreFont.h>

namespace gui {
  void CLabel::UpdateRender(CUpdateContext const& ctx, glm::vec2 const & spaceSize) {
    mSize = ctx.Font.GetTextSize(mText, true) * glm::vec2(TEXT_SCALE) * mTextScale * ctx.TextScale;
    if(!glm::all(glm::equal(mFixedSize, glm::vec2(0.0f)))) {
      mSize = glm::clamp(glm::max(mSize, mFixedSize), glm::vec2(0.0f), spaceSize);
    }
    mTextAlignPos = GetAlignedPos(mSize, spaceSize, mTextAlign);
  }

  void CLabel::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    ctx.Canvas.Print(pos + mTextAlignPos,
                     ctx.Font, mText, mTextColor,
                     glm::vec2(TEXT_SCALE) * mTextScale * ctx.TextScale);
  }
}
