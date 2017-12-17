#pragma once

#include <memory>
#include <vector>
#include <map>

#include <ECOEntity.h>
#include <GFXMeshView.h>

#include <CBGL\Fwd.h>

#include "MeshRepository.h"

namespace eco {
  class CUniverse;
}

namespace trader {
  class CUniverseView {
  public:
    using TransformsT = std::vector<glm::mat4>;
    using MeshTypeTransformsT = std::map<MeshType, TransformsT>;
    using MeshesT = std::map<MeshType, std::shared_ptr<gfx::CMeshView>>;

  private:
    std::shared_ptr<eco::CUniverse> mUniverse;
    MeshesT mMeshes;

  public:
    CUniverseView(std::shared_ptr<eco::CUniverse> universe);

    void Render(glm::mat4 const& transform, cb::gl::CProgram& meshProgram, CMeshRepository& meshRepo);
  };
}