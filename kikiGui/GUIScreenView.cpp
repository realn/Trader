#include "stdafx.h"
#include "GUIScreenView.h"
#include "GUILayer.h"
#include "GFXCanvas.h"
#include "GFXCanvasView.h"
#include "GFXTextureAtlas.h"

#include <CoreFont.h>
#include <CoreAssetRepository.h>

#include <CBGL/Texture.h>

namespace gui {
  CScreen::CScreen(std::shared_ptr<core::CFont> font,
                           std::shared_ptr<cb::gl::CProgram> guiProgram,
                           gfx::CTextureAtlas const& textureAtlas, 
                           core::CAssetRepository<cb::gl::CTexture>& texRepo)
    : mFont(font), mTextureAtlas(textureAtlas), mTransform(1.0f)
  {
    auto fontTexture = texRepo.Load(font->GetTextureFilePath());
    auto baseTexture = texRepo.Load(textureAtlas.GetTextureFileName());

    mCanvasView = std::make_unique<gfx::CCanvasView>(guiProgram, baseTexture, fontTexture);
  }

  CScreen::~CScreen() {}

  gfx::CCanvas CScreen::CreateCanvas() const {
    return gfx::CCanvas(mTextureAtlas);
  }

  void CScreen::UpdateRender(CLayer const& screen, gfx::CCanvas const& canvas) {
    auto screenSize = screen.GetSize();
    mTransform = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y);
    mCanvasView->UpdateRender(canvas);
  }

  void gui::CScreen::Render() const {
    mCanvasView->Render(mTransform);
  }
}
