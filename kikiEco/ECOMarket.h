#pragma once

#include "ECOComponent.h"
#include "ECOProduct.h"

namespace eco {

  class CMarket :
    public CComponent
  {
  public:
    CMarket();
  };

  template<>
  extern cb::string const& GetComponentId<CMarket>();

}