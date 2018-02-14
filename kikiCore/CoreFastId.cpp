#include "stdafx.h"
#include "CoreFastId.h"

namespace core {
  static auto mIdList = std::vector<cb::string>();

  CFastId::CFastId(cb::string const & id) : mName(id), mId(0) {
    if(!id.empty()) {
      for(; mId < mIdList.size(); mId++) {
        if(mIdList[mId] == mName) {
          return;
        }
      }
      mIdList.push_back(mName);
    }
  }

}
