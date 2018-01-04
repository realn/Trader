#pragma once

#include <glm\vec2.hpp>

#include <CoreFwd.h>
#include <GFXFwd.h>

namespace gui {
  struct CUpdateContext {
    core::CFont const& Font;
    glm::vec2 const TextScale;

    CUpdateContext(core::CFont const& font, 
                   glm::vec2 const textScale) : Font(font), TextScale(textScale) {}
  };

  struct CRenderContext {
    core::CFont const& Font;
    gfx::CCanvas& Canvas;
    glm::vec2 const TextScale;

    CRenderContext(core::CFont const& font, 
                   gfx::CCanvas& canvas,
                   glm::vec2 const textScale) : Font(font), Canvas(canvas), TextScale(textScale) {}
  };
}