#pragma once

#include "ECOComponent.h"

namespace eco {
  namespace xml {
    struct CComponent;
  }

  namespace comp {
    class CGovernor 
      : public CComponent
    {
    private:
      struct ProductInfo {
        float mMin = 0.0f;
        float mMax = 0.0f;
      };
      using MemT = std::map<ProductId, ProductInfo>;

      MemT mMemory;

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