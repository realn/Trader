#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <CBGL/Fwd.h>

#include <CoreFwd.h>
#include <GFXFwd.h>
#include <GFXTextureAtlas.h>

namespace gui {
  class CScreen;

  class CScreenView {
  private:
    std::shared_ptr<core::CFont> mFont;
    std::unique_ptr<gfx::CCanvasView> mCanvasView;
    gfx::CTextureAtlas mTextureAtlas;
    glm::mat4 mTransform;

  public:
    CScreenView(std::shared_ptr<core::CFont> font,
                std::shared_ptr<cb::gl::CProgram> guiProgram,
                gfx::CTextureAtlas const& textureAtlas,
                core::CAssetRepository<cb::gl::CTexture>& texRepo);
    ~CScreenView();

    gfx::CCanvas CreateCanvas() const;

    core::CFont const& GetFont() const { return *mFont; }
    gfx::CTextureAtlas const& GetTextureAtlas() const { return mTextureAtlas; }

    void UpdateRender(CScreen const& screen, gfx::CCanvas const& canvas);
    void Render() const;
  };
}
