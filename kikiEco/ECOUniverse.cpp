#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOUniverse.h"
#include "ECOCompMarket.h"
#include "ECOCompDock.h"
#include "ECOTradeRoute.h"

namespace eco {
  CUniverse::~CUniverse() {}

  void CUniverse::UpdateEntities(float const timeDelta) {
    for(auto& item : mEntities) {
      item->UpdateComponents(timeDelta);
    }
  }

  void CUniverse::AddEntity(std::shared_ptr<CEntity> entity) {
    auto dockId = GetComponentId<comp::CDock>();
    if(entity->HasComponent(dockId)) {
      auto entities = GetEntities({ dockId });
      for(auto& dock : entities) {
        if(glm::distance(entity->GetPosition(), dock->GetPosition()) < mMaxJunctionDist) {
          mJunctions.push_back(std::make_shared<CTradeJunction>(entity, dock));
        }
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
