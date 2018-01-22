#pragma once

#include <memory>

#include <CBStr\Defines.h>

namespace eco {

  class CEntity;

  class CComponent {
  private:
    std::weak_ptr<CEntity> mParent;
    cb::string mId;

  public:
    CComponent(std::shared_ptr<CEntity> parent, cb::string const& id);
    CComponent(CComponent&&) = default;
    virtual ~CComponent();

    std::shared_ptr<CEntity> GetParent() const;
    cb::string const& GetId() const { return mId; }

    virtual void Update(const float timeDelta) {}
  };

  template<class _Type>
  cb::string const& GetComponentId() {
    static_assert(false && "Not implemented.");
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
