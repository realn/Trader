#include "stdafx.h"
#include "ECOPriceList.h"

namespace eco {
  void CPriceList::SetValue(cb::string const & productId, float const value) {
    if(value == 0.0f) {
      mValues.erase(productId);
    }
    else if(value < 0.0f) {
      throw std::exception("Cannot set negative value on product.");
    }
    else {
      mValues[productId] = value;
    }
  }

  float CPriceList::GetValue(cb::string const & productId) const {
    auto it = mValues.find(productId);
    if(it == mValues.end()) {
      return 0.0f;
    }
    return it->second;
  }

  void CPriceList::PrintInfo(cb::ostream & stream) const {
    if(!mValues.empty()) {
      stream << L"  Prices:"s << std::endl;
      for(auto& item : mValues) {
        stream << L"   "s << item.first << L": " << item.second << L"$"s << std::endl;
      }
    }
  }

}
