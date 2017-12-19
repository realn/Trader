#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOUniverse.h"
#include "ECOMarket.h"
#include "ECOTradeRoute.h"

namespace eco {
  CUniverse::~CUniverse() {}

  void CUniverse::AddEntity(CEntity && entity) {
    auto newEntity = std::make_shared<CEntity>(std::move(entity));
    if(newEntity->HasComponent(GetComponentId<CMarket>())) {
      auto entities = GetEntities({ GetComponentId<CMarket>() });
      for(auto& market : entities) {
        mJunctions.push_back(std::make_shared<CTradeJunction>(newEntity, market));
      }
    }
    mEntities.push_back(newEntity);
  }

  CUniverse::EntitiesT CUniverse::GetEntities(cb::strvector const & requiredComponents) const {
    auto result = EntitiesT();
    for(auto& entity : mEntities) {
      if(entity->HasComponents(requiredComponents)) {
        result.push_back(entity);
      }
    }
    return result;
  }
}
