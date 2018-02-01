#pragma once

#include "GUIWidget.h"
#include "GUIWidgetContainer.h"

namespace gui {
  class CAbsolute 
    : public CWidget
    , public CWidgetContainer
  {
  protected:
    glm::vec2 mPosition;
    glm::vec2 mFinalPosition;

  public:
    CAbsolute(cb::string const& id = cb::string(), 
              glm::vec4 const& contentMargin = glm::vec4(0.0f), 
              gui::Align const contentAlign = gui::Align::Default);
    virtual ~CAbsolute();

    void SetPosition(glm::vec2 const& value) { mPosition = value; }
    glm::vec2 GetPosition() const { return mPosition; }

    // Inherited via CWidget
    virtual void Update(float const timeDelta) override;
    virtual void UpdateRender(CUpdateContext const& ctx, glm::vec2 const& spaceSize) override;
    virtual void Render(CRenderContext & ctx, glm::vec2 const & pos) const override;

  };
}