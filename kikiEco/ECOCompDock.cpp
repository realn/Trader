#include "stdafx.h"

#include <CBXml/Serialize.h>

#include "ECOEntity.h"
#include "ECOTradeRoute.h"
#include "ECOCompNavigation.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompDock.h"

namespace eco {
  static auto COMP_DOCK_ID = L"Dock"_id;

  namespace xml {
    template<>
    void RegisterComponent<comp::CDock>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_DOCK_ID.GetName());
    }
  }

  namespace comp {
    CDock::CDock(std::shared_ptr<CEntity> parent) 
      : CComponent(parent, COMP_DOCK_ID)
    {}

    CDock::CDock(std::shared_ptr<CEntity> parent, xml::CComponent const& component)
      : CComponent(parent, COMP_DOCK_ID) {}

    CDock::~CDock() {}

    void CDock::AddJunction(std::shared_ptr<CTradeJunction> junction) {
      mJunctions.insert(junction);
    }

    std::shared_ptr<CTradeJunction> CDock::FindJunction(std::shared_ptr<CEntity> targetDock) const {
      auto it = std::find_if(mJunctions.begin(), mJunctions.end(), [targetDock](const JunctionsT::key_type& item) { return item.lock()->Contains(targetDock); });
      if(it == mJunctions.end()) {
        return std::shared_ptr<CTradeJunction>();
      }
      return it->lock();
    }

    void CDock::DockShip(std::shared_ptr<CEntity> entity) {
      mDockedShips.insert(entity);
      entity->SetPosition(GetParent()->GetPosition());
      if(entity->HasComponent<comp::CNavigation>()) {
        entity->GetComponent<comp::CNavigation>().SetCurrentDock(GetParent());
      }
    }
    void CDock::UnDockShip(std::shared_ptr<CEntity> entity) {
      mDockedShips.erase(entity);
      if(entity->HasComponent<comp::CNavigation>()) {
        entity->GetComponent<comp::CNavigation>().ClearCurrentDock();
      }
    }

    CDock::EntitiesT CDock::GetConnectedDocks() const {
      auto result = EntitiesT();
      for(auto& item : mJunctions) {
        if(item.expired())
          continue;
        
        auto junction = item.lock();
        result.insert(junction->OtherTarget(GetParent()));
      }
      return result;
    }

    void CDock::PrintInfo(cb::ostream & stream) const {
      stream << L" Dock: "s << mDockedShips.size() << L" ships docked." << std::endl;
    }
  }

  template<>
  core::CFastId const& GetComponentId<comp::CDock>() {
    return COMP_DOCK_ID;
  }
}
