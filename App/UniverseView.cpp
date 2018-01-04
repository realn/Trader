#include "stdafx.h"

#include <CBGL\Program.h>

#include <GFXConsts.h>
#include <GFXMesh.h>
#include <GFXMeshView.h>

#include <ECOEntity.h>
#include <ECOUniverse.h>
#include <ECOTradeRoute.h>

#include "MeshRepository.h"
#include "UniverseView.h"

namespace trader {
  namespace {
    glm::vec3 to3DSpace(glm::vec2 const& value) {
      return glm::vec3(value.x, 0.0f, value.y);
    }
  }

  CUniverseView::CUniverseView() {}

  void CUniverseView::UpdateRender(std::shared_ptr<eco::CUniverse> universe, CMeshRepository& meshRepo) {
    mFrame++;

    auto type = MeshType::Unknown;
    for(auto& entity : universe->GetEntities()) {
      auto it = mEntities.find(entity);

      if(it == mEntities.end()) {
        if(!cb::fromStr(entity->GetTypeId(), type)) {
          continue;
        }
        auto mesh = meshRepo.Get(type);
        auto view = CEntityView(mesh);
        view.SetPosition(to3DSpace(entity->GetPosition()));
        view.SetRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
        view.SetFrame(mFrame);
        mEntities[entity] = view;
      }
      else {
        auto& view = it->second;

        auto prevPos = view.GetPosition();
        view.SetPosition(to3DSpace(entity->GetPosition()));
        if(view.GetPosition() != prevPos) {
          auto vec = glm::normalize(view.GetPosition() - prevPos);
          auto ang = glm::atan(vec.z, vec.x);
          //auto ang = glm::acos(glm::dot(vec, glm::vec3(0.0f, 0.0f, -1.0f)));
          view.SetRotation(glm::angleAxis(ang, glm::vec3(0.0f, -1.0f, 0.0f))
                           * glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        }
        view.SetFrame(mFrame);
      }
    }

    if(mEntities.size() != universe->GetEntities().size()) {
      auto it = mEntities.begin();
      while(it != mEntities.end()) {
        if(it->second.GetFrame() != mFrame) {
          it = mEntities.erase(it);
        }
        else it++;
      }
    }

    {
      auto mesh = gfx::CMesh(cb::gl::PrimitiveType::LINES);
      auto color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      for(auto& junction : universe->GetJunctions()) {
        auto poslist = std::vector<glm::vec3>();
        for(auto& target : junction->GetTargets()) {
          poslist.push_back(to3DSpace(target->GetPosition()));
        }

        mesh += gfx::CMesh::CreateLine(poslist[0], poslist[1], color);
      }
      mJunctionView = std::make_shared<gfx::CMeshView>(mesh);
    }
  }

  void CUniverseView::Render(glm::mat4 const& transform, cb::gl::CProgram& meshProgram) {
    using namespace glm;

    auto meshtrans = MeshTypeTransformsT();
    for(auto& entity : mEntities) {
      auto mesh = entity.second.GetMesh();
      auto trans = entity.second.GetTransform();

      meshtrans[mesh].push_back(trans);
    }

    auto gprog = cb::gl::bind(meshProgram);
    for(auto& meshtype : meshtrans) {
      auto mesh = meshtype.first;
      auto gmesh = cb::gl::bind(*mesh);

      for(auto& trans : meshtype.second) {
        meshProgram.SetUniform(gfx::UNI_TRANSFORM, transform * trans);
        mesh->Render();
      }
    }

    if(mJunctionView) {
      auto gmesh = cb::gl::bind(*mJunctionView);
      meshProgram.SetUniform(gfx::UNI_TRANSFORM, transform);
      mJunctionView->Render();
    }
  }
}
