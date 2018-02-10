#include "stdafx.h"

#include "ECOEntity.h"
#include "ECOCompIndustry.h"
#include "ECOCompMarket.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompGovernor.h"

namespace eco {
  static const auto COMP_GOVERNOR_ID = L"Governor"s;

  constexpr auto GOV_PRICE_MIN = 10.0f;
  constexpr auto GOV_PRICE_MAX = 500.0f;
  constexpr auto GOV_NEAR_ZERO = 0.001f;

  namespace comp {
    CGovernor::CGovernor(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_GOVERNOR_ID)
    {}

    CGovernor::CGovernor(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CComponent(parent, COMP_GOVERNOR_ID)
    {}

    CGovernor::~CGovernor() {}

    void CGovernor::Update(const float timeDelta) {
      auto parent = GetParent();
      if(!parent->HasComponents<comp::CMarket, comp::CIndustry>()) {
        return;
      }

      auto& market = parent->GetComponent<comp::CMarket>();
      auto& industry = parent->GetComponent<comp::CIndustry>();

      auto production = industry.GetProduction();
      auto priceList = market.GetPriceList();
      auto storage = market.GetStorage();

      for(auto& item : production) {
        auto& mem = mMemory[item.first];

        auto pident = item.second;
        if(glm::abs(pident) <= GOV_NEAR_ZERO) {
          market.SetProductValue(item.first, 0.0f);
        }

        auto amount = storage.GetProductAmount(item.first);

        mem.mMax = glm::max(mem.mMax, amount);
        mem.mMin = glm::min(mem.mMin, amount);

        auto value = (amount - mem.mMin) / (mem.mMax - mem.mMin);
        value = glm::mix(GOV_PRICE_MAX, GOV_PRICE_MIN, value) * glm::abs(pident);

        market.SetProductValue(item.first, value);
      }
    }

    void CGovernor::PrintInfo(cb::ostream & stream) const {
      stream << L" GOVERNOR"s << std::endl;
    }
  }

  template<>
  cb::string const& GetComponentId<comp::CGovernor>() {
    return COMP_GOVERNOR_ID;
  }

  namespace xml {
    template<>
    void RegisterComponent<comp::CGovernor>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_GOVERNOR_ID);
    }
  }
}
