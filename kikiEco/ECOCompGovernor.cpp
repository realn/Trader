#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOCompIndustry.h"
#include "ECOCompMarket.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompGovernor.h"

namespace eco {
  static const auto COMP_GOVERNOR_ID = L"Governor"_id;

  constexpr auto GOV_PRICE_MIN = 10.0f;
  constexpr auto GOV_PRICE_MAX = 1000.0f;
  constexpr auto GOV_NEAR_ZERO = 0.01f;

  namespace comp {
    CGovernor::CGovernor(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_GOVERNOR_ID)
    {}

    CGovernor::CGovernor(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CComponent(parent, COMP_GOVERNOR_ID)
    {}

    CGovernor::~CGovernor() {}

    void CGovernor::Update(const float timeDelta) {
      static const auto compListIds = GetComponentIds<comp::CMarket, comp::CIndustry>();

      auto parent = GetParent();
      if(!parent->HasComponents(compListIds)) {
        return;
      }

      auto& market = parent->GetComponent<comp::CMarket>();
      auto& industry = parent->GetComponent<comp::CIndustry>();

      auto& production = industry.GetProduction();
      auto& priceList = market.GetPriceList();
      auto& storage = market.GetStorage();

      mWaitTime += timeDelta;
      for(auto& item : production) {
        auto& mem = mMemory[item.first];

        auto pident = item.second;
        if(glm::abs(pident) <= GOV_NEAR_ZERO) {
          market.ClearProductValue(item.first);
          continue;
        }

        auto amount = storage.GetProductAmount(item.first);

        mem.mMax = glm::max(mem.mMax, amount);
        mem.mMin = glm::min(mem.mMin, amount);

        auto value = 0.0f;
        if(mem.mMax - mem.mMin > 0.0f) {
          value = (amount - mem.mMin) / (mem.mMax - mem.mMin);
        }

        if(mWaitTime > mMaxWaitTime) {
          if(pident > 0.0f && mem.mMax > 0.0f) {
            if((mem.mMax - amount) / mem.mMax < 0.1f && mem.mMinValue > GOV_PRICE_MIN) {
              mem.mMinValue *= 0.9f;
            }
          }
          else if(pident < 0.0f && mem.mMax > 0.0f) {
            if((amount - mem.mMin) / mem.mMax < 0.1f && mem.mMaxValue < GOV_PRICE_MAX) {
              mem.mMaxValue *= 1.1f;
            }
          }
        }

        value = glm::pow(value, 0.5f);
        value = glm::mix(mem.mMaxValue, mem.mMinValue, value);

        market.SetProductValue(item.first, pident > 0.0f ? PriceType::SELL : PriceType::BUY, value);
      }

      if(mWaitTime > mMaxWaitTime) {
        mWaitTime = 0.0f;
      }
    }

    void CGovernor::PrintInfo(cb::ostream & stream) const {
      stream << L" GOVERNOR"s << std::endl;
    }
  }

  template<>
  core::CFastId const& GetComponentId<comp::CGovernor>() {
    return COMP_GOVERNOR_ID;
  }

  namespace xml {
    template<>
    void RegisterComponent<comp::CGovernor>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_GOVERNOR_ID.GetName());
    }
  }
}
