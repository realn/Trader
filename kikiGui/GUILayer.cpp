#include "stdafx.h"
#include "GUILayer.h"
#include "GUIWidget.h"
#include "GUIContext.h"
#include "GFXCanvas.h"
#include "GUIXml.h"

#include <CBXml/Document.h>
#include <CBIO/File.h>

namespace gui {
  CLayer::CLayer(glm::vec2 const & size, glm::vec4 const& contentMargin, Align const contentAlign) 
    : CWidgetContainer(contentMargin, contentAlign), mSize(size), mTextScale(1.0f) {}

  CLayer::~CLayer() {}

  void CLayer::Update(float timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CLayer::UpdateRender(gfx::CCanvas& canvas, core::CFont const& font) {
    if(mContent) {
      {
        auto ctx = CUpdateContext{font, mTextScale};
        mContent->UpdateRender(ctx, mSize);
      }
      {
        auto ctx = CRenderContext{font, canvas, mTextScale};
        mContent->Render(ctx, {0.0f, 0.0f});
      }
    }
  }

  CLayer CLayer::Load(cb::string const & filepath) {
    auto xmlDoc = cb::CXmlDocument(cb::readtextfileutf8(filepath));
    if(!xmlDoc.IsValid()) {
      throw std::exception("Invalid xml doc file for screen loading.");
    }
    if(xmlDoc.RootNode.GetName() != L"Screen"s) {
      throw std::exception("Invalid root node for screen reading.");
    }
    auto screen = CLayer();
    if(!cb::ReadXmlObject(xmlDoc.RootNode, screen)) {
      throw std::exception("Failed reading xml gui screen.");
    }
    return screen;
  }
}
