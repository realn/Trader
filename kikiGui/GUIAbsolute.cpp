#include "stdafx.h"
#include "GUIAbsolute.h"

namespace gui {
  CAbsolute::CAbsolute(cb::string const & id, glm::vec4 const& contentMargin, gui::Align const contentAlign)
    : CWidget(id)
    , CWidgetContainer(contentMargin, contentAlign)
  {}

  CAbsolute::~CAbsolute() {}

  // Inherited via CWidget

  void CAbsolute::Update(float const timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CAbsolute::UpdateRender(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    mSize = GetMarginSize(spaceSize, mContentMargin);
    if(mContent) {
      mContent->UpdateRender(ctx, mSize);
      auto pos = GetAlignedPos(mContent->GetSize(), mContent->GetSize() * 2.0f, mContentAlign);
      mFinalPosition = mPosition * mSize - pos;
    }
  }

  void CAbsolute::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    if(mContent) {
      mContent->Render(ctx, pos + mFinalPosition);
    }
  }
}
