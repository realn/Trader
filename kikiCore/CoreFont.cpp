#include "stdafx.h"
#include "CoreFont.h"
#include "DataFont.h"

#include <CBCore/File.h>

namespace core {
  CFont::CFont(cb::string const& textureFilePath) : mTextureFilePath(textureFilePath) {}

  CFont::~CFont() {}

  void CFont::AddChar(wchar_t code, CFont::CChar const & fontChar) {
    mChars[code] = fontChar;
  }

  const CFont::CChar & CFont::GetChar(wchar_t code) const {
    auto it = mChars.find(code);
    if(it != mChars.end())
      return it->second;
    it = mChars.find('?');
    if(it != mChars.end())
      return it->second;
    return mChars.begin()->second;
  }

  glm::vec2 CFont::GetTextSize(cb::string const & text, bool const charHeight) const {
    if(text.empty()) {
      return glm::vec2();
    }

    auto result = glm::vec2();
    for(auto& fc : text) {
      auto data = GetChar(fc);
      result.x += data.mAdv.x;
      result.y = glm::max(result.y, data.mMax.y);
    }

    auto data = GetChar(*text.rbegin());
    result.x += (data.mMax.x - data.mMin.x) - data.mAdv.x;

    if(!charHeight) {
      result.y = glm::max(result.y, 1.0f);
    }

    return result;
  }

  glm::vec2 texComb(glm::ivec2 const& min, glm::ivec2 const& max) {
    return glm::vec2(static_cast<float>(min.x), static_cast<float>(max.y));
  }

  CFont CFont::Load(cb::string const & filepath) {
    using namespace glm;

    auto source = cb::readtextfileutf8(filepath);
    cb::CXmlDocument xmlDoc(source);
    if(!xmlDoc.IsValid()) {
      throw std::exception("Failed to load font.");
    }

    auto dataFont = data::CFont();
    if(!cb::ReadXmlObject(xmlDoc.RootNode, dataFont)) {
      throw std::exception("Failed to parse font xml.");
    }

    auto texSize = glm::vec2(dataFont.mTextureSize);
    auto lineDiv = static_cast<float>(dataFont.mLineHeight);

    auto font = CFont(dataFont.mTexture);
    font.mData.mLineSkip = static_cast<float>(dataFont.mLineSkip) / lineDiv;
    font.mData.mLineHeight = 1.0f;
    font.mData.mAscent = static_cast<float>(dataFont.mAscent) / lineDiv;
    font.mData.mDescent = static_cast<float>(dataFont.mDescent) / lineDiv;

    for(auto& dataChar : dataFont.mChars) {
      auto charSize = vec2(dataChar.mTexMax - dataChar.mTexMin);

      auto fontChar = CFont::CChar();
      fontChar.mMin = vec2(0.0f) / lineDiv;
      fontChar.mMax = charSize / lineDiv;
      fontChar.mTexMin = texComb(dataChar.mTexMin, dataChar.mTexMax) / texSize;
      fontChar.mTexMax = texComb(dataChar.mTexMax, dataChar.mTexMin) / texSize;
      fontChar.mAdv = vec2(dataChar.mAdv) / lineDiv;
      font.AddChar(dataChar.mCode, fontChar);
    }

    return font;
  }

  glm::vec2 CFont::CChar::getVPos(glm::ivec2 const& xy, glm::vec2 const& scale) const {
    using namespace glm;
    return mix(mMin, mMax, vec2(xy)) * scale;
  }

  glm::vec2 CFont::CChar::getVTex(glm::ivec2 const& xy) const {
    using namespace glm;
    return mix(mTexMin, mTexMax, vec2(xy));
  }
}
