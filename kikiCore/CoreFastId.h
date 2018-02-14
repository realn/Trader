#pragma once

#include <vector>

#include <CBStr/Defines.h>

namespace core {
  class CFastId {
  private:
    cb::string mName;
    size_t mId;

  public:
    CFastId(cb::string const& id = cb::string());

    cb::string const& GetName() const { return mName; }
    size_t const& GetId() const { return mId; }

    bool operator==(CFastId const& other) const { return mId == other.mId; }
    bool operator!=(CFastId const& other) const { return mId != other.mId; }

    bool operator<(CFastId const& other) const { return mId < other.mId; }
    bool operator>(CFastId const& other) const { return mId > other.mId; }
  };

  using FastIdsT = std::vector<CFastId>;
}

inline core::CFastId operator "" _id(wchar_t const* szName, size_t const len) {
  return core::CFastId(cb::string(szName, len));
}