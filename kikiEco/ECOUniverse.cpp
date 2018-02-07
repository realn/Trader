#include "stdafx.h"

#include <random>

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
          auto junction = std::make_shared<CTradeJunction>(entity, dock);

          entity->GetComponent<comp::CDock>().AddJunction(junction);
          dock->GetComponent<comp::CDock>().AddJunction(junction);

          mJunctions.push_back(junction);
        }
      }
    }
    mEntities.push_back(entity);
  }

  std::shared_ptr<CEntity> CUniverse::FindEntity(cb::string const & id) const {
    if(id != L"Random"s) {
      auto it = std::find_if(mEntities.begin(), mEntities.end(),
                             [&id](EntitiesT::value_type const& item)->auto{return item->GetId() == id; });
      if(it != mEntities.end()) {
        return *it;
      }
      return std::shared_ptr<CEntity>();
    }
    else {
      static std::random_device dev;
      auto it = mEntities.begin();
      std::advance(it, dev() % mEntities.size());
      return *it;
    }
  }

  std::shared_ptr<CEntity> CUniverse::FindEntity(cb::string const & id, cb::string const & requiredComponent) const {
    return FindEntity(id, cb::strvector{ requiredComponent });
  }

  std::shared_ptr<CEntity> CUniverse::FindEntity(cb::string const & id, cb::strvector const & requiredComponents) const {
    if(id != L"Random"s) {
      auto pred = [&id, &requiredComponents](EntitiesT::value_type const& item) -> auto {
        return item->GetId() == id && item->HasComponents(requiredComponents);
      };
      auto it = std::find_if(mEntities.begin(), mEntities.end(), pred);
      if(it != mEntities.end()) {
        return *it;
      }
      return std::shared_ptr<CEntity>();
    }
    else {
      static std::random_device dev;
      auto entities = GetEntities(requiredComponents);
      auto it = entities.begin();
      std::advance(it, dev() % entities.size());
      return *it;
    }
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

  CUniverse::EntitiesT CUniverse::GetEntities(cb::string const& requiredComponent) const {
    auto result = EntitiesT();
    for(auto& entity : mEntities) {
      if(entity->HasComponent(requiredComponent)) {
        result.push_back(entity);
      }
    }
    return result;
  }
}

