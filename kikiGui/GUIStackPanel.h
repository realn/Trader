#pragma once

#include <memory>

#include "GUIWidget.h"
#include "GUIWidgetListContainer.h"

namespace gui {
  class CStackPanel 
    : public CWidget
    , public CWidgetListContainer
  {
  protected:
    Orientation mOrientation;
    glm::vec2 mAlignedPos;

  public:
    CStackPanel(cb::string const& id, Orientation const orientation = Orientation::Horizontal)
      : CWidget(id), mOrientation(orientation) {}
    CStackPanel(CStackPanel&&) = default;
    virtual ~CStackPanel() {}

    void SetOrientation(Orientation const value) { mOrientation = value; }
    Orientation GetOrientation() const { return mOrientation; }

    void AddWidget(std::unique_ptr<CWidget> widget, Align const align = Align::Default);

    virtual void Update(float const timeDelta) override;
    virtual void UpdateRender(CUpdateContext const& ctx, glm::vec2 const& spaceSize) override;
    virtual void Render(CRenderContext& ctx, glm::vec2 const& pos) const override;

    virtual CWidget* FindWidgetById(cb::string const& id) override;
    virtual const CWidget* FindWidgetById(cb::string const& id) const override;

  private:
    ItemListT& GetItemsByAlign();
  };
}
