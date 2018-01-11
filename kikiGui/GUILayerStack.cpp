#include "stdafx.h"

#include <GFXCanvas.h>

#include "GUILayer.h"
#include "GUILayerStack.h"

namespace gui {
  CLayerStack::CLayerStack(gfx::CTextureAtlas const & atlas) 
    : mTexAtlas(atlas)
  {
    mCanvas = std::make_unique<gfx::CCanvas>(mTexAtlas);
  }

  CLayerStack::~CLayerStack() {}


  void CLayerStack::Update(float const timeDelta) {
    for(auto& layer : mLayers) {
      layer->Update(timeDelta);
    }
  }

  void CLayerStack::UpdateRender(core::CFont const& font) {
    mCanvas->Clear();
    for(auto it = mLayers.rbegin(); it != mLayers.rend(); it++) {
      (*it)->UpdateRender(font);
      (*it)->Render(*mCanvas, font);
    }
  }

  void CLayerStack::Insert(std::unique_ptr<CLayer> layer, size_t pos) {
    mLayers.insert(mLayers.begin() + pos, std::move(layer));
  }

  void CLayerStack::Push(std::unique_ptr<CLayer> layer) {
    mLayers.push_back(std::move(layer));
  }

  std::unique_ptr<CLayer> CLayerStack::Pop() {
    if(mLayers.empty()) {
      return std::unique_ptr<CLayer>();
    }
    auto layer = std::move(*mLayers.rbegin());
    mLayers.pop_back();
    return layer;
  }

}
