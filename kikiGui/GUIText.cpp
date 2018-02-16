#include "stdafx.h"

#include <CoreFont.h>
#include <GFXCanvas.h>

#include "GUIContext.h"
#include "GUIText.h"

namespace gui {
  CText::CText(cb::string const & id, cb::string const & text, glm::vec4 const & textColor, Align const align)
    : CLabel(id, text, textColor, align)
  {}

  CText::~CText() {}

  void CText::UpdateRender(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    using namespace glm;

    mSize = spaceSize;
    if(mOldText != mText) {
      mLines.clear();
      auto lineH = ctx.Font.GetLineHeight() * TEXT_SCALE * mTextScale.y * ctx.TextScale.y;
      auto lines = cb::split(mText, L"\n"s, false);
      auto pos = vec2(0.0f, spaceSize.y - lineH);
      for(auto& line : lines) {
        mLines.push_back({ pos, line });
        pos.y -= lineH;
      }
      mOldText = mText;
    }
  }

  void CText::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    for(auto& line : mLines) {
      ctx.Canvas.Print(pos + line.mPos,
                       ctx.Font, line.mText, mTextColor,
                       glm::vec2(TEXT_SCALE) * mTextScale * ctx.TextScale);
    }
  }
}
