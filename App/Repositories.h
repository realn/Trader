#pragma once

#include <GFXShaderRepository.h>

namespace trader {
  class CRepositories {
  public:
    gfx::CShaderRepository Shaders;

    CRepositories(cb::string const& assetsPath);
  };
}