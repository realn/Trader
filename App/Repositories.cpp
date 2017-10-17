#include "stdafx.h"
#include "Repositories.h"

namespace trader {
  static auto const ASSETS_SHADERS_DIR = L"shaders"s;

  CRepositories::CRepositories(cb::string const & assetsPath) 
    : Shaders(cb::makepath(assetsPath, ASSETS_SHADERS_DIR))
  {}
}
