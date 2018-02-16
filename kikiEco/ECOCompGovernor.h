#pragma once

#include "ECOComponent.h"

namespace eco {
  namespace xml {
    struct CComponent;
  }

  namespace comp {

    constexpr auto GOV_PRICE_MIN = 10.0f;
    constexpr auto GOV_PRICE_MAX = 100.0f;
    constexpr auto GOV_PRICE_AVG = (GOV_PRICE_MIN + GOV_PRICE_MAX) / 2.0f;
    constexpr auto GOV_NEAR_ZERO = 0.01f;

    class CGovernor 
      : public CComponent
    {
    private:
      struct ProductInfo {
        float mMin = std::numeric_limits<float>::max();
        float mMax = std::numeric_limits<float>::min();
        float mMinValue = GOV_PRICE_AVG;
        float mMaxValue = GOV_PRICE_AVG;
      };
      using MemT = std::map<ProductId, ProductInfo>;

      MemT mMemory;
      float mWaitTime = 0.0f;
      float mMaxWaitTime = 5.0f;

    public:
      CGovernor(std::shared_ptr<CEntity> parent);
      CGovernor(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      virtual ~CGovernor();

      virtual void Update(const float timeDelta);

      virtual void PrintInfo(cb::ostream& stream) const;
    };
  }

  template<>
  core::CFastId const& GetComponentId<comp::CGovernor>();

  namespace xml {
    template<> void RegisterComponent<comp::CGovernor>();
  }
}