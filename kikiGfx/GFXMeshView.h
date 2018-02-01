#pragma once

#include <CBGL/Fwd.h>
#include <CBGL/Buffer.h>

namespace gfx {
  class CMesh;
  class CMeshView {
  private:
    cb::gl::CBuffer mVertexBuffer;
    cb::gl::CBuffer mIndexBuffer;
    cb::u32 mNumberOfIndices;
    cb::gl::PrimitiveType mPrimitiveType;

  public:
    CMeshView();
    CMeshView(CMesh const& mesh);

    void Set(CMesh const& mesh);
    void Update(CMesh const& mesh);

    void Bind() const;
    void UnBind() const;

    void Render();
  };
}