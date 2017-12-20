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
  };

  template<class _Type>
  cb::string const& GetComponentId() {
    static_assert(false && "Not implemented.");
  }
}