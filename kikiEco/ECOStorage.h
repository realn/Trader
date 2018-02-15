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
    float const mCapacity;
    float mSpace;

  public:
    CStorage(ValuesT const& initStorate = ValuesT(), float const capacity = 500.0f);

    void SetStorage(ValuesT const& values) { mValues = values; }

    void AddProduct(ProductId const& id, float const amount);
    void RemProduct(ProductId const& id, float const amount);

    float GetProductAmount(ProductId const& id) const;
    float GetCapacity() const { return mCapacity; }
    float GetSpace() const { return mSpace; }
    float GetEmpty() const { return mSpace / mCapacity; }
    float GetFill() const { return 1.0f - GetEmpty(); }

    bool CanRemove(ProductId const& id, float const amount) const;
    bool CanAdd(ProductId const& id, float const amount) const;

    void PrintInfo(cb::ostream& stream) const;
  };
}