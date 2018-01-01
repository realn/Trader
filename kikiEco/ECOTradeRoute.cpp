#include "stdafx.h"
#include "ECOTradeRoute.h"
#include "ECOCompDock.h"
#include "ECOEntity.h"

namespace eco {
  void CTravelInfo::AddPower(float const power, float const powerPerUnit) {
    auto powerCurrent = GetPowerCurrent(powerPerUnit) + power;
    auto powerNeeded = GetPowerNeeded(powerPerUnit);

    mProgress = glm::clamp(powerCurrent/powerNeeded, 0.0f, 1.0f);
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

  glm::vec2 CTravelInfo::GetPosition() const {
    return glm::mix(mSource->GetPosition(), mTarget->GetPosition(), mProgress);
  }

  CTradeJunction::CTradeJunction() {}

  CTradeJunction::CTradeJunction(std::shared_ptr<CEntity> targetA, std::shared_ptr<CEntity> targetB) {
    AddTarget(targetA);
    AddTarget(targetB);
  }

  void CTradeJunction::AddTarget(std::shared_ptr<CEntity> dockable) {
    if(!dockable->HasComponent<comp::CDock>()) {
      throw std::exception("Target entity is not dockable for juntions.");
    }
    mTargets.insert(dockable);
  }

  void CTradeJunction::TravelTo(std::shared_ptr<CEntity> ship, std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target) {
    if(mTravels.find(ship) != mTravels.end()) {
      throw std::exception("Ship is already traveling this junction.");
    }
    if(mTargets.find(target) == mTargets.end() || mTargets.find(source) == mTargets.end()) {
      throw std::exception("Target/Source is not placed on this junction.");
    }
    if(source == target) {
      throw std::exception("Target and source cannot be the same.");
    }
    auto& sourceDock = source->GetComponent<comp::CDock>();
    if(sourceDock.GetDockedShips().find(ship) == sourceDock.GetDockedShips().end()) {
      throw std::exception("Ship is not docked on source.");
    }

    sourceDock.UnDockShip(ship);
    mTravels[ship] = CTravelInfo(target, source);
  }

  CTravelInfo CTradeJunction::TravelUpdate(std::shared_ptr<CEntity> ship, float const power) {
    if(mTravels.find(ship) == mTravels.end()) {
      throw std::exception("No travel for ship entity on this junction.");
    }
    mTravels[ship].AddPower(power, mPowerPerUnit);

    auto info = mTravels[ship];
    if(info.IsFinished()) {
      auto target = info.GetTarget();
      auto& targetDock = target->GetComponent<comp::CDock>();

      targetDock.DockShip(ship);
      mTravels.erase(ship);
    }
    return info;
  }
}
