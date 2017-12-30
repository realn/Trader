#pragma once


#include <memory>
#include <array>
#include <vector>
#include <set>

namespace eco {
  class CEntity;
  class CUniverse;

  class CTravelInfo {
  private:
    std::shared_ptr<CEntity> mTarget;
    std::shared_ptr<CEntity> mSource;
    float mProgress = 0.0f;

  public:
    CTravelInfo(std::shared_ptr<CEntity> target, std::shared_ptr<CEntity> source)
      : mTarget(target), mSource(source) {}

    std::shared_ptr<CEntity> GetSource() const { return mSource; }
    std::shared_ptr<CEntity> GetTarget() const { return mTarget; }

    void AddPower(float const power, float const powerPerUnit);

    float GetDistance() const;
    float GetPowerNeeded(float const powerPerUnit) const;
    float GetProgress() const { return mProgress; }
    float GetPowerCurrent(float const powerPerUnit) const;
  };

  class CTradeJunction {
  public:
    using TargetsT = std::set<std::shared_ptr<CEntity>>;
    using TravelsT = std::map<std::shared_ptr<CEntity>, CTravelInfo>;

  private:
    TargetsT mTargets;
    TravelsT mTravels;
    float mPowerPerUnit = 1.0f;

  public:
    CTradeJunction();

    void AddTarget(std::shared_ptr<CEntity> dockable);
    void TravelTo(std::shared_ptr<CEntity> ship, std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target);
    void TravelUpdate(std::shared_ptr<CEntity> ship, float const power);

    TargetsT const& GetTargets() const { return mTargets; }
  };
}
