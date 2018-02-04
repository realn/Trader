#include "stdafx.h"
#include "GUIText.h"

namespace gui {
  CText::CText(cb::string const & id, cb::string const & text, glm::vec4 const & textColor, Align const align)
    : CLabel(id, text, textColor, align)
  {}

  CText::~CText() {}

  void CText::UpdateRender(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    if(mOldText != mText) {
      mLines.clear();
      auto words = cb::split(mText, L" "s, true);

      auto line = cb::strvector();
      for(auto& word : words) {

      }
    }
  }

  void CText::Render(CRenderContext & ctx, glm::vec2 const & pos) const {}
}
