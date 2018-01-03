#include "stdafx.h"
#include "ECOStorage.h"

namespace eco {
  void CStorage::AddProduct(cb::string const & id, float const amount) {
    if(amount <= 0.0f) {
      throw std::exception("Cannot add negative or zero number.");
    }
    // zero value initialization on key miss.
    mValues[id] += amount;
  }

  void CStorage::RemProduct(cb::string const & id, float const amount) {
    if(amount < 0.0f) {
      throw std::exception("Cannot remove negative number.");
    }
    auto it = mValues.find(id);
    if(it == mValues.end()) {
      throw std::exception("Product not stored to remove.");
    }
    if(it->second < amount) {
      throw std::exception("Want to remove more than stored.");
    }
    it->second -= amount;
    if(it->second == 0.0f) {
      mValues.erase(it);
    }
  }

  float CStorage::GetProductAmount(cb::string const & id) const {
    auto it = mValues.find(id);
    if(it == mValues.end())
      return 0.0f;
    return it->second;
  }

  bool CStorage::CanRemove(cb::string const & id, float const amount) const {
    auto it = mValues.find(id);
    if(it == mValues.end()) {
      return false;
    }
    return it->second <= amount;
  }
}
