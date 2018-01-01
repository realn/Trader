#include "stdafx.h"
#include "ECOEntity.h"
#include "ECOTradeRoute.h"
#include "ECOCompWarpDrive.h"

namespace eco {
  static const auto COMP_WARPDRIVE_ID = L"WarpDrive"s;

  namespace comp {
    CWarpDrive::CWarpDrive(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_WARPDRIVE_ID) {}

    CWarpDrive::~CWarpDrive() {}

    void CWarpDrive::SetJunction(std::shared_ptr<CTradeJunction> junction) {
      mJunction = junction;

    }

    void CWarpDrive::ClearJunction() {
      mJunction.reset();
    }

    void CWarpDrive::Update(float const timeDelta) {
      if(!mJunction.expired()) {
        auto junction = mJunction.lock();
        auto parent = GetParent();

        auto info = junction->TravelUpdate(parent, mPowerPerSecond * timeDelta);
        parent->SetPosition(info.GetPosition());
        if(info.IsFinished()) {
          ClearJunction();
        }
      }
    }
  }

  template<>
  cb::string const& GetComponentId<comp::CWarpDrive>() {
    return COMP_WARPDRIVE_ID;
  }
}