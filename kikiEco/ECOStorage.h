#pragma once

#include <map>

#include <CBIO\Defines.h>
#include <CBStr\Defines.h>

namespace eco {
  class CStorage {
  public:
    using ValuesT = std::map<cb::string, float>;

  private:
    ValuesT mValues;

  public:
    CStorage(ValuesT const& initStorate = ValuesT());

    void AddProduct(cb::string const& id, float const amount);
    void RemProduct(cb::string const& id, float const amount);

    float GetProductAmount(cb::string const& id) const;

    bool CanRemove(cb::string const& id, float const amount) const;

    void PrintInfo(cb::ostream& stream) const;
  };
}