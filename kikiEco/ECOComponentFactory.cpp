#include "stdafx.h"
#include "ECOComponentFactory.h"

namespace eco {
  IComponentFactory::~IComponentFactory() {}

  CComponentFactoryRegistry::CComponentFactoryRegistry() {}

  CComponentFactoryRegistry::~CComponentFactoryRegistry() {}

  std::shared_ptr<CComponentFactoryRegistry> CComponentFactoryRegistry::GetInstance() {
    if(mInstance.expired()) {
      auto instance = std::make_shared<CComponentFactoryRegistry>();
      mInstance = instance;
      return instance;
    }
    return mInstance.lock();
  }

  std::shared_ptr<IComponentFactory> CComponentFactoryRegistry::GetFactory(cb::string const & id) {
    auto registry = GetInstance();
    return registry->Get(id);
  }

  std::shared_ptr<IComponentFactory> CComponentFactoryRegistry::Get(cb::string const & id) const {
    auto it = mFactories.find(id);
    if(it != mFactories.end()) {
      return it->second;
    }
    return std::shared_ptr<IComponentFactory>();
  }
}
