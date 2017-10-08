#pragma once

#include <memory>
#include <glm/vec4.hpp>

#include "GUIConsts.h"

namespace gui {
  class CWidget;

  class CWidgetContainer {
  protected:
    std::unique_ptr<CWidget> mContent;
    glm::vec4 mContentMargin;
    Align mContentAlign;

  public:
    CWidgetContainer(glm::vec4 const& contentMargin, Align const contentAlign);
    CWidgetContainer(CWidgetContainer&&) = default;
    virtual ~CWidgetContainer();

    void SetContent(std::unique_ptr<CWidget> content);
    void SetContentMargin(glm::vec4 const& margin) { mContentMargin = margin; }
    void SetContentAlign(Align const align) { mContentAlign = align; }

    CWidget* GetContent();
    const CWidget* GetContent() const;
    glm::vec4 const& GetContentMargin() const;
    Align GetContentAlign() const { return mContentAlign; }

    std::unique_ptr<CWidget> ReleaseContent();

    virtual CWidget* FindWidgetById(cb::string const& id);
    virtual const CWidget* FindWidgetById(cb::string const& id) const;
  };
}
