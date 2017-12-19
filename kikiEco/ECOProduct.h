#pragma once

#include <CBStr\Defines.h>

namespace eco {
  class CProduct {
  private:
    cb::string mId;
    cb::string mName;

  public:
    CProduct(cb::string const& id, cb::string const& name) : mId(id), mName(name) {}

    cb::string GetId() const { return mId; }
    cb::string GetName() const { return mName; }

    bool operator==(CProduct const& other) const { return mId == other.mId; }
    bool operator!=(CProduct const& other) const { return mId != other.mId; }
  };
}