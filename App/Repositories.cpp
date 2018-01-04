#include "stdafx.h"
#include "Repositories.h"

namespace trader {
  static auto const ASSETS_FONTS_DIR = L"fonts"s;
  static auto const ASSETS_SHADERS_DIR = L"shaders"s;
  static auto const ASSETS_TEXTURES_DIR = L"textures"s;

  CRepositories::CRepositories(cb::string const & assetsPath) 
    : Fonts(cb::makepath(assetsPath, ASSETS_FONTS_DIR))
    , Shaders(cb::makepath(assetsPath, ASSETS_SHADERS_DIR))
    , Textures(cb::makepath(assetsPath, ASSETS_TEXTURES_DIR))
  {}
}
