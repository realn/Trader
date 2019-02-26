#include "stdafx.h"

#include <CBCore/StringFunctions.h>
#include <CBCore/Path.h>

#include "CoreAssetRepository.h"

namespace core {
  CAssetRepositoryBase::CAssetRepositoryBase(cb::string const & assetDir, cb::string const & assetExt) 
    : mAssetDir(assetDir), mAssetExt(assetExt) {}

  CAssetRepositoryBase::~CAssetRepositoryBase() {}

  cb::string CAssetRepositoryBase::GetAssetPath(cb::string const & name) const {
    auto filename = name;
    if(!cb::ends_with(filename, mAssetExt)) {
      filename = cb::makefilename(name, mAssetExt);
    }
    return cb::makepath(mAssetDir, filename);
  }

  cb::strvector CAssetRepositoryBase::GetAssetPath(cb::strvector const & names) const {
    auto result = cb::strvector();
    for(auto& item : names) {
      result.push_back(item);
    }
    return result;
  }

  cb::strvector CAssetRepositoryBase::ConvertKey(cb::string const & name) const {
    return cb::split(name, L","s, true);
  }

  cb::string CAssetRepositoryBase::ConvertKey(cb::strvector const & name) const {
    auto sorted = name;
    std::sort(sorted.begin(), sorted.end());
    return cb::join(sorted, L","s);
  }
}
