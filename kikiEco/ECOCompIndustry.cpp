#include "stdafx.h"

#include <CBXml/Serialize.h>

#include "ECOEntity.h"
#include "ECOCompMarket.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompIndustry.h"

namespace eco {
  static const auto COMP_INDUSTRY_ID = L"Industry"s;

  std::weak_ptr<CFactoryTemplateRegistry> CFactoryTemplateRegistry::mInstance;

  CFactoryTemplate::CFactoryTemplate() {}

  CFactoryTemplate::CFactoryTemplate(cb::string const& id,
                                     cb::string const& name, 
                                     ProductMulsT const & inputs, 
                                     ProductMulsT const & outputs)
    : mId(id), mName(name), mInputs(inputs), mOutputs(outputs) {}

  CFactoryTemplate::~CFactoryTemplate() {}

  void CFactoryTemplate::PrintProducts(cb::ostream & stream) const {
    PrintInfo(L"In"s, mInputs, stream);
    PrintInfo(L"Out"s, mOutputs, stream);
  }

  void CFactoryTemplate::PrintInfo(cb::string const & name, ProductMulsT const& products, cb::ostream & stream) const {
    stream << L"   "s << name << L": "s;
    if(products.empty()) {
      stream << L"[None]"s;
    }
    else {
      auto result = cb::strvector();
      for(auto& item : products) {
        result.push_back(cb::format(L"{0}({1})"s, item.first, item.second));
      }
      stream << cb::join(result, L", "s);
    }
    stream << std::endl;
  }


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

  void CFactoryTemplateRegistry::Register(CFactoryTemplate const & factoryTemplate) {
    mTemplates[factoryTemplate.GetId()] = factoryTemplate;
  }

  CFactoryTemplate CFactoryTemplateRegistry::Get(cb::string const & id) const {
    return mTemplates.at(id);
  }

  CFactory::CFactory(CFactoryTemplate const& factoryTemplate, size_t const size)
    : CFactoryTemplate(factoryTemplate), mSize(size)
  {}

  void CFactory::Update(comp::CMarket & market, float const timeDelta) {
    mMisses.clear();
    mProduction.clear();
    for(auto i = 0u; i < mSize; i++) {
      if(GetInputProducts(market, timeDelta)) {
        PutOutputProducts(market, timeDelta);
      }
    }
  }

  bool CFactory::GetInputProducts(comp::CMarket & market, float const timeDelta) {
    for(auto& item : mInputs) {
      if(!market.GetStorage().CanRemove(item.first, item.second * timeDelta)) {
        mMisses[item.first]++;
        return false;
      }
    }
    for(auto& item : mInputs) {
      market.RemProduct(item.first, item.second * timeDelta);
      mProduction[item.first]--;
    }
    return true;
  }

  void CFactory::PutOutputProducts(comp::CMarket & market, float const timeDelta) {
    for(auto& item : mOutputs) {
      market.AddProduct(item.first, item.second * timeDelta);
      mProduction[item.first]++;
    }
  }

  void CFactory::PrintInfo(cb::ostream & stream) const {
    stream << L"  Factory: " << mName << std::endl;
    PrintProducts(stream);
  }

  namespace xml {
    struct CIndustry : public CComponent {
      cb::strvector mFactories;
    };
    template<>
    void RegisterComponent<comp::CIndustry>() {
      CComponentFactory::GetInstance()->Register<CIndustry>(COMP_INDUSTRY_ID);
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

    CIndustry::CIndustry(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CComponent(parent, COMP_INDUSTRY_ID) {
      auto& industry = static_cast<xml::CIndustry const&>(component);
      if(!industry.mFactories.empty()) {
        auto registry = CFactoryTemplateRegistry::GetInstance();
        for(auto& factoryId : industry.mFactories) {
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
      if(!parent->HasComponent<comp::CMarket>()) {
        return;
      }

      mProductions.clear();
      mMisses.clear();

      auto& market = parent->GetComponent<comp::CMarket>();
      for(auto& factory : mFactories) {
        factory.Update(market, timeDelta);

        mProductions += factory.GetProduction();
        mMisses += factory.GetMisses();
      }
    }

    void CIndustry::PrintInfo(cb::ostream & stream) const {
      stream << L" Industry:"s << std::endl;
      for(auto& factory : mFactories) {
        factory.PrintInfo(stream);
      }
      stream << L"  Production:"s;
      for(auto& item : mProductions.mProducts) {
        
      }
    }
  }

  template<>
  cb::string const& GetComponentId<comp::CIndustry>() {
    return COMP_INDUSTRY_ID;
  }
}

const auto XML_COMPONENT_FACTORIES = L"Factories"s;

CB_DEFINEXMLREAD(eco::xml::CIndustry) {
  if(!ReadXmlObject<eco::xml::CComponent>(mNode, mObject)) { return false; }

  auto factories = cb::string();
  if(GetAttribute(XML_COMPONENT_FACTORIES, factories)) {
    mObject.mFactories = cb::split(factories, L","s, true);
  }
  return true;
}
