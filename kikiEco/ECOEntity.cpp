#include "stdafx.h"

#include <set>

#include "ECOEntity.h"

namespace eco {
  CEntity::CEntity(cb::string const & typeId, cb::string const & id) : 
    mTypeId(typeId), mId(id) {}

  CEntity::~CEntity() {}

  void CEntity::UpdateComponents(float const timeDelta) {
    for(auto& item : mComponents) {
      item.second->Update(timeDelta);
    }
  }

  void CEntity::SetComponent(std::unique_ptr<CComponent> component) {
    mComponentIds.insert(component->GetId());
    mComponents[component->GetId()] = std::move(component);
  }

  bool CEntity::HasComponent(core::CFastId const & id) const {
    return mComponents.find(id) != mComponents.end();
  }

  bool CEntity::HasComponents(core::FastIdsT const & ids) const {
    auto itend = mComponentIds.end();
    for(auto& id : ids) {
      if(mComponentIds.find(id) == itend) {
        return false;
      }
    }
    return true;
  }

  CComponent & CEntity::GetComponent(core::CFastId const & id) const {
    return *mComponents.at(id);
  }

  void CEntity::PrintInfo(cb::ostream & stream, ComponentIdsT const & componentIds) const {
    stream << cb::format(L"Entity: {0} ({1})"s, mId, mTypeId) << std::endl;
    auto ids = componentIds.empty() ? mComponentIds : componentIds;
    for(auto& id : ids) {
      auto it = mComponents.find(id);
      if(it != mComponents.end()) {
        it->second->PrintInfo(stream);
      }
    }
  }

}
