#include "stdafx.h"
#include "ECOEntity.h"
#include "ECOCompMarket.h"
#include "ECOCompIndustry.h"

namespace eco {
  static const auto COMP_INDUSTRY_ID = L"Industry"s;

  std::weak_ptr<CFactoryTemplateRegistry> CFactoryTemplateRegistry::mInstance;

  CFactoryTemplate::CFactoryTemplate() {}

  CFactoryTemplate::CFactoryTemplate(ProductMulsT const & inputs, ProductMulsT const & outputs)
    : mInputs(inputs), mOutputs(outputs) {}

  CFactoryTemplate::~CFactoryTemplate() {}


  CFactoryTemplateRegistry::CFactoryTemplateRegistry() {}
  CFactoryTemplateRegistry::~CFactoryTemplateRegistry() {}

  std::shared_ptr<CFactoryTemplateRegistry> CFactoryTemplateRegistry::GetInstance() {
    if(mInstance.expired()) {
      auto instance = std::make_shared<CFactoryTemplateRegistry>();
      mInstance = instance;
      return instance;
    }
    return mInstance.lock();
  }

  void CFactoryTemplateRegistry::Register(cb::string const & id, CFactoryTemplate const & factoryTemplate) {
    mTemplates[id] = factoryTemplate;
  }

  CFactoryTemplate CFactoryTemplateRegistry::Get(cb::string const & id) const {
    return mTemplates.at(id);
  }

  CFactory::CFactory(CFactoryTemplate const& factoryTemplate, size_t const size)
    : CFactoryTemplate(factoryTemplate), mSize(size)
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
    CIndustry::CIndustry(std::shared_ptr<CEntity> parent, cb::strvector const& factories) 
      : CComponent(parent, COMP_INDUSTRY_ID)
    {
      if(!factories.empty()) {
        auto registry = CFactoryTemplateRegistry::GetInstance();
        for(auto& factoryId : factories) {
          AddFactory(registry->Get(factoryId));
        }
      }
    }

    CIndustry::~CIndustry() {}

    void CIndustry::AddFactory(CFactoryTemplate const & factoryTemplate) {
      mFactories.push_back(CFactory(factoryTemplate));
    }

    void CIndustry::Update(float const timeDelta) {
      auto parent = GetParent();
      if(!parent->HasComponent<comp::CMarket>())
        return;

      auto& market = parent->GetComponent<comp::CMarket>();
      for(auto& factory : mFactories) {
        factory.Update(market, timeDelta);
      }
    }
  }


  template<>
  cb::string const& GetComponentId<comp::CIndustry>() {
    return COMP_INDUSTRY_ID;
  }
}

