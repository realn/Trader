#pragma once

#include <memory>
#include <vector>
#include <map>

#include <CBCore/Defines.h>
#include <CBGL/Fwd.h>
#include <glm/vec2.hpp>

namespace core {
  class CFont {
  public:
    struct CData {
      float mLineHeight = 0.0f;
      float mLineSkip = 0.0f;
      float mAscent = 0.0f;
      float mDescent = 0.0f;
    };
    struct CChar {
      glm::vec2 mMin;
      glm::vec2 mMax;
      glm::vec2 mTexMin;
      glm::vec2 mTexMax;
      glm::vec2 mAdv;

      glm::vec2 getVPos(glm::ivec2 const& xy, glm::vec2 const& scale = glm::vec2(1.0f)) const;
      glm::vec2 getVTex(glm::ivec2 const& xy) const;
    };
    using CharsT = std::map<wchar_t, CChar>;
  private:
    CData mData;
    CharsT mChars;
    cb::string mTextureFilePath;

  public:
    CFont(cb::string const& textureFilePath = cb::string());
    ~CFont();

    void AddChar(wchar_t code, CChar const& fontChar);

    const CChar&  GetChar(wchar_t code) const;
    float GetLineHeight() const { return 1.0f; }
    cb::string const& GetTextureFilePath() const { return mTextureFilePath; }

    glm::vec2 GetTextSize(cb::string const& text, bool const charHeight = false) const;

    static CFont Load(cb::string const& filepath);
  };
}
