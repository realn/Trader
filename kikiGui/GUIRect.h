#pragma once

#include <glm/vec4.hpp>

#include "GUIWidget.h"

namespace gui {
  class CRect 
    : public CWidget
  {
  protected:
    glm::vec4 mBackColor;

  public:
    CRect(cb::string const& id, 
          glm::vec4 const& backColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) 
      : CWidget(id), mBackColor(backColor) {}
    CRect(CRect&&) = default;
    virtual ~CRect() {}

    void SetBackColor(glm::vec4 const& backColor) { mBackColor = backColor; }
    glm::vec4 const& GetBackColor() const { return mBackColor; }

    virtual void Render(CRenderContext& ctx, glm::vec2 const& pos) const override;
  };
}