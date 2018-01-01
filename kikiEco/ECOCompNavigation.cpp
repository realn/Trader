#include "stdafx.h"

#include <random>

#include "ECOEntity.h"
#include "ECOTradeRoute.h"
#include "ECOCompDock.h"
#include "ECOCompWarpDrive.h"
#include "ECOCompNavigation.h"

namespace eco {
  static const auto COMP_NAVIGATION_ID = L"Navigation"s;

  namespace comp {
    CNavigation::CNavigation(std::shared_ptr<CEntity> parent) 
      : CComponent(parent, COMP_NAVIGATION_ID)
    {}

    CNavigation::~CNavigation() {}

    void CNavigation::SetCurrentDock(std::shared_ptr<CEntity> dock) {
      mCurrentDock = dock;
    }

    void CNavigation::ClearCurrentDock() {
      mCurrentDock.reset();
    }

    void CNavigation::Update(float const timeDelta) {
      if(!mCurrentDock.expired()) {
        auto dock = mCurrentDock.lock();
        auto targets = dock->GetComponent<CDock>().GetConnectedDocks();

        std::random_device dev;
        auto it = targets.begin();
        std::advance(it, dev() % targets.size());

        auto target = it->lock();
        auto parent = GetParent();
        auto junction = dock->GetComponent<CDock>().FindJunction(target);

        junction->TravelTo(parent, dock, target);
        parent->GetComponent<comp::CWarpDrive>().SetJunction(junction);
      }
    }

  }

  template<>
  cb::string const& GetComponentId<comp::CNavigation>() {
    return COMP_NAVIGATION_ID;
  }
}
