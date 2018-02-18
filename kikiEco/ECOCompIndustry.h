#pragma once

#include <map>

#include "ECOProduct.h"
#include "ECOComponent.h"

namespace eco {
  namespace comp {
    class CMarket;
  }
  namespace xml {
    struct CComponent;
    struct CUniverse;
  }

  class CFactoryTemplate {
  protected:
    cb::string mId;
    cb::string mName;
    CProductIdValueList mInputs;
    CProductIdValueList mOutputs;

  public:
    CFactoryTemplate();
    CFactoryTemplate(cb::string const& id, 
                     cb::string const& name, 
                     CProductIdValueList const& inputs, 
                     CProductIdValueList const& outputs);
    virtual ~CFactoryTemplate();

    cb::string const& GetId() const { return mId; }
    cb::string const& GetName() const { return mName; }

    void PrintProducts(cb::ostream& stream) const;

    CProductIdValueList GetProduction() const;

  private:
    void PrintInfo(cb::string const& name, CProductIdValueList const& products, cb::ostream& stream) const;
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
    bool CanGetInputProducts(comp::CMarket const& market, float const timeDelta, float& outMult) const;
    bool CanPutOutputProducts(comp::CMarket const& market, float const timeDelta) const;

    void GetInputProducts(comp::CMarket& market, float const timeDelta);
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
      CProductIdValueList mProduction;

    public:
      CIndustry(std::shared_ptr<CEntity> parent, cb::strvector const& factories = cb::strvector());
      CIndustry(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      CIndustry(CIndustry&&) = default;
      virtual ~CIndustry();

      CProductIdValueList const& GetProduction() const { return mProduction; }

      void AddFactory(CFactoryTemplate const& factoryTemplate);

      void Update(float const timeDelta) override;

      void PrintInfo(cb::ostream& stream) const override;
    };
  }

  namespace xml {
    template<> void RegisterComponent<comp::CIndustry>();
  }

  template<>
  extern core::CFastId const& GetComponentId<comp::CIndustry>();
}
