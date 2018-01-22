#pragma once

#include <tuple>
#include <functional>

#include "ECOComponent.h"
#include "ECOEntity.h"

namespace eco {
  namespace helper {
    template<int... Is>
    struct index {};
    
    template<int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

    template<int ... Is>
    struct gen_seq<0, Is...> : index<Is...> {};
  }

  class IComponentFactory {
  public:
    virtual ~IComponentFactory();

    virtual void SetComponentToEntity(CEntity& entity) const = 0;
  };

  template<class _Type, class ... _Params>
  class CComponentFactory 
    : public IComponentFactory
  {
  private:
    std::tuple<_Params...> mParams;

  public:
    CComponentFactory(_Params&&... args) 
      : mParams(std::make_tuple<_Params...>(std::forward<_Params>(args)...))
    {}
    virtual ~CComponentFactory() {}

    void SetComponentToEntity(CEntity& entity) const override {
      SetComponentWithParams(entity, mParams);
    }

  private:
    template<class ... _Args>
    void SetComponentWithParams(CEntity& entity, std::tuple<_Args...> const& params) const {
      SetComponentWithParams(entity, params, helper::gen_seq<sizeof...(_Args)>{});
    }
    template<class ... _Args, int ... Is>
    void SetComponentWithParams(CEntity& entity, std::tuple<_Args...> const& params, helper::index<Is...>) const {
      entity.SetComponent<_Type>(std::get<Is>(params)...);
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

    template<class _Type, class ... _Args>
    void Register(_Args&&... args) {
      Register(eco::GetComponentId<_Type>(), std::make_shared<CComponentFactory<_Type, _Args...>>(std::forward<_Args...>(args)...));
    }
  };
}
