#include "stdafx.h"
#include "GUILayer.h"
#include "GUIWidget.h"
#include "GUIContext.h"
#include "GFXCanvas.h"
#include "GUIXml.h"

#include <CBXml/Document.h>
#include <CBCore/File.h>

namespace gui {
  CLayer::CLayer(glm::vec4 const& contentMargin, Align const contentAlign)
    : CWidgetContainer(contentMargin, contentAlign), mTextScale(1.0f) {}

  CLayer::~CLayer() {}

  void CLayer::Update(float timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CLayer::UpdateRender(core::CFont const& font, glm::vec2 const& size) {
    if(mContent) {
      auto ctx = CUpdateContext{ font, mTextScale };
      mContent->UpdateRender(ctx, size);
    }
  }

  void CLayer::Render(gfx::CCanvas & canvas, core::CFont const& font) const {
    if(mContent) {
      auto ctx = CRenderContext{ font, canvas, mTextScale };
      mContent->Render(ctx, { 0.0f, 0.0f });
    }
  }

  CLayer CLayer::Load(cb::string const & filepath) {
    auto xmlDoc = cb::CXmlDocument(cb::readtextfileutf8(filepath));
    if(!xmlDoc.IsValid()) {
      throw std::exception("Invalid xml doc file for screen loading.");
    }
    if(xmlDoc.RootNode.GetName() != L"Layer"s) {
      throw std::exception("Invalid root node for screen reading.");
    }
    auto screen = CLayer();
    if(!cb::ReadXmlObject(xmlDoc.RootNode, screen)) {
      throw std::exception("Failed reading xml gui screen.");
    }
    return screen;
  }
  std::unique_ptr<CLayer> CLayer::LoadPtr(cb::string const & filepath) {
    auto layer = Load(filepath);
    return std::make_unique<CLayer>(std::move(layer));
  }
}
