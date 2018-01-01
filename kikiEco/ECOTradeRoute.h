#pragma once


#include <memory>
#include <array>
#include <vector>
#include <set>
#include <glm\vec2.hpp>

namespace eco {
  class CEntity;
  class CUniverse;

  class CTravelInfo {
  private:
    std::shared_ptr<CEntity> mTarget;
    std::shared_ptr<CEntity> mSource;
    float mProgress = 0.0f;

  public:
    CTravelInfo() = default;
    CTravelInfo(std::shared_ptr<CEntity> target, std::shared_ptr<CEntity> source)
      : mTarget(target), mSource(source) {}

    std::shared_ptr<CEntity> GetSource() const { return mSource; }
    std::shared_ptr<CEntity> GetTarget() const { return mTarget; }

    void AddPower(float const power, float const powerPerUnit);

    float GetDistance() const;
    float GetPowerNeeded(float const powerPerUnit) const;
    float GetProgress() const { return mProgress; }
    float GetPowerCurrent(float const powerPerUnit) const;
    glm::vec2 GetPosition() const;

    bool IsFinished() const { return mProgress >= 1.0f; }
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
    CTradeJunction(std::shared_ptr<CEntity> targetA, std::shared_ptr<CEntity> targetB);

    void AddTarget(std::shared_ptr<CEntity> dockable);
    void TravelTo(std::shared_ptr<CEntity> ship, std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target);
    CTravelInfo TravelUpdate(std::shared_ptr<CEntity> ship, float const power);

    TargetsT const& GetTargets() const { return mTargets; }
  };
}
