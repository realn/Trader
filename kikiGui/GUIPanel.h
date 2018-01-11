#pragma once

#include <memory>

#include "GUIRect.h"
#include "GUIWidgetContainer.h"

namespace gui {
  class CPanel 
    : public CRect
    , public CWidgetContainer
  {
  protected:
    glm::vec2 mContentPos;

  public:
    CPanel(cb::string const& id, 
           glm::vec4 const& backColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
           glm::vec4 const& contentMargin = glm::vec4(0.0f),
           Align const contentAlign = Align::Default)
      : CRect(id, backColor), CWidgetContainer(contentMargin, contentAlign) {}
    CPanel(CPanel&&) = default;
    virtual ~CPanel() {}

    virtual void Update(float const timeDelta);
    virtual void UpdateWidget(CUpdateContext const& ctx, glm::vec2 const& spaceSize);
    virtual void Render(CRenderContext& ctx, glm::vec2 const& pos) const;

    virtual CWidget* FindWidgetById(cb::string const& id) override;
    virtual const CWidget* FindWidgetById(cb::string const& id) const override;
  };
}
