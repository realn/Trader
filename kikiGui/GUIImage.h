#pragma once

#include "GUIRect.h"

namespace gui {
  class CImage
    : public CRect 
  {
  protected:
    cb::string mImgName;
    bool mFlipHorizontal = false;
    bool mFlipVertical = false;

  public:
    CImage(cb::string const& id = cb::string(), 
           cb::string const& imgName = cb::string(), 
           glm::vec4 const& imgColor = glm::vec4(1.0f));
    virtual ~CImage();

    void SetImage(cb::string const& value) { mImgName = value; }
    cb::string GetImage() const { return mImgName; }

    void SetFlipHorizontal(bool const value) { mFlipHorizontal = value; }
    bool GetFlipHorizontal() const { return mFlipHorizontal; }
    void SetFlipVertical(bool const value) { mFlipVertical = value; }
    bool GetFlipVertical() const { return mFlipVertical; }

    virtual void Render(CRenderContext& ctx, glm::vec2 const& pos) const override;
  };
}