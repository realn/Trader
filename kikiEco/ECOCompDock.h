#pragma once

#include <vector>
#include <set>

#include "ECOComponent.h"

namespace eco {
  class CTradeJunction;

  namespace comp {
    class CDock 
      : public CComponent
    {
    public:
      using EntitiesT = std::set<std::weak_ptr<CEntity>, weak_ptr_less<CEntity>>;
      using JunctionsT = std::set<std::weak_ptr<CTradeJunction>, weak_ptr_less<CTradeJunction>>;

    private:
      EntitiesT mDockedShips;
      JunctionsT mJunctions;
      
    public:
      CDock(std::shared_ptr<CEntity> parent);
      virtual ~CDock();

      void AddJunction(std::shared_ptr<CTradeJunction> junction);
      std::shared_ptr<CTradeJunction> FindJunction(std::shared_ptr<CEntity> targetDock) const;

      void DockShip(std::shared_ptr<CEntity> entity);
      void UnDockShip(std::shared_ptr<CEntity> entity);

      EntitiesT const& GetDockedShips() const { return mDockedShips; }
      EntitiesT GetConnectedDocks() const;
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CDock>();
}