#pragma once

#include <array>

#include "GFXMeshVertex.h"

namespace gfx {
  class CMesh {
  public:
    class CFace {
    public:
      std::array<cb::u16, 3> Indices;

      CFace() = default;
      CFace(cb::u16 const v1, cb::u16 const v2, cb::u16 const v3) : Indices({v1, v2, v3}) {}

      bool operator==(CFace const& other) { return std::equal(Indices.begin(), Indices.end(), other.Indices.begin(), other.Indices.end()); }
      bool operator!=(CFace const& other) { return !(*this == other); }
    };

    using VertexVecT = std::vector<CMeshVertex>;
    using FaceVecT = std::vector<CFace>;

  private:
    VertexVecT mVertices;
    FaceVecT mFaces;

  public:
    CMesh() = default;
    CMesh(CMesh&&) = default;
    CMesh(CMesh const&) = default;

    FaceVecT::iterator AddFace(CMeshVertex const& v1, CMeshVertex const& v2, CMeshVertex const& v3);
    FaceVecT::iterator AddFace(std::array<CMeshVertex, 3> const& verts);
    VertexVecT::iterator AddVertex(CMeshVertex const& vertex);

    FaceVecT const& GetFaces() const { return mFaces; }
    VertexVecT const& GetVertices() const { return mVertices; }

    CMesh operator*(glm::mat4 const& value) const;

    static std::array<glm::vec2, 4> CreateRectVertices(glm::vec2 const& size,
                                                       glm::vec2 const& offset = glm::vec2());

    static CMesh CreatePlane(glm::vec2 const& size,
                             glm::uvec2 const& slices = {1,1},
                             glm::vec4 const& color = glm::vec4(1.0f));
    static CMesh CreateCube(glm::vec3 const& size, std::array<glm::vec4, 6> const& sideColors);
  };

}
