#pragma once

#include "GUILabel.h"

namespace gui {
  class CText
    : public CLabel 
  {
  protected:
    struct CLine {
      glm::vec2 mPos;
      cb::string mText;
    };
    using LinesT = std::vector<CLine>;
    LinesT mLines;
    cb::string mOldText;

  public:
    CText(cb::string const& id = cb::string(), cb::string const& text = cb::string(),
          glm::vec4 const& textColor = glm::vec4(1.0f), Align const align = Align::Default);
    virtual ~CText();

    virtual void UpdateRender(CUpdateContext const& ctx, glm::vec2 const& spaceSize) override;
    virtual void Render(CRenderContext & ctx, glm::vec2 const & pos) const override;
  };
}