#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOCompIndustry.h"
#include "ECOCompMarket.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompGovernor.h"

namespace eco {
  static const auto COMP_GOVERNOR_ID = L"Governor"_id;

  namespace comp {
    constexpr float fast_abs(float const value) {
      return value >= 0.0f ? value : -value;
    }
    constexpr float fast_minmul(float const value, float const mulVal, float const minVal, float const pident = 1.0f) {
      return (value * mulVal > minVal / fast_abs(pident)) ? value * mulVal : value;
    }
    constexpr float fast_maxmul(float const value, float const mulVal, float const maxVal, float const pident = 1.0f) {
      return (value * mulVal < maxVal * fast_abs(pident)) ? value * mulVal : value;
    }


    CGovernor::CGovernor(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_GOVERNOR_ID)
    {}

    CGovernor::CGovernor(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CGovernor(parent)
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

        if(mWaitTime > mMaxWaitTime && mem.mMax > 0.0f) {
          float valEmpty = (mem.mMax - amount) / mem.mMax;
          float valFill = amount / mem.mMax;

          if(pident > 0.0f) {
            if(valEmpty < 0.1f) {
              mem.mMinValue = fast_minmul(mem.mMinValue, 0.9f, GOV_PRICE_MIN, pident);
              mem.mMaxValue = fast_minmul(mem.mMaxValue, 0.95f, mem.mMinValue);
            }
            else if(valFill < 0.1f) {
              mem.mMaxValue = fast_maxmul(mem.mMaxValue, 1.1f, GOV_PRICE_MAX, pident);
              mem.mMinValue = fast_maxmul(mem.mMinValue, 1.05f, mem.mMaxValue);
            }
          }
          else if(pident < 0.0f) {
            if(valFill < 0.1f) {
              mem.mMaxValue = fast_maxmul(mem.mMaxValue, 1.1f, GOV_PRICE_MAX, pident);
              mem.mMinValue = fast_maxmul(mem.mMinValue, 1.05f, mem.mMaxValue);
            }
            else if(valEmpty < 0.1f) {
              mem.mMinValue = fast_minmul(mem.mMinValue, 0.95f, GOV_PRICE_MIN, pident);
              mem.mMaxValue = fast_minmul(mem.mMaxValue, 0.9f, mem.mMinValue);
            }
          }
        }

        //value = glm::pow(value, 0.5f);
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
