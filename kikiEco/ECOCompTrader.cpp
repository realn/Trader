#include "stdafx.h"

#include "ECOCompDock.h"
#include "ECOCompMarket.h"

#include "ECOEntity.h"
#include "ECOUniverse.h"
#include "ECOCompTrader.h"

namespace eco {
  static const auto COMP_TRADER_ID = L"Trader"_id;

  namespace comp {
    CTrader::CTrader(std::shared_ptr<CEntity> parent) 
      : CComponent(parent, COMP_TRADER_ID)
    {}

    CTrader::CTrader(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CTrader(parent)
    {}

    CTrader::~CTrader() {}

    void CTrader::Update(float const timeDelta) {
      auto parent = GetParent();

      auto uni = parent->GetUniverse();

      UpdatePrices(*uni);
      UpdateTransactions();
    }

    void CTrader::PrintInfo(cb::ostream & stream) const {}

    void CTrader::UpdatePrices(CUniverse const& uni) {
      auto marketEntities = uni.GetEntities<comp::CMarket, comp::CDock>();

      for(auto& entity : marketEntities) {
        auto& market = entity->GetComponent<comp::CMarket>();
        
        auto& priceList = market.GetPriceList();
        for(auto& price : priceList) {
          auto& data = mPrices[price.first];

          if(price.second.GetType() == PriceType::SELL) {
            data.mMax = glm::max(data.mMax, price.second.GetValue());
            data.mMaxMarket = entity;
          }
          else {
            data.mMin = glm::min(data.mMin, price.second.GetValue());
            data.mMinMarket = entity;
          }
        }
      }
    }

    void CTrader::UpdateTransactions() {
    }
  }
}
