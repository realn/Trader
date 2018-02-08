#pragma once

#include "ECOComponent.h"

namespace eco {
  namespace xml {
    struct CComponent;
  }

  namespace comp {
    class CGovernor 
      : public CComponent
    {
    public:
      CGovernor(std::shared_ptr<CEntity> parent);
      CGovernor(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      virtual ~CGovernor();

      virtual void Update(const float timeDelta);

      virtual void PrintInfo(cb::ostream& stream) const;
    };
  }

  template<>
  cb::string const& GetComponentId<comp::CGovernor>();

  namespace xml {
    template<> void RegisterComponent<comp::CGovernor>();
  }
}