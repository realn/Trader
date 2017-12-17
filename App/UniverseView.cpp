#include "stdafx.h"

#include <CBGL\Program.h>

#include <GFXConsts.h>
#include <GFXMeshView.h>

#include <ECOEntity.h>
#include <ECOUniverse.h>

#include "UniverseView.h"

namespace trader {
  namespace {
    glm::vec3 to3DSpace(glm::vec2 const& value) {
      return glm::vec3(value.x, 0.0f, value.y);
    }
  }

  CUniverseView::CUniverseView(std::shared_ptr<eco::CUniverse> universe) 
    : mUniverse(universe)
  {}

  void CUniverseView::Render(glm::mat4 const& transform, cb::gl::CProgram& meshProgram, CMeshRepository& meshRepo) {
    using namespace glm;

    auto meshtrans = MeshTypeTransformsT();
    for(auto& entity : mUniverse->GetEntities()) {
      auto type = MeshType::Unknown;
      if(!cb::fromStr(entity->GetTypeId(), type)) {
        continue;
      }

      auto trans =
        translate(mat4(1.0f), to3DSpace(entity->GetPosition()));

      meshtrans[type].push_back(trans);
    }

    auto gprog = cb::gl::bind(meshProgram);
    for(auto& meshtype : meshtrans) {
      auto mesh = meshRepo.Get(meshtype.first);
      auto gmesh = cb::gl::bind(*mesh);

      for(auto& trans : meshtype.second) {
        meshProgram.SetUniform(gfx::UNI_TRANSFORM, transform * trans);
        mesh->Render();
      }
    }
  }
}
