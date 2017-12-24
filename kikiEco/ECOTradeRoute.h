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
    float mProgress;

    CTravelInfo(std::shared_ptr<CEntity> target, std::shared_ptr<CEntity> source);
  };

  class CTradeJunction {
  public:
    using TargetsT = std::set<std::shared_ptr<CEntity>>;
    using TravelsT = std::map<std::shared_ptr<CEntity>, CTravelInfo>;

  private:
    TargetsT mTargets;
    TravelsT mTravels;

  public:
    CTradeJunction(std::shared_ptr<CEntity> targetA, std::shared_ptr<CEntity> targetB);

    TargetsT const& GetTargets() const { return mTargets; }
  };
}
