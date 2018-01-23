#include "stdafx.h"

#include "ECOComponent.h"
#include "ECOComponentFactory.h"
#include "ECOEntity.h"
#include "ECOEntityFactory.h"

namespace eco {
  std::weak_ptr<CEntityFactoryRegistry> CEntityFactoryRegistry::mInstance;

  CEntityFactory::CEntityFactory(cb::string const & typeId)
    : mTypeId(typeId)
  {}

  CEntityFactory::~CEntityFactory() {}

  std::shared_ptr<CEntity> CEntityFactory::Create(cb::string const & id) {
    auto entity = std::make_shared<CEntity>(mTypeId, id);
    auto registry = CComponentFactoryRegistry::GetInstance();

    for(auto& component : mComponents) {
      component.second->SetComponentToEntity(*entity);
    }

    return entity;
  }

  CEntityFactoryRegistry::CEntityFactoryRegistry() {}

  CEntityFactoryRegistry::~CEntityFactoryRegistry() {}

  std::shared_ptr<CEntityFactoryRegistry> CEntityFactoryRegistry::GetInstance() {
    if(mInstance.expired()) {
      auto instance = std::make_shared<CEntityFactoryRegistry>();
      mInstance = instance;
      return instance;
    }
    return mInstance.lock();
  }

  void CEntityFactoryRegistry::Register(std::shared_ptr<CEntityFactory> factory) {
    mTypes[factory->GetTypeId()] = factory;
  }

  std::shared_ptr<CEntityFactory> CEntityFactoryRegistry::Get(cb::string const & typeId) const {
    auto it = mTypes.find(typeId);
    if(it != mTypes.end()) {
      return it->second;
    }
    return std::shared_ptr<CEntityFactory>();
  }
}
