#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOComponent.h"

namespace eco {
  CComponent::CComponent(std::shared_ptr<CEntity> parent, cb::string const & id) : mParent(parent), mId(id) {}

  CComponent::~CComponent() {}

  std::shared_ptr<CEntity> CComponent::GetParent() const {
    return mParent.lock();
  }
}
