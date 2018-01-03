#pragma once

#include <map>

#include <CBStr\Defines.h>

namespace eco {
  class CPriceList {
  public:
    using ValuesT = std::map<cb::string, float>;

  private:
    ValuesT mValues;

  public:
    void SetValue(cb::string const& productId, float const value);
    float GetValue(cb::string const& productId) const;

    bool Contains(cb::string const& productId) const { return mValues.find(productId) != mValues.end(); }
  };
}