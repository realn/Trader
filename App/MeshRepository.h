#pragma once

#include <memory>
#include <map>

#include <CBStr\Convert.h>

#include <GFXFwd.h>

namespace trader {
  enum class MeshType {
    Unknown = 0,
    Planet,
    Satelite,
    Ship
  };

  extern gfx::CMesh CreateMesh(MeshType const type);

  class CMeshRepository {
  private:
    using MeshesT = std::map<MeshType, std::shared_ptr<gfx::CMeshView>>;

    MeshesT mMeshes;

  public:
    CMeshRepository();
    ~CMeshRepository();

    std::shared_ptr<gfx::CMeshView> Get(MeshType const type);
  };
}

namespace cb {
  template<>
  extern string toStr<trader::MeshType>(trader::MeshType const& value);

  template<>
  extern bool fromStr<trader::MeshType>(string const& value, trader::MeshType& outValue);
}
