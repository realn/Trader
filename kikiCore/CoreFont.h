#pragma once

#include <memory>
#include <vector>
#include <map>

#include <CBStr/StringEx.h>
#include <CBGL/Fwd.h>
#include <glm/vec2.hpp>

namespace core {
  class CFont {
  public:
    struct CChar {
      glm::vec2 mMin;
      glm::vec2 mMax;
      glm::vec2 mTexMin;
      glm::vec2 mTexMax;
      glm::vec2 mAdv;

      glm::vec2 getVPos(glm::ivec2 const& xy, glm::vec2 const& scale = glm::vec2(1.0f)) const;
      glm::vec2 getVTex(glm::ivec2 const& xy) const;
    };
  private:
    std::map<wchar_t, CChar> mChars;
    float mLineHeight;
    float mAscent;
    float mDescent;
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
