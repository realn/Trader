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

  auto const MIN_MESH_TRANSFORM = L"mTransform"s;
  auto const VIN_MESH_COLOR = L"vMeshColor"s;

  CUniverseView::CUniverseView() {}

  void CUniverseView::UpdateRender(std::shared_ptr<eco::CUniverse> universe, CMeshRepository& meshRepo) {
    mFrame++;

    for(auto& entity : universe->GetEntities()) {
      auto it = mEntities.find(entity);

      if(it == mEntities.end()) {
        mEntities[entity] = CreateView(*entity, meshRepo);
      }
      else {
        UpdateView(it->second, *entity);
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
      mJunctionView.Update(mesh);
    }
  }

  void CUniverseView::Render(glm::mat4 const& transform, cb::gl::CProgram& meshProgram) {
    using namespace glm;

    auto meshtrans = MeshTypeTransformsT();
    for(auto& entity : mEntities) {
      auto mesh = entity.second.GetMesh();
      auto trans = entity.second.GetTransform();
      auto color = entity.second.GetColor();

      meshtrans[mesh].push_back({trans, color});
    }

    auto gprog = cb::gl::bind(meshProgram);
    for(auto& meshtype : meshtrans) {
      auto mesh = meshtype.first;
      auto gmesh = cb::gl::bind(*mesh);

      for(auto& renderData : meshtype.second) {
        meshProgram.SetUniform(VIN_MESH_COLOR, renderData.mColor);
        meshProgram.SetUniform(MIN_MESH_TRANSFORM, transform * renderData.mTransform);
        mesh->Render();
      }
    }

    {
      auto gmesh = cb::gl::bind(mJunctionView);
      meshProgram.SetUniform(VIN_MESH_COLOR, vec4(1.0f));
      meshProgram.SetUniform(MIN_MESH_TRANSFORM, transform);
      mJunctionView.Render();
    }
  }

  CEntityView CUniverseView::CreateView(eco::CEntity & entity, CMeshRepository& meshRepo) const {
    auto type = MeshType::Unknown;
    if(!cb::fromStr(entity.GetTypeId(), type)) {
      return CEntityView();
    }
    auto mesh = meshRepo.Get(type);
    auto view = CEntityView(mesh);
    view.SetPosition(to3DSpace(entity.GetPosition()));
    view.SetRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    view.SetFrame(mFrame);
    return view;
  }

  void CUniverseView::UpdateView(CEntityView & view, eco::CEntity const & entity) const {
    auto prevPos = view.GetPosition();
    view.SetPosition(to3DSpace(entity.GetPosition()));
    if(view.GetPosition() != prevPos) {
      auto vec = glm::normalize(view.GetPosition() - prevPos);
      auto ang = glm::atan(vec.z, vec.x);
      view.SetRotation(glm::angleAxis(ang, glm::vec3(0.0f, -1.0f, 0.0f))
                       * glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    view.SetFrame(mFrame);
  }
}
