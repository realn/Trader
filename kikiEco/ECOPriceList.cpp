#include "stdafx.h"
#include "ECOPriceList.h"

namespace eco {
  cb::string toStr(PriceType const type) {
    switch(type) {
    case PriceType::BUY:  return L"BUY"s;
    case PriceType::SELL: return L"SELL"s;
    default:
      return cb::string();
    }
  }

  float CPriceList::GetValue(cb::string const & id, PriceType const type) const {
    auto it = mValues.find(id);
    if(it != mValues.end() && it->second.GetType() == type) {
      return it->second.GetValue();
    }
    return 0.0f;
  }

  void CPriceList::RemovePrice(cb::string const & id) {
    auto it = mValues.find(id);
    if(it != mValues.end()) {
      mValues.erase(it);
    }
  }

  void CPriceList::PrintInfo(cb::ostream & stream) const {
    if(!mValues.empty()) {
      stream << L"  Prices:"s << std::endl;
      for(auto& item : mValues) {
        stream << L"   "s << item.first << L"("s << toStr(item.second.GetType());
        stream << L"): "s << item.second.GetValue() << L"$"s << std::endl;
      }
    }
  }

}
