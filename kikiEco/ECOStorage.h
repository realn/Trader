#pragma once

#include <map>

#include <CBIO\Defines.h>
#include <CBStr\Defines.h>

#include "ECOProduct.h"

namespace eco {
  class CStorage {
  public:
    using ValuesT = std::map<ProductId, float>;

  private:
    ValuesT mValues;

  public:
    CStorage(ValuesT const& initStorate = ValuesT());

    void SetStorage(ValuesT const& values) { mValues = values; }

    void AddProduct(ProductId const& id, float const amount);
    void RemProduct(ProductId const& id, float const amount);

    float GetProductAmount(ProductId const& id) const;

    bool CanRemove(ProductId const& id, float const amount) const;

    void PrintInfo(cb::ostream& stream) const;
  };
}