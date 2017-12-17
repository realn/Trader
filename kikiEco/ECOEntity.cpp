#include "stdafx.h"
#include "ECOEntity.h"

namespace eco {
  CEntity::CEntity(cb::string const & typeId, cb::string const & id) : 
    mTypeId(typeId), mId(id) {}

}
