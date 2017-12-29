#pragma once

#include <set>

#include "ECOComponent.h"

namespace eco {
  namespace comp {
    struct weak_ptr_ent_less {
      bool operator()(const std::weak_ptr<CEntity>& A, const std::weak_ptr<CEntity>& B) const {
        if(A.expired())
          return true;
        if(B.expired())
          return false;
        auto ptrA = A.lock();
        auto ptrB = B.lock();
        return ptrA.get() < ptrB.get();
      }
    };

    class CDock 
      : public CComponent
    {
    public:
      using EntitiesT = std::set<std::weak_ptr<CEntity>, weak_ptr_ent_less>;

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