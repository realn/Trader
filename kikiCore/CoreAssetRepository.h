#pragma once

#include <memory>
#include <map>
#include <CBCore/Defines.h>
#include <CBCore/Path.h>

namespace core {
  class CAssetRepositoryBase {
  protected:
    cb::string mAssetDir;
    cb::string mAssetExt;

  public:
    CAssetRepositoryBase(cb::string const& assetDir, cb::string const& assetExt);
    virtual ~CAssetRepositoryBase();

  protected:
    virtual cb::string GetAssetPath(cb::string const& name) const;
    virtual cb::strvector GetAssetPath(cb::strvector const& names) const;

    virtual cb::strvector ConvertKey(cb::string const& name) const;
    virtual cb::string ConvertKey(cb::strvector const& name) const;
  };

  template<typename _Type>
  class CAssetRepository 
    : public CAssetRepositoryBase
  {
  private:
    std::map<cb::string, std::weak_ptr<_Type>> mRepo;

  public:
    CAssetRepository(cb::string const& assetDir, cb::string const& assetExt)
      : CAssetRepositoryBase(assetDir, assetExt) {}
    virtual ~CAssetRepository() = default;

    std::shared_ptr<_Type> Get(cb::string const& name);
    std::shared_ptr<_Type> Get(cb::strvector const& names);

    virtual std::shared_ptr<_Type> Load(cb::string const& name) const = 0;
    virtual std::shared_ptr<_Type> Load(cb::strvector const& names) const;

  protected:
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
  std::shared_ptr<_Type> CAssetRepository<_Type>::Get(cb::strvector const & names) {
    return Get(ConvertKey(names));
  }

  template<typename _Type>
  std::shared_ptr<_Type> CAssetRepository<_Type>::Load(cb::strvector const & names) const {
    return Load(ConvertKey(names));
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
