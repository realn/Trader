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
    cb::string mId;
    cb::string mName;
    ProductMulsT mInputs;
    ProductMulsT mOutputs;

  public:
    CFactoryTemplate();
    CFactoryTemplate(cb::string const& id, cb::string const& name, ProductMulsT const& inputs, ProductMulsT const& outputs);
    virtual ~CFactoryTemplate();

    cb::string const& GetId() const { return mId; }
    cb::string const& GetName() const { return mName; }

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

    void Register(CFactoryTemplate const& factoryTemplate);
    CFactoryTemplate Get(cb::string const& id) const;
  };

  class CFactory
    : public CFactoryTemplate
  {
  private:
    size_t mSize;

  public:
    CFactory(CFactoryTemplate const& factoryTemplate = CFactoryTemplate(), size_t const size = 1);

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

      void AddFactory(CFactoryTemplate const& factoryTemplate);

      void Update(float const timeDelta) override;

      void PrintInfo(cb::ostream& stream) const override;
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CIndustry>();
}