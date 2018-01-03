#include "stdafx.h"
#include "ECOEntity.h"
#include "ECOCompMarket.h"
#include "ECOCompIndustry.h"

namespace eco {
  static const auto COMP_INDUSTRY_ID = L"Industry"s;

  CFactory::CFactory(CFactoryTemplate const& factoryTemplate, size_t const size) 
    : CFactoryTemplate(factoryTemplate)
    , mSize(size)
  {}

  void CFactory::Update(comp::CMarket & market, float const timeDelta) {
    for(auto i = 0u; i < mSize; i++) {
      if(GetInputProducts(market, timeDelta)) {
        PutOutputProducts(market, timeDelta);
      }
    }
  }

  bool CFactory::GetInputProducts(comp::CMarket & market, float const timeDelta) {
    for(auto& item : mInputs) {
      if(!market.GetStorage().CanRemove(item.first, item.second * timeDelta))
        return false;
    }
    for(auto& item : mInputs) {
      market.RemProduct(item.first, item.second * timeDelta);
    }
    return true;
  }

  void CFactory::PutOutputProducts(comp::CMarket & market, float const timeDelta) {
    for(auto& item : mOutputs) {
      market.AddProduct(item.first, item.second * timeDelta);
    }
  }

  namespace comp {
    CIndustry::CIndustry(std::shared_ptr<CEntity> parent) 
      : CComponent(parent, COMP_INDUSTRY_ID)
    {}

    CIndustry::~CIndustry() {}

    void CIndustry::SetFactory(cb::string const & id, CFactoryTemplate const & factoryTemplate) {
      mFactories[id] = CFactory(factoryTemplate);
    }

    void CIndustry::Update(float const timeDelta) {
      auto parent = GetParent();
      if(!parent->HasComponent<comp::CMarket>())
        return;

      auto& market = parent->GetComponent<comp::CMarket>();
      for(auto& factory : mFactories) {
        factory.second.Update(market, timeDelta);
      }
    }
  }


  template<>
  cb::string const& GetComponentId<comp::CIndustry>() {
    return COMP_INDUSTRY_ID;
  }
}

