#pragma once

#include <memory>
#include <vector>

#include <CBStr/Defines.h>

namespace eco {
  class CEntity;
  class CTradeJunction;

  template<class _Type> cb::string const& GetComponentId();
  template<class _Type, class ... _Types> cb::strvector GetComponentIds();

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

    void SetMaxJunctionDistance(float const value) { mMaxJunctionDist = value; }
    float GetMaxJunctionDistance() const { return mMaxJunctionDist; }

    void UpdateEntities(float const timeDelta);

    void AddEntity(std::shared_ptr<CEntity> entity);

    std::shared_ptr<CEntity> FindEntity(cb::string const& id) const;
    std::shared_ptr<CEntity> FindEntity(cb::string const& id, cb::string const& requiredComponent) const;
    std::shared_ptr<CEntity> FindEntity(cb::string const& id, cb::strvector const& requiredComponents) const;

    EntitiesT const& GetEntities() const { return mEntities; }
    EntitiesT GetEntities(cb::strvector const& requiredComponents) const;
    EntitiesT GetEntities(cb::string const& requiredComponent) const;
    JunctionsT const& GetJunctions() const { return mJunctions; }

    template<class _Type, class ... _Types>
    std::shared_ptr<CEntity> FindEntity(cb::string const& id) const {
      return FindEntity(id, GetComponentIds<_Type, _Types...>());
    }

    template<class _Type, class ... _Types>
    EntitiesT GetEntities() const {
      return GetEntities(GetComponentIds<_Type, _Types...>());
    }
  };
}