#include "stdafx.h"
#include "ECOStorage.h"

namespace eco {
  CStorage::CStorage(ValuesT const& initStorage)
    : mValues(initStorage) {}

  void CStorage::AddProduct(ProductId const & id, float const amount) {
    if(amount <= 0.0f) {
      throw std::exception("Cannot add negative or zero number.");
    }
    // zero value initialization on key miss.
    mValues[id] += amount;
  }

  void CStorage::RemProduct(ProductId const & id, float const amount) {
    if(amount < 0.0f) {
      throw std::exception("Cannot remove negative number.");
    }
    mValues[id] -= amount;
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

  void CStorage::PrintInfo(cb::ostream & stream) const {
    if(!mValues.empty()) {
      stream << L"  Storage:" << std::endl;
      for(auto& item : mValues) {
        stream << L"   "s << item.first.GetName() << L" x "s << item.second << std::endl;
      }
    }
  }
}
