#include "stdafx.h"
#include "GUIWidgetContainer.h"
#include "GUIWidget.h"

namespace gui {
  CWidgetContainer::CWidgetContainer(glm::vec4 const & contentMargin, Align const contentAlign)
    : mContentMargin(contentMargin), mContentAlign(contentAlign) {}

  CWidgetContainer::~CWidgetContainer() {}

  void CWidgetContainer::SetContent(std::unique_ptr<CWidget> content) { mContent = std::move(content); }

  inline CWidget * CWidgetContainer::GetContent() { return mContent.get(); }

  inline const CWidget * CWidgetContainer::GetContent() const { return mContent.get(); }

  inline glm::vec4 const & CWidgetContainer::GetContentMargin() const { return mContentMargin; }

  std::unique_ptr<CWidget> CWidgetContainer::ReleaseContent() { return std::move(mContent); }

  CWidget * CWidgetContainer::FindWidgetById(cb::string const & id) {
    if(mContent && mContent->GetId() == id) {
      return mContent.get();
    }
    return nullptr;
  }

  const CWidget * CWidgetContainer::FindWidgetById(cb::string const & id) const {
    if(mContent && mContent->GetId() == id) {
      return mContent.get();
    }
    return nullptr;
  }
}
