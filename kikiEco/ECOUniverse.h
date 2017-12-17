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

  public:
    CUniverse() = default;
    ~CUniverse();

    void AddEntity(CEntity&& entity);

    EntitiesT const& GetEntities() const { return mEntities; }
    JunctionsT const& GetJunctions() const { return mJunctions; }
  };
}