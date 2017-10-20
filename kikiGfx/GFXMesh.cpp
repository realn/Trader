#include "stdafx.h"
#include "GFXMesh.h"
#include "GFXConsts.h"

#include <glm/gtc/matrix_transform.hpp>

namespace gfx {
  namespace {
    glm::mat4 makemat(float tx, float ty, float tz, float rdeg, float rx, float ry, float rz) {
      using namespace glm;
      return translate(mat4(1.0f), {tx, ty, tz}) * rotate(mat4(1.0f), radians(rdeg), {rx, ry, rz});
    }
  }

  CMesh::FaceVecT::iterator CMesh::AddFace(CMeshVertex const & v1,
                                           CMeshVertex const & v2,
                                           CMeshVertex const & v3) {
    return AddFace({v1, v2, v3});
  }

  CMesh::FaceVecT::iterator CMesh::AddFace(CMeshVertex const & v1, CMeshVertex const & v2) {
    return AddFace({v1, v2});
  }

  CMesh::FaceVecT::iterator CMesh::AddFace(CMesh::VertexVecT const & verts) {
    auto face = CFace();
    for(auto& vert : verts) {
      auto iv = static_cast<cb::u16>(std::distance(mVertices.begin(), AddVertex(vert)));
      face.Indices.push_back(iv);
    }
    auto it = std::find(mFaces.begin(), mFaces.end(), face);
    if(it != mFaces.end()) {
      return it;
    }
    return mFaces.emplace(mFaces.end(), face);
  }

  CMesh::VertexVecT::iterator CMesh::AddVertex(CMeshVertex const & vertex) {
    auto it = std::find(mVertices.begin(), mVertices.end(), vertex);
    if(it != mVertices.end()) {
      return it;
    }
    return mVertices.emplace(mVertices.end(), vertex);
  }

  void CMesh::Transform(glm::mat4 const & value) {
    for(auto& vert : mVertices) {
      vert = value * vert;
    }
  }

  CMesh CMesh::operator+(CMesh const & mesh) const {
    auto result = CMesh(*this);
    for(auto& face : mesh.mFaces) {
      auto verts = VertexVecT();
      for(auto& index : face.Indices) {
        verts.push_back(mesh.mVertices[index]);
      }
      result.AddFace(verts);
    }
    return result;
  }

  std::array<glm::vec3, 4> CMesh::CreateRectVertices(glm::vec2 const & size, glm::vec2 const & offset) {
    using namespace glm;
    auto far = offset + size;
    return {
      vec3(mix(offset, far, vec2{0, 0}), 0.0f),
      vec3(mix(offset, far, vec2{1, 0}), 0.0f),
      vec3(mix(offset, far, vec2{1, 1}), 0.0f),
      vec3(mix(offset, far, vec2{0, 1}), 0.0f),
    };
  }

  std::vector<glm::vec3> CMesh::CreateCircleVertices(glm::vec2 const & size, cb::u32 const slices,
                                                     glm::vec3 const & offset, float const radAngle) {
    using namespace glm;
    auto result = std::vector<vec3>();
    auto step = radAngle / slices;
    auto radius = size / 2.0f;
    for(auto i = 0u; i < slices; i++) {
      result.push_back(vec3(vec2{cos(step * i), sin(step * i)} * radius, 0.0f) + offset);
    }
    return result;
  }

  CMesh CMesh::CreatePlane(glm::vec2 const & size, glm::vec4 const & color, glm::uvec2 const & slices,
                           bool const wireFrame) {
    using namespace glm;
    auto mesh = CMesh();
    if(wireFrame) {
      mesh.SetPrimitiveType(cb::gl::PrimitiveType::LINES);
    }
    auto half = size / 2.0f;
    auto step = size / vec2(slices);
    auto start = vec3(-half, 0.0f);
    auto n = vec3(0.0f, 0.0f, -1.0f);

    for(auto y = 0u; y < slices.y; y++) {
      for(auto x = 0u; x < slices.x; x++) {
        auto v = CreateRectVertices(step, glm::vec2(x, y) * step);
        if(wireFrame) {
          for(auto i = 0u; i < 4; i++) {
            mesh.AddFace({start + v[i], n, color}, {start + v[(i+1)%4], n, color});
          }
        }
        else {
          mesh.AddFace({start + v[0], n, color}, {start + v[1], n, color}, {start + v[2], n, color});
          mesh.AddFace({start + v[0], n, color}, {start + v[2], n, color}, {start + v[3], n, color});
        }
      }
    }

    return mesh;
  }

