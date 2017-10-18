#pragma once

#include <GFXShaderRepository.h>
#include <GFXTextureRepository.h>

namespace trader {
  class CRepositories {
  public:
    gfx::CShaderRepository Shaders;
    gfx::CTextureRepository Textures;

    CRepositories(cb::string const& assetsPath);
  };
}