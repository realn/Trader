#pragma once

#include <memory>
#include <vector>

namespace eco {
  class CEntity;
  class CTradeJunction;
  class CUniverse {
  public:
    using EntitiesT = std::vector<std::shared_ptr<CEntity>>;
    using JunctionsT = std::vector<std::shared_ptr<CTradeJunction>>;

  private:
    EntitiesT mEntities;
    JunctionsT mJunctions;
    float mMaxJunctionDist = 7.0f;

  public:
    CUniverse() = default;
    CUniverse(CUniverse&&) = default;
    ~CUniverse();

    void UpdateEntities(float const timeDelta);

    void AddEntity(std::shared_ptr<CEntity> entity);

    EntitiesT const& GetEntities() const { return mEntities; }
    EntitiesT GetEntities(cb::strvector const& requiredComponents) const;
    EntitiesT GetEntities(cb::string const& requiredComponent) const;
    JunctionsT const& GetJunctions() const { return mJunctions; }
  };
}