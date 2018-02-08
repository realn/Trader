#include "stdafx.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompGovernor.h"

namespace eco {
  static const auto COMP_GOVERNOR_ID = L"Governor"s;

  namespace comp {
    CGovernor::CGovernor(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_GOVERNOR_ID)
    {}

    CGovernor::CGovernor(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CComponent(parent, COMP_GOVERNOR_ID)
    {}

    CGovernor::~CGovernor() {}

    void CGovernor::Update(const float timeDelta) {

    }

    void CGovernor::PrintInfo(cb::ostream & stream) const {
      stream << L" GOVERNOR"s << std::endl;
    }
  }

  template<>
  cb::string const& GetComponentId<comp::CGovernor>() {
    return COMP_GOVERNOR_ID;
  }

  namespace xml {
    template<>
    void RegisterComponent<comp::CGovernor>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_GOVERNOR_ID);
    }
  }
}
