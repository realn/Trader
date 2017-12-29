#include "stdafx.h"

#include <set>

#include "ECOEntity.h"

namespace eco {
  CEntity::CEntity(cb::string const & typeId, cb::string const & id) : 
    mTypeId(typeId), mId(id) {}

  CEntity::~CEntity() {}

  void CEntity::SetComponent(std::unique_ptr<CComponent> component) {
    mComponentIds.insert(component->GetId());
    mComponents[component->GetId()] = std::move(component);
  }

  bool CEntity::HasComponent(cb::string const & id) const {
    return mComponents.find(id) != mComponents.end();
  }

  bool CEntity::HasComponents(cb::strvector const & ids) const {
    auto compIds = mComponentIds;
    auto check = [compIds](const cb::strvector::value_type& item) {
      return compIds.find(item) != compIds.end();
    };
    return std::all_of(ids.begin(), ids.end(), check);
  }

  CComponent & CEntity::GetComponent(cb::string const & id) const {
    return *mComponents.at(id);
  }

}
