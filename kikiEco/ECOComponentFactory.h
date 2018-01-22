#pragma once

#include "ECOComponent.h"
#include "ECOEntity.h"

namespace eco {
  class IComponentFactory {
  public:
    virtual ~IComponentFactory();

    virtual void SetComponentToEntity(CEntity& entity) const = 0;
  };

  template<class _Type>
  class CComponentFactory 
    : public IComponentFactory
  {
  public:
    CComponentFactory() {}
    virtual ~CComponentFactory() {}

    void SetComponentToEntity(CEntity& entity) const override {
      entity.SetComponent<_Type>();
    }
  };

  class CComponentFactoryRegistry 
    : public std::enable_shared_from_this<CComponentFactoryRegistry>
  {
  private:
    using FactoriesT = std::map<cb::string, std::shared_ptr<IComponentFactory>>;

    FactoriesT mFactories;
    static std::weak_ptr<CComponentFactoryRegistry> mInstance;

  public:
    CComponentFactoryRegistry();
    ~CComponentFactoryRegistry();

    static std::shared_ptr<CComponentFactoryRegistry> GetInstance();
    static std::shared_ptr<IComponentFactory> GetFactory(cb::string const& id);

    void Register(cb::string const& id, std::shared_ptr<IComponentFactory> factory) { mFactories[id] = factory; }
    std::shared_ptr<IComponentFactory> Get(cb::string const& id) const;

    template<class _Type>
    void Register() {
      Register(eco::GetComponentId<_Type>(), std::make_shared<CComponentFactory<_Type>>());
    }
  };
}
