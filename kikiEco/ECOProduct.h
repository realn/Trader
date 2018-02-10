#pragma once

#include <map>

#include <CBStr\Defines.h>

namespace eco {
  class CProduct {
  private:
    cb::string mId;
    cb::string mName;

  public:
    CProduct(cb::string const& id, cb::string const& name) : mId(id), mName(name) {}

    cb::string GetId() const { return mId; }
    cb::string GetName() const { return mName; }

    bool operator==(CProduct const& other) const { return mId == other.mId; }
    bool operator!=(CProduct const& other) const { return mId != other.mId; }
  };

  class CProductIdValueList {
  public:
    using ListT = std::map<cb::string, float>;

  private:
    ListT mList;

  public:
    ListT::iterator begin() { return mList.begin(); }
    ListT::const_iterator begin() const { return mList.begin(); }

    ListT::iterator end() { return mList.end(); }
    ListT::const_iterator end() const { return mList.end(); }

    void clear() { mList.clear(); }
    size_t size() const { return mList.size(); }
    bool empty() const { return mList.empty(); }

    float& operator[](cb::string const& id) { return mList[id]; }
    float operator[](cb::string const& id) const { return mList.at(id); }

    void operator+=(CProductIdValueList const& other) {
      for(auto& item : other.mList) {
        mList[item.first] += item.second;
      }
    }
    void operator-=(CProductIdValueList const& other) {
      for(auto& item : other.mList) {
        mList[item.first] -= item.second;
      }
    }
  };
}