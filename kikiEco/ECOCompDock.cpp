#include "stdafx.h"
#include "ECOEntity.h"
#include "ECOCompDock.h"

namespace eco {
  static auto COMP_DOCK_ID = L"Dock"s;

  namespace comp {
    CDock::CDock(std::shared_ptr<CEntity> parent) 
      : CComponent(parent, COMP_DOCK_ID)
    {}

    CDock::~CDock() {}

    void CDock::DockShip(std::shared_ptr<CEntity> entity) {
      mDockedShips.insert(entity);
      entity->SetPosition(GetParent()->GetPosition());
    }
    void CDock::UnDockShip(std::shared_ptr<CEntity> entity) {
      mDockedShips.erase(entity);
    }
  }

  template<>
  cb::string const& GetComponentId<comp::CDock>() {
    return COMP_DOCK_ID;
  }
}
