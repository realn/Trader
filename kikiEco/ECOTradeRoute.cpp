#include "stdafx.h"
#include "ECOTradeRoute.h"
#include "ECOEntity.h"

namespace eco {
  CTradeJunction::CTradeJunction(std::shared_ptr<CEntity> targetA, std::shared_ptr<CEntity> targetB) {
    mTargets[0] = targetA;
    mTargets[1] = targetB;
  }
}
