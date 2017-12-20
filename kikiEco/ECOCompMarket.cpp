#include "stdafx.h"
#include "ECOCompMarket.h"

namespace eco {
  static auto COMP_MARKET_ID = L"market"s;

  namespace comp {
    CMarket::CMarket(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_MARKET_ID) {}
  }

  template<>
  cb::string const& GetComponentId<comp::CMarket>() {
    return COMP_MARKET_ID;
  }
}
