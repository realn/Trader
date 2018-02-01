#pragma once

#include <memory>
#include <vector>

#include <GFXFwd.h>
#include <GFXTextureAtlas.h>

namespace gui {
  class CLayer;
  class CWidget;

  class CLayerStack {
  public:
    using LayersT = std::vector<std::unique_ptr<CLayer>>;

  private:
    LayersT mLayers;
    gfx::CTextureAtlas mTexAtlas;
    std::unique_ptr<gfx::CCanvas> mCanvas;
    glm::vec2 mSize;

  public:
    CLayerStack(gfx::CTextureAtlas const& atlas, glm::vec2 const& size);
    ~CLayerStack();

    gfx::CCanvas const& GetCanvas() const { return *mCanvas; }
    glm::vec2 GetSize() const { return mSize; }

    void Update(float const timeDelta);
    void UpdateRender(core::CFont const& font);

    void Insert(std::unique_ptr<CLayer> layer, size_t pos = 0);
    void Push(std::unique_ptr<CLayer> layer);
    std::unique_ptr<CLayer> Pop();

    virtual CWidget* FindWidgetById(cb::string const& id);
    virtual const CWidget* FindWidgetById(cb::string const& id) const;

    template<typename _Type>
    _Type* FindById(cb::string const& id) {
      return dynamic_cast<_Type*>(FindWidgetById(id));
    }
    template<typename _Type>
    const _Type* FindById(cb::string const& id) const {
      return dynamic_cast<const _Type*>(FindWidgetById(id));
    }
  };
}