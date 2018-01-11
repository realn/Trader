#include "stdafx.h"

#include <CBGL/Texture.h>

#include "GFXCanvasView.h"
#include "GUIScreen.h"

namespace gui {
  CScreen::CScreen(std::shared_ptr<cb::gl::CProgram> guiProgram,
                   std::shared_ptr<cb::gl::CTexture> baseTexture,
                   std::shared_ptr<cb::gl::CTexture> fontTexture)
    : mTransform(1.0f)
  {
    mCanvasView = std::make_unique<gfx::CCanvasView>(guiProgram, baseTexture, fontTexture);
  }

  CScreen::~CScreen() {}

  void CScreen::UpdateRender(gfx::CCanvas const& canvas, glm::vec2 const screenSize) {
    mTransform = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y);
    mCanvasView->UpdateRender(canvas);
  }

  void gui::CScreen::Render() const {
    mCanvasView->Render(mTransform);
  }
}
