#include "stdafx.h"

#include <CBXml\Serialize.h>
#include <CBXml\Document.h>
#include <CBIO\File.h>

#include "GFXTextureAtlas.h"

struct CImageData {
  cb::string mName;
  glm::uvec2 mMin;
  glm::uvec2 mMax;
};

struct CAtlasData {
  using ImagesT = std::vector<CImageData>;

  cb::string mTextureFile;
  glm::uvec2 mTextureSize;
  ImagesT mImages;
};

namespace cb {
  static bool fromStr(string const& text, glm::uvec2& outValue) {
    auto list = split(text, L","s);
    return list.size() >= 2 &&
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y);
  }
}

static const auto XML_ATLAS = L"Atlas"s;
static const auto XML_ATLAS_TEXTURE = L"Texture"s;
static const auto XML_ATLAS_TEXSIZE = L"Size"s;
static const auto XML_ATLAS_IMAGE = L"Image"s;
static const auto XML_ATLAS_IMAGE_NAME = L"Name"s;
static const auto XML_ATLAS_IMAGE_MIN = L"Min"s;
static const auto XML_ATLAS_IMAGE_MAX = L"Max"s;

CB_DEFINEXMLREAD(CImageData) {
  return
    GetAttribute(XML_ATLAS_IMAGE_NAME, mObject.mName) &&
    GetAttribute(XML_ATLAS_IMAGE_MIN, mObject.mMin) &&
    GetAttribute(XML_ATLAS_IMAGE_MAX, mObject.mMax);
}

CB_DEFINEXMLREAD(CAtlasData) {
  return
    GetAttribute(XML_ATLAS_TEXTURE, mObject.mTextureFile) &&
    GetAttribute(XML_ATLAS_TEXSIZE, mObject.mTextureSize) &&
    GetNodeList(mObject.mImages, XML_ATLAS_IMAGE);
}

namespace gfx {
  void CTextureAtlas::AddTexCoord(cb::string const & imgName,
                                  glm::vec2 const & min,
                                  glm::vec2 const & max) {
    auto texSize = glm::vec2(mTextureSize);
    mCoordMap[imgName] = {
      glm::uvec2(min * texSize),
      glm::uvec2(max * texSize),
      min, max
    };
  }

  void CTextureAtlas::AddImgCoord(cb::string const & imgName,
                                  glm::uvec2 const & min,
                                  glm::uvec2 const & max) {
    auto texSize = glm::vec2(mTextureSize);
    mCoordMap[imgName] = {
      min, max,
      glm::vec2(min) / texSize,
      glm::vec2(max) / texSize
    };
  }

  CTextureAtlas CTextureAtlas::Load(cb::string const & filepath) {
    auto xmlDoc = cb::CXmlDocument(cb::readtextfileutf8(filepath));
    if(!xmlDoc.IsValid()) {
      throw std::exception("Invalid xml doc file for atlas loading.");
    }
    if(xmlDoc.RootNode.GetName() != XML_ATLAS) {
      throw std::exception("Invalid root node for atlas reading.");
    }
    auto atlas = CAtlasData();
    if(!cb::ReadXmlObject(xmlDoc.RootNode, atlas)) {
      throw std::exception("Failed reading xml atlas.");
    }

    auto result = CTextureAtlas(atlas.mTextureFile, atlas.mTextureSize);
    for(auto& img : atlas.mImages) {
      result.AddImgCoord(img.mName, img.mMin, img.mMax);
    }
    return result;
  }
}

