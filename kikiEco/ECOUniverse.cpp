#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOUniverse.h"

namespace eco {
  CUniverse::~CUniverse() {}
  void CUniverse::AddEntity(CEntity && entity) {
    mEntities.push_back(std::make_shared<CEntity>(entity));
  }
}
