#pragma once

#include <map>

#include <CBStr/Defines.h>
#include <glm/vec2.hpp>

namespace gfx {
  class CTextureAtlas {
  public:
    struct Coords {
      glm::uvec2 ImgMin;
      glm::uvec2 ImgMax;
      glm::vec2 TexMin;
      glm::vec2 TexMax;
    };
  private:
    std::map<cb::string, Coords> mCoordMap;
    glm::uvec2 mTextureSize;
    cb::string mTextureFileName;
    
  public:
    CTextureAtlas(cb::string const& textureFileName,
                  glm::uvec2 const& textureSize)
      : mTextureSize(textureSize), mTextureFileName(textureFileName) {}

    glm::uvec2 GetTextureSize() const { return mTextureSize; }
    cb::string GetTextureFileName() const { return mTextureFileName; }

    void AddTexCoord(cb::string const& imgName, glm::vec2 const& min, glm::vec2 const& max);
    void AddImgCoord(cb::string const& imgName, glm::uvec2 const& min, glm::uvec2 const& max);

    const Coords& GetCoords(cb::string const& imgName) const { return mCoordMap.at(imgName); }
    const Coords& operator[](cb::string const& imgName) const { return GetCoords(imgName); }
  };
}