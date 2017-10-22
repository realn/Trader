#pragma once

#include <array>
#include <glm/gtc/constants.hpp>
#include <CBGL/Buffer.h>
#include "GFXMeshVertex.h"

namespace gfx {
  class CMesh {
  public:
    using IndexVecT = std::vector<cb::u16>;

    class CFace {
    public:
      IndexVecT Indices;

      CFace() = default;
      CFace(cb::u16 const v1, cb::u16 const v2) : Indices({v1, v2}) {}
      CFace(cb::u16 const v1, cb::u16 const v2, cb::u16 const v3) : Indices({v1, v2, v3}) {}

      bool operator==(CFace const& other) const;
      bool operator!=(CFace const& other) const { return !(*this == other); }
    };

    using VertexVecT = std::vector<CMeshVertex>;
    using FaceVecT = std::vector<CFace>;

  private:
    VertexVecT mVertices;
    FaceVecT mFaces;
    cb::gl::PrimitiveType mPrimitiveType;

  public:
    CMesh(cb::gl::PrimitiveType type = cb::gl::PrimitiveType::TRIANGLES) : mPrimitiveType(type) {};
    CMesh(CMesh&&) = default;
    CMesh(CMesh const&) = default;

    FaceVecT::iterator AddFace(CMeshVertex const& v1, CMeshVertex const& v2, CMeshVertex const& v3);
    FaceVecT::iterator AddFace(CMeshVertex const& v1, CMeshVertex const& v2);
    FaceVecT::iterator AddFace(VertexVecT const& verts);
    VertexVecT::iterator AddVertex(CMeshVertex const& vertex);

    void SetPrimitiveType(cb::gl::PrimitiveType const type) { mPrimitiveType = type; }

    FaceVecT const& GetFaces() const { return mFaces; }
    VertexVecT const& GetVertices() const { return mVertices; }
    cb::gl::PrimitiveType GetPrimitiveType() const { return mPrimitiveType; }

    void Transform(glm::mat4 const& value);

    CMesh operator+(CMesh const& mesh) const;

    void operator=(CMesh const& mesh) { mVertices = mesh.mVertices; mFaces = mesh.mFaces; mPrimitiveType = mesh.mPrimitiveType; }
    void operator+=(CMesh const& mesh) { *this = *this + mesh; }

    static std::array<glm::vec3, 4> CreateRectVertices(glm::vec2 const& size,
                                                       glm::vec2 const& offset = glm::vec3());
    static std::vector<glm::vec3> CreateCircleVertices(glm::vec2 const& size, cb::u32 const slices,
                                                       glm::vec3 const& offset = glm::vec3(),
                                                       float const radAngle = glm::two_pi<float>());

    static CMesh CreateLine(glm::vec3 const& beg, glm::vec3 const& end, glm::vec4 const& color);
    static CMesh CreatePlane(glm::vec2 const& size, glm::vec4 const& color = glm::vec4(1.0f),
                             glm::uvec2 const& slices = {1,1}, bool const wireFrame = false);
    static CMesh CreateCube(glm::vec3 const& size, std::array<glm::vec4, 6> const& sideColors,
                            glm::uvec3 const& slices, bool const wireFrame);
    static CMesh CreateCube(glm::vec3 const& size, glm::vec4 const& color = glm::vec4(1.0f), 
                            glm::uvec3 const& slices = {1, 1, 1}, bool const wireFrame = false);
    static CMesh CreateCircle(glm::vec2 const& size, cb::u32 const slices, glm::vec4 const& color,
                              float const centerHeight = 0.0f, bool const wireFrame = false);
    static CMesh CreateTube(glm::vec2 const& sizeBase, glm::vec2 const& sizeEnd, float const height, 
                            float const radSurfaceAngle, glm::uvec2 const& surfaceSlices, 
                            bool const wireFrame, glm::vec4 const& color);
    static CMesh CreateSphere(glm::vec3 const& size, glm::uvec2 const& slices, glm::vec4 const& color,
                              bool const wireFrame = false, float const radYAngle = glm::pi<float>(),
                              float const radXAngle = glm::two_pi<float>());
  };
}

gfx::CMesh operator*(glm::mat4 const& value, gfx::CMesh const& mesh);
