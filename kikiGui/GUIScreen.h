#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <CBGL/Fwd.h>

#include <CoreFwd.h>
#include <GFXFwd.h>

namespace gui {
  class CScreen {
  private:
    std::unique_ptr<gfx::CCanvasView> mCanvasView;
    glm::mat4 mTransform;

  public:
    CScreen(std::shared_ptr<cb::gl::CProgram> guiProgram,
            std::shared_ptr<cb::gl::CTexture> baseTexture,
            std::shared_ptr<cb::gl::CTexture> fontTexture);
    ~CScreen();

    void UpdateRender(gfx::CCanvas const& canvas, glm::vec2 const screenSize);
    void Render() const;
  };
}
