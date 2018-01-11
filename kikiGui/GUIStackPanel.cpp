#include "stdafx.h"
#include "GUIStackPanel.h"
#include "GUIContext.h"

namespace gui {
  void CStackPanel::AddWidget(std::unique_ptr<CWidget> widget, Align const align) {
    mItems.push_back({glm::vec2(), std::move(widget), align});
  }

  void CStackPanel::Update(float const timeDelta) {
    for(auto& item : mItems) {
      item.Widget->Update(timeDelta);
    }
  }

  void CStackPanel::UpdateRender(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    CWidget::UpdateRender(ctx, spaceSize);
    auto fullSpace = GetMarginSize(spaceSize, mContentMargin);
    auto spaceLeft = fullSpace;
    auto ori = (mOrientation == Orientation::Horizontal) ? glm::vec2(1.0f, 0.0f) : glm::vec2(0.0f, 1.0f);
    auto nori = glm::vec2(1.0f) - ori;

    auto pos = glm::vec2();
    for(auto& item : mItems) {
      item.Widget->UpdateRender(ctx, spaceLeft);

      auto itemSpaceSize = item.Widget->GetSize();

      spaceLeft -= itemSpaceSize * ori;
      itemSpaceSize = spaceLeft * nori + itemSpaceSize * ori;
      
      item.Pos = pos + GetAlignedPos(item.Widget->GetSize(), itemSpaceSize, item.Align);
      pos += itemSpaceSize * ori;
    }

    mAlignedPos = GetMarginPos(mContentMargin) + 
      GetAlignedPos(fullSpace - spaceLeft * ori, fullSpace, mContentAlign);
  }

  void CStackPanel::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    for(auto& item : mItems) {
      item.Widget->Render(ctx, pos + mAlignedPos + item.Pos);
    }
  }

  CWidget * CStackPanel::FindWidgetById(cb::string const & id) {
    auto result = CWidgetListContainer::FindWidgetById(id);
    if(result) {
      return result;
    }
    return CWidget::FindWidgetById(id);
  }

  const CWidget * CStackPanel::FindWidgetById(cb::string const & id) const {
    auto result = CWidgetListContainer::FindWidgetById(id);
    if(result) {
      return result;
    }
    return CWidget::FindWidgetById(id);
  }
}
