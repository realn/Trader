#pragma once

#include <memory>

#include <CBIO/Defines.h>
#include <CBStr/Defines.h>

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

    virtual void PrintInfo(cb::ostream& stream) const {}
  };

  template<class _Type>
  cb::string const& GetComponentId() {
    static_assert(false && "Not implemented.");
  }

  template<class _Type, class ... _Types>
  cb::strvector GetComponentIds() {
    return { GetComponentId<_Type>(), GetComponentId<_Types>()... };
  }

  //template<class _Type, class ... _Types>
  //cb::strvector GetComponentIds(cb::strvector& list = cb::strvector()) {
  //  list.push_back(GetComponentId<_Type>());
  //  return GetComponentIds<_Types...>(list);
  //}

  //cb::strvector GetComponentIds(cb::strvector& list = cb::strvector()) {
  //  return list;
  //}

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
