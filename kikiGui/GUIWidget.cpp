#include "stdafx.h"
#include "GUIWidget.h"

namespace gui {
  cb::string CWidget::NoId = cb::string();

  void CWidget::UpdateRender(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    if(glm::all(glm::equal(mFixedSize, glm::vec2(0.0f)))) {
      mSize = spaceSize;
    }
    else {
      mSize = glm::clamp(mFixedSize, glm::vec2(0.0f), spaceSize);
    }
  }

  glm::vec2 CWidget::GetAlignedPos(glm::vec2 const & size, glm::vec2 const & spaceSize, Align const align) {
    auto val = glm::vec2(0.0f);
    auto spaceLeft = spaceSize - size;

    if(isTrueAnd(align, Align::Center)) { val.x = 0.5f; }
    else if(isTrueAnd(align, Align::Right)) { val.x = 1.0f; }

    if(isTrueAnd(align, Align::Middle)) { val.y = 0.5f; }
    else if(isTrueAnd(align, Align::Top)) { val.y = 1.0f; }

    return spaceLeft * val;
  }

  glm::vec2 CWidget::GetMarginPos(glm::vec4 const & margin) {
    return glm::vec2(margin.x, margin.y);
  }

  glm::vec2 CWidget::GetMarginSize(glm::vec2 const& size, glm::vec4 const & margin) {
    return size - glm::vec2(margin.x + margin.z, margin.y + margin.w);
  }
}
