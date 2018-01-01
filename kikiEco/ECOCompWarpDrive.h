#pragma once

#include "ECOComponent.h"

namespace eco {
  class CTradeJunction;

  namespace comp {
    class CWarpDrive
      : public CComponent {
    private:
      std::weak_ptr<CTradeJunction> mJunction;
      float mPowerPerSecond = 0.5f;

    public:
      CWarpDrive(std::shared_ptr<CEntity> parent);
      virtual ~CWarpDrive();

      void SetJunction(std::shared_ptr<CTradeJunction> junction);
      void ClearJunction();

      void Update(float const timeDelta) override;
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CWarpDrive>();
}