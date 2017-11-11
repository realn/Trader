#pragma once

#include <memory>
#include <vector>

namespace eco {
  class CEntity;
  class CUniverse {
  public:
    using EntityVecT = std::vector<std::shared_ptr<CEntity>>;

  private:
    EntityVecT mEntities;

  public:
    CUniverse() = default;

    EntityVecT const& GetEntities() const { return mEntities; }
  };
}