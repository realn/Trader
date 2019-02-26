#pragma once

#include <memory>

#include <CBCore/Defines.h>

#include <CoreFastId.h>

namespace eco {
  namespace xml {
    struct CComponent;
  }

  class CEntity;

  class CComponent {
  private:
    std::weak_ptr<CEntity> mParent;
    core::CFastId mId;

  public:
    CComponent(std::shared_ptr<CEntity> parent, core::CFastId const& id);
    CComponent(CComponent&&) = default;
    virtual ~CComponent();

    std::shared_ptr<CEntity> GetParent() const;
    core::CFastId const& GetId() const { return mId; }

    virtual void Update(const float timeDelta) {}

    virtual void PrintInfo(cb::ostream& stream) const {}
  };

  template<class _Type>
  core::CFastId const& GetComponentId() {
    static_assert(false && "Not implemented.");
  }

  template<class _Type, class ... _Types>
  core::FastIdsT GetComponentIds() {
    return { GetComponentId<_Type>(), GetComponentId<_Types>()... };
  }

  namespace xml {
    template<class _Type>
    void RegisterComponent() {
      static_assert(false && "Unsupported by xml component.");
    }
  }

  template<class _Type>
  struct weak_ptr_less {
    bool operator()(const std::weak_ptr<_Type> left, const std::weak_ptr<_Type> right) const {
      auto lptr = left.lock(), rptr = right.lock();
      if(!rptr) return false;
      if(!lptr) return true;
      return lptr.get() < rptr.get();
    }
  };
}
