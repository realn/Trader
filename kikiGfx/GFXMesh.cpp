#include "stdafx.h"
#include "GFXMesh.h"
#include "GFXConsts.h"

#include <glm/gtc/matrix_transform.hpp>

namespace gfx {
  CMesh::CVertex CMesh::CVertex::operator*(glm::mat4 const & value) const {
    return CVertex{
      glm::vec3(glm::vec4(Pos, 1.0f) * value),
      Normal * glm::mat3(value),
      Color
    };
  }

  cb::gl::CVertexDefinition CMesh::CVertex::Def = {
      {IDX_VERTEX3_POS, cb::gl::DataType::FLOAT, 3, sizeof(CVertex), 0},
      {IDX_VERTEX3_NORMAL, cb::gl::DataType::FLOAT, 3, sizeof(CVertex), sizeof(glm::vec3)},
      {IDX_VERTEX3_COLOR, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), sizeof(glm::vec3) * 2},
  };

  CMesh::FaceVecT::iterator CMesh::AddFace(CMesh::CVertex const & v1,
                                           CMesh::CVertex const & v2,
                                           CMesh::CVertex const & v3) {
    auto iv1 = AddVertex(v1) - mVertices.begin();
    auto iv2 = AddVertex(v2) - mVertices.begin();
    auto iv3 = AddVertex(v3) - mVertices.begin();
    auto face = CFace{iv1, iv2, iv3};

    auto it = std::find(mFaces.begin(), mFaces.end(), face);
    if(it != mFaces.end()) {
      return it;
    }
    return mFaces.emplace(mFaces.end(), face);
  }

  CMesh::FaceVecT::iterator CMesh::AddFace(std::array<CVertex, 3> const & verts) {
    return AddFace(verts[0], verts[1], verts[2]);
  }

  CMesh::VertexVecT::iterator CMesh::AddVertex(CMesh::CVertex const & vertex) {
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
