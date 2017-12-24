#pragma once

#include <set>

#include "ECOComponent.h"

namespace eco {
  namespace comp {
    class CDock 
      : public CComponent
    {
    public:
      using EntitiesT = std::set<std::weak_ptr<CEntity>>;

    private:
      EntitiesT mDockedShips;
      
    public:
      CDock(std::shared_ptr<CEntity> parent);
      virtual ~CDock();

      void DockShip(std::shared_ptr<CEntity> entity);
      void UnDockShip(std::shared_ptr<CEntity> entity);

      EntitiesT const& GetDockedShips() const { return mDockedShips; }
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CDock>();
}