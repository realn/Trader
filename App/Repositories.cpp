#include "Repositories.h"

#include <CBIO/Path.h>

using namespace std::literals;

namespace trader {
  static auto const SHADERS_DIR_NAME = L"shaders"s;

  CRepositories::CRepositories(cb::string const & assetsPath) 
    : Shaders(cb::makepath(assetsPath, SHADERS_DIR_NAME))
  {}
}
