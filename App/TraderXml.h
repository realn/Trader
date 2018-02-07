#pragma once

#include <vector>
#include <glm/vec2.hpp>

#include <CBStr/Defines.h>

namespace trader {
  namespace data {
    struct CProduct {
      cb::string mId;
      float mValue = 0.0f;
    };
    using ProductsT = std::vector<CProduct>;

    struct CFactoryTemplate {
      cb::string mId;
      cb::string mName;
      ProductsT mInProducts;
      ProductsT mOutProducts;
    };
    using FactoryTemplatesT = std::vector<CFactoryTemplate>;

    struct CEntity {
      cb::string mName;
      glm::vec2 mPosition; // optional
      cb::string mDock; // optional; target must have dock component
    };
    using EntitiesT = std::vector<CEntity>;

    struct CEntityList{
      cb::string mTypeId;
      EntitiesT mEntities;
    };
    using EntityTypesT = std::vector<CEntityList>;

    struct CUniverse {
      float mMaxJunctionDistance = 7.0f;
      FactoryTemplatesT mFactoryTemplates;
      EntityTypesT mTypes;
    };

    bool Load(cb::string const& filepath, CUniverse& outUniverse);
  }
}