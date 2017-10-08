#include "stdafx.h"
#include "GFXTextureAtlas.h"

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
}
