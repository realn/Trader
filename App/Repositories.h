#pragma once

#include <GFXShaderRepository.h>
#include <GFXTextureRepository.h>

#include "MeshRepository.h"

namespace trader {
  class CRepositories {
  public:
    gfx::CShaderRepository Shaders;
    gfx::CTextureRepository Textures;
    CMeshRepository Meshes;

    CRepositories(cb::string const& assetsPath);
  };
}