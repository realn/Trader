#pragma once

#include "ECOComponent.h"

namespace eco {
  class CTradeJunction;

  namespace xml {
    struct CComponent;
  }

  namespace comp {
    class CWarpDrive
      : public CComponent {
    private:
      std::weak_ptr<CTradeJunction> mJunction;
      float mPowerPerSecond = 0.5f;

    public:
      CWarpDrive(std::shared_ptr<CEntity> parent);
      CWarpDrive(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      virtual ~CWarpDrive();

      void SetJunction(std::shared_ptr<CTradeJunction> junction);
      void ClearJunction();

      void Update(float const timeDelta) override;

      void PrintInfo(cb::ostream& stream) const override;
    };
  }

  namespace xml {
    template<> void RegisterComponent<comp::CWarpDrive>();
  }

  template<>
  extern cb::string const& GetComponentId<comp::CWarpDrive>();
}