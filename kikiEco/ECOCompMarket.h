#pragma once

#include "ECOComponent.h"
#include "ECOProduct.h"

namespace eco {
  namespace comp {
    class CMarket :
      public CComponent {
    public:
      CMarket(std::shared_ptr<CEntity> parent);
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CMarket>();
}