#pragma once

#include <memory>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <CoreFwd.h>
#include <GFXFwd.h>

#include "GUIWidgetContainer.h"

namespace gui {
  class CLayer 
    : public CWidgetContainer
  {
  private:
    glm::vec2 mSize;
    glm::vec2 mTextScale;

  public:
    CLayer(glm::vec2 const& size = glm::vec2(), 
            glm::vec4 const& contentMargin = glm::vec4(), 
            Align const contentAlign = Align::Default);
    CLayer(CLayer&&) = default;
    virtual ~CLayer();

    void SetSize(glm::vec2 const& value) { mSize = value; }
    void SetTextScale(glm::vec2 const& value) { mTextScale = value; }

    glm::vec2 GetSize() const { return mSize; }
    glm::vec2 GetTextScale() const { return mTextScale; }
    float GetAspectRation() const { return mSize.x / mSize.y; }

    void Update(float const timeDelta);
    void UpdateRender(core::CFont const& font);
    void Render(gfx::CCanvas& canvas, core::CFont const& font) const;

    template<typename _Type>
    _Type* FindById(cb::string const& id) {
      return dynamic_cast<_Type*>(FindWidgetById(id));
    }
    template<typename _Type>
    const _Type* FindById(cb::string const& id) const {
      return dynamic_cast<const _Type*>(FindWidgetById(id));
    }

    static CLayer Load(cb::string const& filepath);
  };
}