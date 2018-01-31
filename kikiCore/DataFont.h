#pragma once

#include <glm/glm.hpp>
#include <CBStr/StringEx.h>
#include <CBXml/Serialize.h>
#include <CBXml/Document.h>

using namespace std::literals;

namespace cb {
  template<typename _Type>
  string toStrVec2(_Type const& value) {
    return format(L"{0},{1}", value.x, value.y);
  }
  template<typename _Type>
  bool fromStrVec2(string const& text, _Type& outValue) {
    static const auto splitComma = L","s;
    auto list = split(text, splitComma);
    if(list.size() < 2)
      return false;
    return
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y);
  }

  static string toStr(glm::vec2 const& value) { return toStrVec2(value); }
  static string toStr(glm::ivec2 const& value) { return toStrVec2(value); }
  static string toStr(glm::uvec2 const& value) { return toStrVec2(value); }

  static bool fromStr(string const& text, glm::vec2& outValue) {
    return fromStrVec2(text, outValue);
  }
  static bool fromStr(string const& text, glm::ivec2& outValue) {
    return fromStrVec2(text, outValue);
  }
  static bool fromStr(string const& text, glm::uvec2& outValue) {
    return fromStrVec2(text, outValue);
  }
}

namespace data {
  struct CFontChar {
    wchar_t mCode = 0;
    glm::ivec2 mMin;
    glm::ivec2 mMax;
    glm::ivec2 mTexMin;
    glm::ivec2 mTexMax;
    glm::ivec2 mAdv;

    glm::ivec2 GetSize() const { return (mMax - mMin); }
    glm::ivec2 GetTexSize() const { return (mTexMax - mTexMin); }
  };

  struct CFont {
    cb::string mName;
    cb::string mTexture;
    cb::u32 mLineHeight;
    cb::u32 mLineSkip;
    cb::s32 mAscent;
    cb::s32 mDescent;
    glm::uvec2 mTextureSize;
    std::vector<CFontChar> mChars;
  };

  static const auto XML_FONTCHAR_CODE = L"Code"s;
  static const auto XML_FONTCHAR_MIN = L"Min"s;
  static const auto XML_FONTCHAR_MAX = L"Max"s;
  static const auto XML_FONTCHAR_TEXMIN = L"TexMin"s;
  static const auto XML_FONTCHAR_TEXMAX = L"TexMax"s;
  static const auto XML_FONTCHAR_ADV = L"Adv"s;

  static const auto XML_FONT_NAME = L"Name"s;
  static const auto XML_FONT_TEXTURE = L"Texture"s;
  static const auto XML_FONT_TEXTURESIZE = L"TextureSize"s;
  static const auto XML_FONT_CHARS_ELEM = L"Char"s;
  static const auto XML_FONT_LINEHEIGHT = L"LineHeight"s;
  static const auto XML_FONT_LINESKIP = L"LineSkip"s;
  static const auto XML_FONT_ASCENT = L"Ascent";
  static const auto XML_FONT_DESCENT = L"Descent";
}

CB_DEFINEXMLRW(data::CFontChar) {
  return
    RWAttribute(data::XML_FONTCHAR_CODE, mObject.mCode) &&
    RWAttribute(data::XML_FONTCHAR_MIN, mObject.mMin) &&
    RWAttribute(data::XML_FONTCHAR_MAX, mObject.mMax) &&
    RWAttribute(data::XML_FONTCHAR_TEXMIN, mObject.mTexMin) &&
    RWAttribute(data::XML_FONTCHAR_TEXMAX, mObject.mTexMax) &&
    RWAttribute(data::XML_FONTCHAR_ADV, mObject.mAdv);
}

CB_DEFINEXMLRW(data::CFont) {
  return
    RWAttribute(data::XML_FONT_NAME, mObject.mName) &&
    RWAttribute(data::XML_FONT_TEXTURE, mObject.mTexture) &&
    RWAttribute(data::XML_FONT_TEXTURESIZE, mObject.mTextureSize) &&
    RWAttribute(data::XML_FONT_LINEHEIGHT, mObject.mLineHeight) &&
    RWAttribute(data::XML_FONT_LINESKIP, mObject.mLineSkip) &&
    RWAttribute(data::XML_FONT_ASCENT, mObject.mAscent) &&
    RWAttribute(data::XML_FONT_DESCENT, mObject.mDescent) &&
    RWNodeList(mObject.mChars, data::XML_FONT_CHARS_ELEM);
}

