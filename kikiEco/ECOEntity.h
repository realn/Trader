#pragma once

#include <memory>
#include <set>
#include <glm/vec2.hpp>

#include "ECOComponent.h"

namespace eco {
  class CUniverse;

  class CEntity
    : public std::enable_shared_from_this<CEntity>
  {
  public:
    using ComponentsT = std::map<core::CFastId, std::unique_ptr<CComponent>>;
    using ComponentIdsT = std::set<core::CFastId>;

  private:
    std::weak_ptr<CUniverse> mUniverse;
    cb::string mTypeId;
    cb::string mId;
    glm::vec2 mPosition;
    ComponentsT mComponents;
    ComponentIdsT mComponentIds;

  public:
    CEntity(cb::string const& typeId, cb::string const& id = cb::string());
    CEntity(CEntity&&) = default;
    ~CEntity();

    void UpdateComponents(float const timeDelta);

    void SetId(cb::string const& id) { mId = id; }
    void SetPosition(glm::vec2 const& value) { mPosition = value; }
    void SetUniverse(std::shared_ptr<CUniverse> value) { mUniverse = value; }

    cb::string GetTypeId() const { return mTypeId; }
    cb::string GetId() const { return mId; }
    glm::vec2 GetPosition() const { return mPosition; }
    std::shared_ptr<CUniverse> GetUniverse() const { return mUniverse.lock(); }

    void SetComponent(std::unique_ptr<CComponent> component);
    bool HasComponent(core::CFastId const& id) const;
    bool HasComponents(core::FastIdsT const& ids) const;
    CComponent& GetComponent(core::CFastId const& id) const;

    void PrintInfo(cb::ostream& stream, ComponentIdsT const& componentIds = ComponentIdsT()) const;

    template<class _Type, class ... _Args>
    void SetComponent(_Args const&... args) {
      SetComponent(std::make_unique<_Type>(shared_from_this(), args...));
    }

    template<class _Type>
    bool HasComponent() {
      return HasComponent(GetComponentId<_Type>());
    }

    template<class _Type, class ... _Types>
    bool HasComponents() {
      auto list = core::FastIdsT{ GetComponentId<_Type>(), GetComponentId<_Types>()... };
      return HasComponents(list);
    }

    template<class _Type>
    _Type& GetComponent() const {
      return dynamic_cast<_Type&>(GetComponent(GetComponentId<_Type>()));
    }

  private:
  };
}