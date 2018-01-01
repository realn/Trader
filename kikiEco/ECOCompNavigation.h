#pragma once

#include "ECOComponent.h"

namespace eco {
  namespace comp {
    class CNavigation 
      : public CComponent
    {
    private:
      std::weak_ptr<CEntity> mCurrentDock;

    public:
      CNavigation(std::shared_ptr<CEntity> parent);
      virtual ~CNavigation();

      void SetCurrentDock(std::shared_ptr<CEntity> dock);
      void ClearCurrentDock();

      void Update(float const timeDelta) override;
    };
  }

  template<>
  extern cb::string const& GetComponentId<comp::CNavigation>();
}