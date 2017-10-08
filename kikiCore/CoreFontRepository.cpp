#include "stdafx.h"
#include "CoreFontRepository.h"
#include "CoreFont.h"

namespace core {
  CFontRepository::CFontRepository(cb::string const & assetsDir) 
    : CAssetRepository<CFont>(assetsDir, L"xml"s)
  {}

  CFontRepository::~CFontRepository() {}

  std::shared_ptr<CFont> CFontRepository::Load(cb::string const & name) const {
    auto path = GetAssetPath(name);
    auto font = CFont::Load(path);
    return std::make_shared<CFont>(std::move(font));
  }
}
