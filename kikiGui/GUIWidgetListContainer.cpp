#include "stdafx.h"
#include "GUIWidgetListContainer.h"
#include "GUIWidget.h"

#include <algorithm>

namespace gui {
  CWidgetListItem::~CWidgetListItem() {}

  CWidgetListContainer::~CWidgetListContainer() {}

  CWidget * CWidgetListContainer::FindWidgetById(cb::string const & id) {
    auto it = std::find_if(begin(), end(), [id](auto& item) -> auto { return item.Widget->GetId() == id; });
    if(it != end()) {
      return it->Widget.get();
    }
    return nullptr;
  }

  const CWidget * CWidgetListContainer::FindWidgetById(cb::string const & id) const {
    auto it = std::find_if(begin(), end(), [id](auto& item) -> auto { return item.Widget->GetId() == id; });
    if(it != end()) {
      return it->Widget.get();
    }
    return nullptr;
  }
}
