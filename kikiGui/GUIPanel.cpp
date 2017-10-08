#include "stdafx.h"
#include "GUIPanel.h"

namespace gui {
  void CPanel::Update(float const timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CPanel::UpdateWidget(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    CRect::UpdateWidget(ctx, spaceSize);

    if(mContent) {
      auto marginSize = GetMarginSize(spaceSize, mContentMargin);
      mContent->UpdateWidget(ctx, marginSize);
      mContentPos = GetMarginPos(mContentMargin) + 
        GetAlignedPos(mContent->GetSize(), marginSize, mContentAlign);
    }
  }

  void CPanel::UpdateRender(CRenderContext & ctx, glm::vec2 const & pos) const {
    CRect::UpdateRender(ctx, pos);

    if(mContent) {
      mContent->UpdateRender(ctx, pos + mContentPos);
    }
  }

  CWidget * CPanel::FindWidgetById(cb::string const & id) {
    auto widget = CWidgetContainer::FindWidgetById(id);
    if(widget) {
      return widget;
    }
    return CWidget::FindWidgetById(id);
  }

  const CWidget * CPanel::FindWidgetById(cb::string const & id) const {
    auto widget = CWidgetContainer::FindWidgetById(id);
    if(widget) {
      return widget;
    }
    return CWidget::FindWidgetById(id);
  }
}

