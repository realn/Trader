#pragma once

#include <map>

#include "ECOComponent.h"

namespace eco {
  namespace comp {
    class CMarket;
  }

  class CFactoryTemplate {
  public:
    using ProductMulsT = std::map<cb::string, float>;

  protected:
    ProductMulsT mInputs;
    ProductMulsT mOutputs;

  public:
    void SetInput(cb::string const& productId, float mult) { if(mult > 0.0f) mInputs[productId] = mult; else mInputs.erase(productId); }
    void SetOutput(cb::string const& productId, float mult) { if(mult > 0.0f) mOutputs[productId] = mult; else mOutputs.erase(productId); }
  };

  class CFactory
    : public CFactoryTemplate
  {
  private:
    size_t mSize;

  public:
    CFactory(CFactoryTemplate const& factoryTemplate = CFactoryTemplate(), size_t const size = 1);

    void Update(comp::CMarket& market, float const timeDelta);

  private:
    bool GetInputProducts(comp::CMarket& market, float const timeDelta);
    void PutOutputProducts(comp::CMarket& market, float const timeDelta);
  };

  namespace comp {
    class CIndustry 
      : public CComponent
    {
    public:
      using FactoriesT = std::map<cb::string, CFactory>;

    private:
      FactoriesT mFactories;

    public:
      CIndustry(std::shared_ptr<CEntity> parent);
      virtual ~CIndustry();

      void SetFactory(cb::string const& id, CFactoryTemplate const& factoryTemplate);

      void Update(float const timeDelta) override;
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CIndustry>();
}