#include "stdafx.h"
#include "ECOTradeRoute.h"
#include "ECOCompDock.h"
#include "ECOEntity.h"

namespace eco {
  void CTravelInfo::AddPower(float const power, float const powerPerUnit) {
    mProgress = (GetPowerCurrent(powerPerUnit) + power / GetPowerNeeded(powerPerUnit));
  }

  float CTravelInfo::GetDistance() const {
    return glm::distance(mSource->GetPosition(), mTarget->GetPosition());
  }

  float CTravelInfo::GetPowerNeeded(float const powerPerUnit) const {
    return GetDistance() * powerPerUnit;
  }

  float CTravelInfo::GetPowerCurrent(float const powerPerUnit) const {
    return GetProgress() * GetPowerNeeded(powerPerUnit);
  }

  CTradeJunction::CTradeJunction() {}

  void CTradeJunction::AddTarget(std::shared_ptr<CEntity> dockable) {
    if(dockable->HasComponent<comp::CDock>()) {
      mTargets.insert(dockable);
    }
  }

  void CTradeJunction::TravelTo(std::shared_ptr<CEntity> ship, std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target) {
    if(mTravels.find(ship) != mTravels.end()) {
      return;
    }
    if(mTargets.find(target) == mTargets.end() || mTargets.find(source) == mTargets.end()) {
      return;
    }
    auto& sourceDock = source->GetComponent<comp::CDock>();
    if(sourceDock.GetDockedShips().find(ship) == sourceDock.GetDockedShips().end()) {
      return;
    }

    sourceDock.UnDockShip(ship);
    mTravels[ship] = CTravelInfo(target, source);
  }

  void CTradeJunction::TravelUpdate(std::shared_ptr<CEntity> ship, float const power) {
    if(mTravels.find(ship) == mTravels.end()) {
      return;
    }
    auto& info = mTravels[ship];
    info.AddPower(power, mPowerPerUnit);
    
    if(info.GetProgress() > 1.0f) {
      auto target = info.GetTarget();
      auto& targetDock = target->GetComponent<comp::CDock>();

      targetDock.DockShip(ship);
      mTravels.erase(ship);
    }
  }
}
