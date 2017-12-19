#pragma once


#include <memory>
#include <array>
#include <vector>

namespace eco {
  class CEntity;
  class CUniverse;

  class CTradeJunction {
  public:
    using TargetsT = std::array<std::shared_ptr<CEntity>, 2>;

  private:
    TargetsT mTargets;

  public:
    CTradeJunction(std::shared_ptr<CEntity> targetA, std::shared_ptr<CEntity> targetB);

    std::shared_ptr<CEntity> GetTargetA() const { return mTargets[0]; }
    std::shared_ptr<CEntity> GetTargetB() const { return mTargets[1]; }

    TargetsT const& GetTargets() const { return mTargets; }
  };
}