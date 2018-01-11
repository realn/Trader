#pragma once

#include <memory>
#include <vector>

#include <GFXFwd.h>
#include <GFXTextureAtlas.h>

namespace gui {
  class CLayer;

  class CLayerStack {
  public:
    using LayersT = std::vector<std::unique_ptr<CLayer>>;

  private:
    LayersT mLayers;
    gfx::CTextureAtlas mTexAtlas;
    std::unique_ptr<gfx::CCanvas> mCanvas;

  public:
    CLayerStack(gfx::CTextureAtlas const& atlas);

    gfx::CCanvas const& GetCanvas() const { return *mCanvas; }

    void Update(float const timeDelta);
    void UpdateRender(core::CFont const& font);

    void Insert(std::unique_ptr<CLayer> layer, size_t pos = 0);
    void Push(std::unique_ptr<CLayer> layer);
    std::unique_ptr<CLayer> Pop();
  };
}