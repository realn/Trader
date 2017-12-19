#include "stdafx.h"
#include "ECOComponent.h"

namespace eco {
  CComponent::CComponent(cb::string const & id) : mId(id) {}

  CComponent::~CComponent() {}
}
