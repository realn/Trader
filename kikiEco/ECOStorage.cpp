#include "stdafx.h"
#include "ECOStorage.h"

namespace eco {
  CStorage::CStorage(ValuesT const& initStorage, float const capacity)
    : mValues(initStorage)
    , mCapacity(capacity <= 0.0f ? -1.0f : capacity)
    , mSpace(mCapacity)
  {
  }

  void CStorage::AddProduct(ProductId const & id, float const amount) {
    if(amount <= 0.0f) {
      throw std::exception("Cannot add negative or zero number.");
    }
    // zero value initialization on key miss.
    mValues[id] += amount;
    if(mCapacity > 0.0f)
      mSpace -= amount;
  }

  void CStorage::RemProduct(ProductId const & id, float const amount) {
    if(amount < 0.0f) {
      throw std::exception("Cannot remove negative number.");
    }
    mValues[id] -= amount;
    if(mCapacity > 0.0f)
      mSpace += amount;
  }

  float CStorage::GetProductAmount(ProductId const & id) const {
    auto it = mValues.find(id);
    if(it == mValues.end())
      return 0.0f;
    return it->second;
  }

  bool CStorage::CanRemove(ProductId const & id, float const amount) const {
    auto it = mValues.find(id);
    if(it == mValues.end()) {
      return false;
    }
    return it->second >= amount;
  }

  bool CStorage::CanAdd(ProductId const & id, float const amount) const {
    if(mCapacity <= 0.0f)
      return true;
    return mSpace - amount >= 0.0f;
  }

  void CStorage::PrintInfo(cb::ostream & stream) const {
    if(!mValues.empty()) {
      stream << L"  Storage (";
      if(mCapacity > 0.0f) {
        stream << mSpace << L"/" << mCapacity;
      }
      else {
        stream << L"INF"s;
      }
      stream << L"):" << std::endl;
      for(auto& item : mValues) {
        if(item.second > 0.0f)
          stream << L"   "s << item.first.GetName() << L" x "s << item.second << std::endl;
      }
    }
  }
}
