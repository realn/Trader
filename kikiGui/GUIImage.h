#pragma once

#include "GUIRect.h"

namespace gui {
  class CImage
    : public CRect 
  {
  protected:
    cb::string mImgName;

  public:
    CImage(cb::string const& id = cb::string(), 
           cb::string const& imgName = cb::string(), 
           glm::vec4 const& imgColor = glm::vec4(1.0f));
    virtual ~CImage();

    void SetImage(cb::string const& value) { mImgName = value; }
    cb::string GetImage() const { return mImgName; }

    virtual void Render(CRenderContext& ctx, glm::vec2 const& pos) const override;
  };
}