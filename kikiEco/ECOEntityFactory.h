#pragma once

#include <tuple>

#include <CBStr\Defines.h>

#include "ECODefines.h"
#include "ECOComponent.h"

namespace eco {
  class IEntityComponentConfig {
  public:
    virtual ~IEntityComponentConfig() {}

    virtual void SetComponentToEntity(CEntity& entity) const = 0;
  };

  template<class _Type, class ... _Params>
  class CEntityComponentConfig
    : public IEntityComponentConfig {
  private:
    std::tuple<_Params...> mParams;

  public:
    CEntityComponentConfig(_Params&&... args) : mParams(std::forward<_Params>(args)...) {}
    virtual ~CEntityComponentConfig() {}

    void SetComponentToEntity(CEntity& entity) const override {
      SetComponentToEntity(entity, mParams);
    }

  private:
    template<class ... _Args>
    void SetComponentToEntity(CEntity& entity, std::tuple<_Params...> const& args) const {
      SetComponentToEntity(entity, args, helper::gen_seq<sizeof...(_Args)>{});
    }
    template<class ... _Args, int ... Is>
    void SetComponentToEntity(CEntity& entity, std::tuple<_Args...> const& params, helper::index<Is...>) const {
      entity.SetComponent<_Type>(std::get<Is>(params)...);
    }
  };

  class CEntityFactory {
  private:
    using ComponentsT = std::map<cb::string, std::unique_ptr<IEntityComponentConfig>>;
    cb::string mTypeId;
    ComponentsT mComponents;

  public:
    CEntityFactory(cb::string const& typeId);
    ~CEntityFactory();

    cb::string GetTypeId() const { return mTypeId; }

    template<class _Type, class ... _Params>
    void SetComponent(_Params&& ... args) {
      mComponents[GetComponentId<_Type>()] = std::make_unique<CEntityComponentConfig<_Type, _Params...>>(std::forward<_Params...>(args)...);
    }

    std::shared_ptr<CEntity> Create(cb::string const& id = cb::string());
  };

  class CEntityFactoryRegistry {
  private:
    using TypesT = std::map<cb::string, std::shared_ptr<CEntityFactory>>;
    TypesT mTypes;

    static std::weak_ptr<CEntityFactoryRegistry> mInstance;

  public:
    CEntityFactoryRegistry();
    ~CEntityFactoryRegistry();

    static std::shared_ptr<CEntityFactoryRegistry> GetInstance();

    void Register(std::shared_ptr<CEntityFactory> factory);
    std::shared_ptr<CEntityFactory> Register(cb::string const& typeId) {
      auto factory = std::make_shared<CEntityFactory>(typeId);
      Register(factory);
      return factory;
    }
    std::shared_ptr<CEntityFactory> Get(cb::string const& typeId) const;
  };
}