#pragma once

#include <memory>
#include <map>
#include <CBStr/Defines.h>
#include <CBIO/Path.h>

#include "CoreFont.h"

namespace core {
  template<typename _Type>
  class CAssetRepository {
  private:
    cb::string mAssetDir;
    cb::string mAssetExt;
    std::map<cb::string, std::weak_ptr<_Type>> mRepo;

  public:
    CAssetRepository(cb::string const& assetDir, cb::string const& assetExt)
      : mAssetDir(assetDir), mAssetExt(assetExt) {}
    virtual ~CAssetRepository() = default;

    std::shared_ptr<_Type> Get(cb::string const& name);
    virtual std::shared_ptr<_Type> Load(cb::string const& name) const = 0;

  protected:
    virtual cb::string GetAssetPath(cb::string const& name) const;
    virtual std::shared_ptr<_Type> LoadAsset(cb::string const& name);
  };

  template<typename _Type>
  std::shared_ptr<_Type> CAssetRepository<_Type>::Get(cb::string const & name) {
    auto it = mRepo.find(name);
    if(it == mRepo.end()) {
      return LoadAsset(name);
    }
    auto ptr = it->second.lock();
    if(!ptr) {
      return LoadAsset(name);
    }
    return ptr;
  }

  template<typename _Type>
  cb::string CAssetRepository<_Type>::GetAssetPath(cb::string const & name) const {
    auto filename = name;
    if(!cb::ends_with(filename, mAssetExt)) {
      filename = cb::makefilename(name, mAssetExt);
    }
    return cb::makepath(mAssetDir, filename);
  }

  template<typename _Type>
  std::shared_ptr<_Type> CAssetRepository<_Type>::LoadAsset(cb::string const & name) {
    auto ptr = Load(name);
    if(ptr) {
      mRepo[name] = ptr;
    }
    return ptr;
  }
}