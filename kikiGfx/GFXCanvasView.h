#pragma once

#include <memory>

#include <CBGL/Fwd.h>
#include <CBGL/Buffer.h>

namespace gfx {
  class CCanvas;

  class CCanvasView {
  private:
    std::shared_ptr<cb::gl::CProgram> mProgram;
    std::shared_ptr<cb::gl::CTexture> mBaseTexture;
    std::shared_ptr<cb::gl::CTexture> mFontTexture;
    std::unique_ptr<cb::gl::CBuffer> mVertexBuffer;
    std::unique_ptr<cb::gl::CBuffer> mIndexBuffer;
    size_t mNumberOfVertices = 0;
    size_t mNumberOfIndices = 0;
    cb::u32 mNumberOfRenderIndices = 0;

  public:
    CCanvasView(std::shared_ptr<cb::gl::CProgram> program,
                std::shared_ptr<cb::gl::CTexture> baseTexture,
                std::shared_ptr<cb::gl::CTexture> fontTexture);
    ~CCanvasView();
    
    void UpdateRender(CCanvas const& canvas);
    void Render(glm::mat4 const& transform) const;
  };
}
