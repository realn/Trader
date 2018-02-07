#pragma once

#include <vector>
#include <glm/vec2.hpp>

#include <CBStr/Defines.h>

namespace trader {
  namespace data {
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
      EntityTypesT mTypes;
    };

    bool Load(cb::string const& filepath, CUniverse& outUniverse);
  }
}