  CMesh CMesh::CreateCube(glm::vec3 const & size, std::array<glm::vec4, 6> const & sideColors,
                          glm::uvec3 const & slices, bool const wireFrame) {
    auto offset = -size / 2.0f;
    return
      makemat(0.0f, 0.0f, -offset.z, 0.0f, 0.0f, 1.0f, 0.0f) *
      CreatePlane({size.x, size.y}, sideColors[0], {slices.x, slices.y}, wireFrame) +
      makemat(0.0f, 0.0f, offset.z, 180.0f, 0.0f, 1.0f, 0.0f) *
      CreatePlane({size.z, size.y}, sideColors[1], {slices.x, slices.y}, wireFrame) +
      makemat(-offset.x, 0.0f, 0.0f, 90.0f, 0.0f, 1.0f, 0.0f) *
      CreatePlane({size.z, size.y}, sideColors[2], {slices.z, slices.y}, wireFrame) +
      makemat(offset.x, 0.0f, 0.0f, 270.0f, 0.0f, 1.0f, 0.0f) *
      CreatePlane({size.z, size.y}, sideColors[3], {slices.z, slices.y}, wireFrame) +
      makemat(0.0f, -offset.y, 0.0f, 90.0f, 1.0f, 0.0f, 0.0f) *
      CreatePlane({size.z, size.y}, sideColors[4], {slices.z, slices.y}, wireFrame) +
      makemat(0.0f, offset.y, 0.0f, 270.0f, 1.0f, 0.0f, 0.0f) *
      CreatePlane({size.z, size.y}, sideColors[5], {slices.z, slices.y}, wireFrame);
  }

  CMesh CMesh::CreateCube(glm::vec3 const & size, glm::vec4 const & color, glm::uvec3 const & slices,
                          bool const wireFrame) {
    auto colors = std::array<glm::vec4, 6>();
    colors.fill(color);
    return CreateCube(size, colors, slices, wireFrame);
  }

  CMesh CMesh::CreateCircle(glm::vec2 const & size, cb::u32 const slices, glm::vec4 const & color,
                            float const centerHeight, bool const wireFrame) {
    using namespace glm;
    auto result = CMesh();
    if(wireFrame) {
      result.SetPrimitiveType(cb::gl::PrimitiveType::LINES);
    }
    auto v = CreateCircleVertices(size, slices);
    auto n = vec3(0.0f, 0.0f, -1.0f);
    auto c = color;
    auto center = CMeshVertex({0.0f, 0.0f, centerHeight}, n, color);

    for(auto i = 0u; i < slices; i++) {
      if(wireFrame) {
        result.AddFace(center, {v[i], n, c});
        result.AddFace({v[i], n, c}, {v[(i + 1) % slices], n, c});
        result.AddFace({v[(i + 1) % slices], n, c}, center);
      }
      else {
        result.AddFace(center, {v[i], n, c}, {v[(i + 1) % slices], n, c});
      }
    }
    return result;
  }

  CMesh CMesh::CreateTube(glm::vec2 const & sizeBase, glm::vec2 const & sizeEnd, float const height, 
                          float const radSurfaceAngle, glm::uvec2 const & surfaceSlices,
                          bool const wireFrame, glm::vec4 const& color) {
    using namespace glm;
    auto result = CMesh();
    if(wireFrame)
      result.SetPrimitiveType(cb::gl::PrimitiveType::LINES);
    
    auto step = (sizeEnd - sizeBase) / static_cast<float>(surfaceSlices.y);
    auto hstep = height / surfaceSlices.y;
    auto n = vec3(0.0f, 0.0f, -1.0f);
    auto c = color;
    
    for(auto y = 0u; y < surfaceSlices.y; y++) {
      auto base = sizeBase + step * static_cast<float>(y);
      auto end = sizeBase + step * static_cast<float>(y + 1);

      auto vb = CreateCircleVertices(base, surfaceSlices.x, {0.0f, 0.0f, 0.0f}, radSurfaceAngle);
      auto ve = CreateCircleVertices(end, surfaceSlices.x, {0.0f, 0.0f, height}, radSurfaceAngle);

      for(auto x = 0u; x < surfaceSlices.x; x++) {
        auto nx = (x + 1) % surfaceSlices.x;
        if(wireFrame) {
          result.AddFace({vb[x], n, c}, {vb[nx], n, c});
          result.AddFace({vb[nx], n, c}, {ve[nx], n, c});
          result.AddFace({ve[nx], n, c}, {ve[x], n, c});
          result.AddFace({ve[x], n, c}, {vb[x], n, c});
        }
        else {
          result.AddFace({vb[x], n, c}, {vb[nx], n, c}, {ve[nx], n, c});
          result.AddFace({vb[x], n, c}, {ve[nx], n, c}, {ve[x], n, c});
        }
      }
    }

    return result;
  }
}

gfx::CMesh operator*(glm::mat4 const & value, gfx::CMesh const & mesh) {
  auto result = gfx::CMesh(mesh);
  result.Transform(value);
  return result;
}
