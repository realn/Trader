#include "stdafx.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"

namespace eco {
  namespace xml {
    std::shared_ptr<CComponentFactory> CComponentFactory::mInstance;

    std::shared_ptr<CComponentFactory> CComponentFactory::GetInstance() {
      if(!mInstance) {
        mInstance = std::make_shared<CComponentFactory>();
      }
      return mInstance;
    }

    std::shared_ptr<CComponent> CComponentFactory::Create(cb::CXmlNode const & node) const {
      return mFactories.at(node.GetName())->Create(node);
    }
  }
}
