#include "stdafx.h"
#include "CoreFont.h"
#include "DataFont.h"

#include <CBIO/File.h>

namespace core {
  CFont::CFont(cb::string const& textureFilePath) : mTextureFilePath(textureFilePath) {}

  CFont::~CFont() {}

  void CFont::AddChar(wchar_t code, CFont::CChar const & fontChar) {
    mChars[code] = fontChar;
  }

  glm::vec2 CFont::getVPos(CChar const & fontChar, glm::ivec2 const & xy, glm::vec2 const & scale) const {
    return fontChar.getVPos(xy, mData, scale);
  }

  glm::vec2 CFont::getVTex(CChar const & fontChar, glm::ivec2 const & xy) const {
    return fontChar.getVTex(xy, mData);
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
    for(auto i = 0u; i < text.length()-1; i++) {
      auto data = GetChar(text[i]);
      result.x += data.mAdv.x;
      result.y = glm::max(result.y, data.mMax.y);
    }

    auto data = GetChar(*text.rbegin());
    result.x += data.mMax.x - data.mMin.x;
    result.y = glm::max(result.y, data.mMax.y);

    if(!charHeight) {
      result.y = glm::max(result.y, 1.0f);
    }

    return result;
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
    auto adj = glm::vec2(static_cast<float>(dataFont.mLineSkip)) / texSize;

    auto font = CFont(dataFont.mTexture);
    font.mData.mLineSkip = dataFont.mLineSkip;
    font.mData.mLineHeight = dataFont.mLineHeight;
    font.mData.mAscent = static_cast<float>(dataFont.mAscent);
    font.mData.mDescent = static_cast<float>(dataFont.mDescent);

    auto padj = vec2(0.0f, font.mData.mLineHeight - font.mData.mDescent);
    auto pmul = vec2(1.0f, -1.0f);

    for(auto& dataChar : dataFont.mChars) {
      auto fontChar = CFont::CChar();
      fontChar.mMin = vec2(0.0f) * pmul + padj + vec2(dataChar.mMin) * pmul;
      fontChar.mMax = vec2(dataChar.mTexMax - dataChar.mTexMin) * pmul + padj + vec2(dataChar.mMin) * pmul;
      fontChar.mTexMin = vec2(dataChar.mTexMin) / texSize;
      fontChar.mTexMax = vec2(dataChar.mTexMax) / texSize;
      fontChar.mAdv = vec2(dataChar.mAdv);
      font.AddChar(dataChar.mCode, fontChar);
    }

    return font;
  }

  glm::vec2 CFont::CChar::getVPos(glm::ivec2 const& xy, CData const& data, glm::vec2 const& scale) const {
    using namespace glm;
    //return glm::mix(glm::vec2(0.0f), glm::vec2(1.0f), glm::vec2(xy));
    return mix(mMin, mMax, vec2(xy)) * scale;
  }

  glm::vec2 CFont::CChar::getVTex(glm::ivec2 const& xy, CData const& data) const {
    using namespace glm;
    //return getOrg(mTexMin, mTexMax, glm::vec2(xy));
    return mix(mTexMin, mTexMax, vec2(xy));
  }
}
