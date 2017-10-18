#include "stdafx.h"
#include "GFXMesh.h"
#include "GFXConsts.h"

#include <glm/gtc/matrix_transform.hpp>

namespace gfx {
  CMesh::FaceVecT::iterator CMesh::AddFace(CMeshVertex const & v1, 
                                           CMeshVertex const & v2,
                                           CMeshVertex const & v3) {
    auto iv1 = static_cast<cb::u16>(AddVertex(v1) - mVertices.begin());
    auto iv2 = static_cast<cb::u16>(AddVertex(v2) - mVertices.begin());
    auto iv3 = static_cast<cb::u16>(AddVertex(v3) - mVertices.begin());
    auto face = CFace{iv1, iv2, iv3};

    auto it = std::find(mFaces.begin(), mFaces.end(), face);
    if(it != mFaces.end()) {
      return it;
    }
    return mFaces.emplace(mFaces.end(), face);
  }

  CMesh::FaceVecT::iterator CMesh::AddFace(std::array<CMeshVertex, 3> const & verts) {
    return AddFace(verts[0], verts[1], verts[2]);
  }

  CMesh::VertexVecT::iterator CMesh::AddVertex(CMeshVertex const & vertex) {
    auto it = std::find(mVertices.begin(), mVertices.end(), vertex);
    if(it != mVertices.end()) {
      return it;
    }
    return mVertices.emplace(mVertices.end(), vertex);
  }

  CMesh CMesh::operator*(glm::mat4 const & value) const {
    auto mesh = CMesh(*this);
    for(auto& vert : mesh.mVertices) {
      vert *= value;
    }
    return mesh;
  }

  std::array<glm::vec2, 4> CMesh::CreateRectVertices(glm::vec2 const & size, glm::vec2 const & offset) {
    auto far = offset + size;
    return {
      glm::mix(offset, far, glm::vec2{0, 0}),
      glm::mix(offset, far, glm::vec2{1, 0}),
      glm::mix(offset, far, glm::vec2{1, 1}),
      glm::mix(offset, far, glm::vec2{0, 1}),
    };
  }

  CMesh CMesh::CreatePlane(glm::vec2 const & size, glm::uvec2 const & slices, glm::vec4 const & color) {
    auto mesh = CMesh();
    auto half = size / 2.0f;
    auto step = size / glm::vec2(slices);
    auto start = -half;
    auto normal = glm::vec3(0.0f, 0.0f, -1.0f);

    for(auto y = 0u; y < slices.y; y++) {
      for(auto x = 0u; x < slices.x; x++) {
        auto v = CreateRectVertices(step, glm::vec2(x, y) * step);

        mesh.AddFace(
        {{v[0], 0.0f}, normal, color},
        {{v[1], 0.0f}, normal, color},
        {{v[2], 0.0f}, normal, color}
        );

        mesh.AddFace(
        {{v[0], 0.0f}, normal, color},
        {{v[2], 0.0f}, normal, color},
        {{v[3], 0.0f}, normal, color}
        );
      }
    }

    return mesh;
  }

  CMesh CMesh::CreateCube(glm::vec3 const & size,
                          std::array<glm::vec4, 6> const & sideColors) {

    return CMesh();
  }
}
