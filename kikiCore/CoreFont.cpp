#include "stdafx.h"
#include "CoreFont.h"
#include "../FontCompiler/DataFont.h"

#include <CBIO/File.h>

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

    auto font = CFont(dataFont.mTexture);
    auto lineSize = glm::vec2(static_cast<float>(dataFont.mLineHeight));
    auto texSize = glm::vec2(dataFont.mTextureSize);

    auto posAdj = glm::vec2(0.0f, dataFont.mAscent) / lineSize;

    for(auto& dataChar : dataFont.mChars) {
      auto fontChar = CFont::CChar();
      fontChar.mMin = vec2(0.0f);// vec2(dataChar.mMin.x, -dataChar.mMax.y) / lineSize + posAdj;
      fontChar.mMax = vec2(1.0f);// vec2(dataChar.GetSize()) / lineSize + posAdj;
      fontChar.mTexMin = vec2(dataChar.mTexMin) / texSize;
      fontChar.mTexMax = vec2(dataChar.mTexMax) / texSize;
      fontChar.mAdv = vec2(dataChar.mAdv) / lineSize;

      font.AddChar(dataChar.mCode, fontChar);

      //font.AddChar(dataChar.mCode, {
      //  glm::vec2(dataChar.mMin.x, -dataChar.mMax.y) / lineSize + posAdj,
      //  glm::vec2(dataChar.GetTexSize()) / lineSize + posAdj,
      //  glm::vec2(dataChar.mTexMin.x, dataChar.mTexMin.y) / texSize,
      //  glm::vec2(dataChar.mTexMax.x, dataChar.mTexMax.y) / texSize,
      //  glm::vec2(dataChar.mAdv) / lineSize
      //});
    }

    return font;
  }

  glm::vec2 getOrg(glm::vec2 const& min, glm::vec2 const& max, glm::vec2 const& xy) {
    auto nxy = glm::vec2(1.0f) - xy;
    return min * nxy + max * xy;
  }


  glm::vec2 CFont::CChar::getVPos(glm::ivec2 const& xy, glm::vec2 const& scale) const {
   // return mMin + glm::vec2(mMax - mMin) * glm::vec2(xy) * scale;
    //return getOrg(mMin, mMax, glm::vec2(xy));
    return glm::vec2(xy) * scale;
  }

  glm::vec2 CFont::CChar::getVTex(glm::ivec2 const& xy) const {
    return getOrg(mTexMin, mTexMax, glm::vec2(xy));
  }
}
