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
    struct RenderData {
      glm::mat4 mTransform = glm::mat4(1.0f);
      glm::vec4 mColor = glm::vec4(1.0f);
    };
    using TransformsT = std::vector<RenderData>;
    using MeshTypeTransformsT = std::map<std::shared_ptr<gfx::CMeshView>, TransformsT>;
    using EntitiesMapT = std::map<std::shared_ptr<eco::CEntity>, CEntityView>;

  private:
    EntitiesMapT mEntities;
    gfx::CMeshView mJunctionView;
    size_t mFrame = 0;

  public:
    CUniverseView();

    void UpdateRender(std::shared_ptr<eco::CUniverse> universe, CMeshRepository& meshRepo);
    void Render(glm::mat4 const& transform, cb::gl::CProgram& meshProgram);

    CEntityView CreateView(eco::CEntity& entity, CMeshRepository& meshRepo) const;
    void UpdateView(CEntityView& view, eco::CEntity const& entity) const;
  };
}
