#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOUniverse.h"
#include "ECOCompMarket.h"
#include "ECOTradeRoute.h"

namespace eco {
  CUniverse::~CUniverse() {}

  void CUniverse::AddEntity(std::shared_ptr<CEntity> entity) {
    auto marketId = GetComponentId<comp::CMarket>();
    if(entity->HasComponent(marketId)) {
      auto entities = GetEntities({ marketId });
      for(auto& market : entities) {
        mJunctions.push_back(std::make_shared<CTradeJunction>(entity, market));
      }
    }
    mEntities.push_back(entity);
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
