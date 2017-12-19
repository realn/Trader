#include "stdafx.h"
#include "ECOMarket.h"

namespace eco {
  static auto COMP_MARKET_ID = L"market"s;

  CMarket::CMarket() 
    : CComponent(COMP_MARKET_ID)
  {
  }

  template<>
  cb::string const& GetComponentId<CMarket>() {
    return COMP_MARKET_ID;
  }
}
