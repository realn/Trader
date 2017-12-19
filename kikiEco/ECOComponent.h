#pragma once

#include <CBStr\Defines.h>

namespace eco {
  class CComponent {
  private:
    cb::string mId;

  public:
    CComponent(cb::string const& id);
    CComponent(CComponent&&) = default;
    virtual ~CComponent();

    cb::string const& GetId() const { return mId; }
  };

  template<class _Type>
  cb::string const& GetComponentId() {
    static_assert(false && "Not implemented.");
  }
}