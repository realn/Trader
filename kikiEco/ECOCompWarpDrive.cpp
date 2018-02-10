#include "stdafx.h"

#include <CBXml/Serialize.h>

#include "ECOEntity.h"
#include "ECOTradeRoute.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompWarpDrive.h"

namespace eco {
  static const auto COMP_WARPDRIVE_ID = L"WarpDrive"_id;

  namespace xml {
    template<>
    void RegisterComponent<comp::CWarpDrive>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_WARPDRIVE_ID.GetName());
    }
  }

  namespace comp {
    CWarpDrive::CWarpDrive(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_WARPDRIVE_ID) {}

    CWarpDrive::CWarpDrive(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CComponent(parent, COMP_WARPDRIVE_ID)
    {}

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
    void CWarpDrive::PrintInfo(cb::ostream & stream) const {
      stream << L" WARPDRIVE: "s;
      if(!mJunction.expired()) {
        stream << L"ONLINE" << std::endl;
      }
      else {
        stream << L"OFFLINE" << std::endl;
      }
    }
  }

  template<>
  core::CFastId const& GetComponentId<comp::CWarpDrive>() {
    return COMP_WARPDRIVE_ID;
  }
}