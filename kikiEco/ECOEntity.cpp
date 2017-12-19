#include "stdafx.h"
#include "ECOEntity.h"

namespace eco {
  CEntity::CEntity(cb::string const & typeId, cb::string const & id) : 
    mTypeId(typeId), mId(id) {}

  CEntity::~CEntity() {}

  void CEntity::SetComponent(std::unique_ptr<CComponent> component) {
    mComponents[component->GetId()] = std::move(component);
  }

  bool CEntity::HasComponent(cb::string const & id) const {
    return mComponents.find(id) != mComponents.end();
  }

  bool CEntity::HasComponents(cb::strvector const & ids) const {
    auto check = [ids](const ComponentsT::value_type& item) {
      return std::find(ids.begin(), ids.end(), item.first) != ids.end();
    };
    return std::all_of(mComponents.begin(), mComponents.end(), check);
  }

  CComponent & CEntity::GetComponent(cb::string const & id) const {
    return *mComponents.at(id);
  }

}
