#pragma once

#include <memory>
#include <vector>
#include <map>

#include <CBGL\Fwd.h>

#include <GFXMeshView.h>

#include "EntityView.h"

namespace eco {
  class CEntity;
  class CUniverse;
}

namespace trader {
  class CMeshRepository;

  class CUniverseView {
  public:
    using TransformsT = std::vector<glm::mat4>;
    using MeshTypeTransformsT = std::map<std::shared_ptr<gfx::CMeshView>, TransformsT>;
    using EntitiesT = std::map<std::shared_ptr<eco::CEntity>, CEntityView>;

  private:
    EntitiesT mEntities;
    std::shared_ptr<gfx::CMeshView> mJunctionView;
    size_t mFrame = 0;

  public:
    CUniverseView();

    void UpdateRender(std::shared_ptr<eco::CUniverse> universe, CMeshRepository& meshRepo);
    void Render(glm::mat4 const& transform, cb::gl::CProgram& meshProgram);
  };
}