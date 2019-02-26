#pragma once

#include <tuple>

#include <CBCore\Defines.h>

#include "ECODefines.h"
#include "ECOComponent.h"
#include "ECOComponentFactory.h"

namespace eco {
  namespace xml {
    struct CComponent;
    struct CUniverse;
  }

  class IEntityComponentConfig {
  public:
    virtual ~IEntityComponentConfig() {}
    virtual void SetComponentToEntity(CEntity& entity) const = 0;
  };

  class CEntityComponentConfigSimple
    : public IEntityComponentConfig {
  private:
    cb::string mId;

  public:
    CEntityComponentConfigSimple(cb::string const& id) : mId(id) {}
    virtual ~CEntityComponentConfigSimple() {}

    virtual void SetComponentToEntity(CEntity& entity) const override;
  };

  class CEntityComponentConfigXml 
    : public IEntityComponentConfig {
  private:
    cb::string mId;
    std::shared_ptr<xml::CComponent> mData;

  public:
    CEntityComponentConfigXml(cb::string const& id, 
                              std::shared_ptr<xml::CComponent> data);
    virtual ~CEntityComponentConfigXml();

    virtual void SetComponentToEntity(CEntity& entity) const override;
  };

  template<class _Type, class ... _Params>
  class CEntityComponentConfig
    : public IEntityComponentConfig {
  private:
    std::tuple<_Params...> mParams;

  public:
    CEntityComponentConfig(_Params&&... args) : mParams(std::forward<_Params>(args)...) {}
    CEntityComponentConfig(CEntityComponentConfig&&) = default;
    virtual ~CEntityComponentConfig() {}

    void SetComponentToEntity(CEntity& entity) const override {
      SetComponentToEntity(entity, std::index_sequence_for<_Params...>{});
    }

  private:
    template<size_t ... Is>
    void SetComponentToEntity(CEntity& entity, std::index_sequence<Is...>) const {
      entity.SetComponent<_Type, _Params...>(std::get<Is>(mParams)...);
    }
  };

  class CEntityFactory {
  private:
    using ComponentsT = std::map<cb::string, std::unique_ptr<IEntityComponentConfig>>;
    cb::string mTypeId;
    ComponentsT mComponents;

  public:
    CEntityFactory(cb::string const& typeId);
    CEntityFactory(CEntityFactory&&) = default;
    ~CEntityFactory();

    cb::string GetTypeId() const { return mTypeId; }

    void SetComponentById(cb::string const& id);

    void SetComponentByXml(std::shared_ptr<xml::CComponent> component);

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

    std::shared_ptr<CEntityFactory> Get(cb::string const& typeId) const;
    void Register(std::shared_ptr<CEntityFactory> factory);

    std::shared_ptr<CEntityFactory> Register(cb::string const& typeId) {
      auto factory = std::make_shared<CEntityFactory>(typeId);
      Register(factory);
      return factory;
    }
  };
}