#pragma once

#include <memory>
#include <set>
#include <glm/vec2.hpp>

#include "ECOComponent.h"

namespace eco {
  class CEntity
    : public std::enable_shared_from_this<CEntity>
  {
  public:
    using ComponentsT = std::map<cb::string, std::unique_ptr<CComponent>>;
    using ComponentIdsT = std::set<cb::string>;

  private:
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

    cb::string GetTypeId() const { return mTypeId; }
    cb::string GetId() const { return mId; }
    glm::vec2 GetPosition() const { return mPosition; }

    void SetComponent(std::unique_ptr<CComponent> component);
    bool HasComponent(cb::string const& id) const;
    bool HasComponents(cb::strvector const& ids) const;
    CComponent& GetComponent(cb::string const& id) const;

    void PrintInfo(cb::ostream& stream, ComponentIdsT const& componentIds = ComponentIdsT()) const;

    template<class _Type, class ... _Args>
    void SetComponent(_Args const&... args) {
      SetComponent(std::make_unique<_Type>(shared_from_this(), args...));
    }

    template<class _Type>
    bool HasComponent() {
      return HasComponent(GetComponentId<_Type>());
    }
    template<class _Type>
    _Type& GetComponent() const {
      return dynamic_cast<_Type&>(GetComponent(GetComponentId<_Type>()));
    }

  private:
  };
}