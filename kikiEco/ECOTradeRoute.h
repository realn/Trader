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
    std::weak_ptr<CUniverse> mUniverse;
    TargetsT mTargets;
  };

  class CTradeRoute {
  public:
    using JunctionsT = std::vector<std::shared_ptr<CTradeJunction>>;

  private:
    JunctionsT mJunctions;

  public:
  };

}