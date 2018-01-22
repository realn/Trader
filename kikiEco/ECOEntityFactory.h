#pragma once

#include <CBStr\Defines.h>

namespace eco {
  class CEntity;

  class CEntityFactory {
  private:
    cb::string mTypeId;
    cb::strvector mComponents;

  public:
    CEntityFactory(cb::string const& typeId, cb::strvector const& components);
    ~CEntityFactory();

    cb::string GetTypeId() const { return mTypeId; }

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
    std::shared_ptr<CEntityFactory> Get(cb::string const& typeId) const;
  };
}