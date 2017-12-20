#include "stdafx.h"
#include "ECOMarket.h"

namespace eco {
  static auto COMP_MARKET_ID = L"market"s;

  CMarket::CMarket(std::shared_ptr<CEntity> parent) 
    : CComponent(parent, COMP_MARKET_ID)
  {
  }

  template<>
  cb::string const& GetComponentId<CMarket>() {
    return COMP_MARKET_ID;
  }
}
