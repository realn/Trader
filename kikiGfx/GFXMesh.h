#pragma once

#include <array>

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <CBGL/VertexDefinition.h>

namespace gfx {
  class CMesh {
  public:
    class CVertex {
    public:
      glm::vec3 Pos;
      glm::vec3 Normal = {1.0f, 0.0f, 0.0f};
      glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};

      CVertex() = default;
      CVertex(glm::vec3 const& pos, glm::vec3 const& normal, glm::vec4 const& color)
        : Pos(pos), Normal(normal), Color(color) {}
      CVertex(float const x, float const y, float const z,
              float const nx, float const ny, float const nz,
              float const r, float const g, float const b, float const a)
        : Pos(x, y, z), Normal(nx, ny, nz), Color(r, g, b, a) {}

      bool operator==(CVertex const& other) { return Pos == other.Pos && Normal == other.Normal && Color == other.Color; }
      bool operator!=(CVertex const& other) { return !(*this == other); }

      CVertex operator*(glm::mat4 const& value) const;
      CVertex operator*=(glm::mat4 const& value) { *this = *this * value; return *this; }

      static cb::gl::CVertexDefinition Def;
    };

    class CFace {
    public:
      std::array<cb::u16, 3> Indices;

      CFace() = default;
      CFace(cb::u16 const v1, cb::u16 const v2, cb::u16 const v3) : Indices({v1, v2, v3}) {}
    };

    using VertexVecT = std::vector<CVertex>;
    using FaceVecT = std::vector<CFace>;

  private:
    VertexVecT mVertices;
    FaceVecT mFaces;

  public:
    CMesh() = default;
    CMesh(CMesh&&) = default;
    CMesh(CMesh const&) = default;

    FaceVecT::iterator AddFace(CVertex const& v1, CVertex const& v2, CVertex const& v3);
    FaceVecT::iterator AddFace(std::array<CVertex, 3> const& verts);
    VertexVecT::iterator AddVertex(CVertex const& vertex);

    FaceVecT const& GetFaces() const { return mFaces; }
    VertexVecT const& GetVertices() const { return mVertices; }

    CMesh operator*(glm::mat4 const& value) const;

    static std::array<glm::vec2, 4> CreateRectVertices(glm::vec2 const& size,
                                                       glm::vec2 const& offset = glm::vec2());

    static CMesh CreatePlane(glm::vec2 const& size,
                             glm::uvec2 const& slices = glm::uvec2(1),
                             glm::vec4 const& color = glm::vec4(1.0f));
    static CMesh CreateCube(glm::vec3 const& size,
                            std::array<glm::vec4, 6> const& sideColors);
  };

}
