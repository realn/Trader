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
    CFactoryTemplate();
    CFactoryTemplate(ProductMulsT const& inputs, ProductMulsT const& outputs);
    virtual ~CFactoryTemplate();

    void PrintProducts(cb::ostream& stream) const;

  private:
    void PrintInfo(cb::string const& name, ProductMulsT const& products, cb::ostream& stream) const;
  };

  class CFactoryTemplateRegistry {
  private:
    using TemplatesT = std::map<cb::string, CFactoryTemplate>;

    TemplatesT mTemplates;
    static std::weak_ptr<CFactoryTemplateRegistry> mInstance;

  public:
    CFactoryTemplateRegistry();
    ~CFactoryTemplateRegistry();

    static std::shared_ptr<CFactoryTemplateRegistry> GetInstance();

    void Register(cb::string const& id, CFactoryTemplate const& factoryTemplate);
    CFactoryTemplate Get(cb::string const& id) const;
  };

  class CFactory
    : public CFactoryTemplate
  {
  private:
    cb::string mName;
    size_t mSize;

  public:
    CFactory(cb::string const& name, CFactoryTemplate const& factoryTemplate = CFactoryTemplate(), size_t const size = 1);

    void Update(comp::CMarket& market, float const timeDelta);
    void PrintInfo(cb::ostream& stream) const;

  private:
    bool GetInputProducts(comp::CMarket& market, float const timeDelta);
    void PutOutputProducts(comp::CMarket& market, float const timeDelta);

  };

  namespace comp {
    class CIndustry 
      : public CComponent
    {
    public:
      using FactoriesT = std::vector<CFactory>;

    private:
      FactoriesT mFactories;

    public:
      CIndustry(std::shared_ptr<CEntity> parent, cb::strvector const& factories = cb::strvector());
      virtual ~CIndustry();

      void AddFactory(cb::string const& name, CFactoryTemplate const& factoryTemplate);

      void Update(float const timeDelta) override;

      void PrintInfo(cb::ostream& stream) const override;
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CIndustry>();
}