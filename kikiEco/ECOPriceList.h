#pragma once

#include <map>

#include <CBIO\Defines.h>
#include <CBStr\Defines.h>

namespace eco {
  enum class PriceType {
    SELL, BUY
  };

  class CPrice {
  private:
    float mValue = 0.0f;
    PriceType mType = PriceType::SELL;

  public:
    void SetValue(float const value) { mValue = value; }
    void SetType(PriceType const value) { mType = value; }

    float GetValue() const { return mValue; }
    PriceType GetType() const { return mType; }
  };

  class CPriceList {
  public:
    using ValuesT = std::map<cb::string, CPrice>;

  private:
    ValuesT mValues;

  public:
    CPrice const& operator[](cb::string const& id) const { return mValues.at(id); }
    CPrice& operator[](cb::string const& id) { return mValues[id]; }

    ValuesT::iterator begin() { return mValues.begin(); }
    ValuesT::const_iterator begin() const { return mValues.begin(); }

    ValuesT::iterator end() { return mValues.end(); }
    ValuesT::const_iterator end() const { return mValues.end(); }

    size_t size() const { return mValues.size(); }
    bool empty() const { return mValues.empty(); }

    void clear() { mValues.clear(); }

    float GetValue(cb::string const& id, PriceType const type) const;
    void RemovePrice(cb::string const& id);

    bool Contains(cb::string const& productId) const { return mValues.find(productId) != mValues.end(); }

    void PrintInfo(cb::ostream& stream) const;
  };
}