#include "stdafx.h"

#include <GFXMesh.h>
#include <GFXMeshView.h>

#include "MeshRepository.h"

namespace trader {
  gfx::CMesh CreateMesh(MeshType const type) {
    using namespace glm;

    switch(type) {
    case  MeshType::Planet:
      return
        rotate(mat4(1.0f), radians(-90.0f), { 1.0f, 0.0f, 0.0f }) *
        //gfx::CMesh::CreateTube({0.2f, 0.2f}, {0.2f,0.2f}, 0.4f, two_pi<float>(), {12, 1}, false, {0.0f, 1.0f, 0.0f, 1.0f});
        gfx::CMesh::CreateSphere(vec3(0.5f), { 16, 8 }, { 0.0f, 1.0f, 0.0f, 1.0f }, false);

    case MeshType::Ship:
      return
        gfx::CMesh::CreateTube({ 0.0f, 0.0f }, { 0.2f, 0.2f }, 0.3f, two_pi<float>(), { 8, 1 }, false, { 1.0f, 0.0f, 0.0f, 1.0f });

    case  MeshType::Satelite:
      return
        rotate(mat4(1.0f), radians(-90.0f), { 1.0f, 0.0f, 0.0f }) *
        //gfx::CMesh::CreateTube({0.2f, 0.2f}, {0.2f,0.2f}, 0.4f, two_pi<float>(), {12, 1}, false, {0.0f, 1.0f, 0.0f, 1.0f});
        gfx::CMesh::CreateSphere(vec3(0.5f), { 16, 8 }, { 0.0f, 0.5f, 1.0f, 1.0f }, false);

    default:
      break;
    }
    return gfx::CMesh();
  }


  CMeshRepository::CMeshRepository() {}

  CMeshRepository::~CMeshRepository() {}

  std::shared_ptr<gfx::CMesh> CMeshRepository::Get(MeshType const type) {
    auto it = mMeshes.find(type);
    if(it != mMeshes.end()) {
      return it->second;
    }

    auto mesh = std::make_shared<gfx::CMesh>(std::move(CreateMesh(type)));
    mMeshes[type] = mesh;

    return mesh;
  }
}

namespace cb {
  const auto gMeshTypeStrings = std::map<trader::MeshType, string>{
    {trader::MeshType::Unknown, L"Unknown"s},
    {trader::MeshType::Planet, L"Planet"s},
    {trader::MeshType::Satelite, L"Satelite"s},
    {trader::MeshType::Ship, L"Ship"s}
  };

  template<>
  string toStr(trader::MeshType const & value) {
    auto it = gMeshTypeStrings.find(value);
    if(it != gMeshTypeStrings.end())
      return it->second;
    return string();
  }

  template<>
  bool fromStr(string const & value, trader::MeshType & outValue) {
    auto it = std::find_if(gMeshTypeStrings.begin(), gMeshTypeStrings.end(),
                           [value](const std::pair<trader::MeshType, string>& item) -> bool {
      return item.second == value;
    });
    if(it == gMeshTypeStrings.end()) {
      return false;
    }

    outValue = it->first;
    return true;
  }
}
