#pragma once

#include <map>

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <CBGL/VertexDefinition.h>

namespace gfx {
  class CMeshVertex {
  public:
    glm::vec3 Pos;
    glm::vec3 Normal = {1.0f, 0.0f, 0.0f};
    glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};

    CMeshVertex() = default;
    CMeshVertex(glm::vec3 const& pos, glm::vec3 const& normal, glm::vec4 const& color)
      : Pos(pos), Normal(normal), Color(color) {}
    CMeshVertex(float const x, float const y, float const z,
            float const nx, float const ny, float const nz,
            float const r, float const g, float const b, float const a)
      : Pos(x, y, z), Normal(nx, ny, nz), Color(r, g, b, a) {}

    bool operator==(CMeshVertex const& other) { return Pos == other.Pos && Normal == other.Normal && Color == other.Color; }
    bool operator!=(CMeshVertex const& other) { return !(*this == other); }

    CMeshVertex operator*(glm::mat4 const& value) const;
    CMeshVertex operator*=(glm::mat4 const& value) { *this = *this * value; return *this; }

    static cb::gl::CVertexDefinition Def;
    static std::map<cb::u32, cb::string> Inputs;
  };
}