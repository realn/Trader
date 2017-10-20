#include "stdafx.h"
#include "GFXMeshVertex.h"
#include "GFXConsts.h"

namespace gfx {
  cb::gl::CVertexDefinition CMeshVertex::Def = {
    {IDX_VERTEX3_POS, cb::gl::DataType::FLOAT, 3, sizeof(CMeshVertex), 0},
    {IDX_VERTEX3_NORMAL, cb::gl::DataType::FLOAT, 3, sizeof(CMeshVertex), sizeof(glm::vec3)},
    {IDX_VERTEX3_COLOR, cb::gl::DataType::FLOAT, 4, sizeof(CMeshVertex), sizeof(glm::vec3) * 2},
  };

  std::map<cb::u32, cb::string> CMeshVertex::Inputs = {
    {IDX_VERTEX3_POS, VIN_VERTEX3_POS},
    {IDX_VERTEX3_NORMAL, VIN_VERTEX3_NORMAL},
    {IDX_VERTEX3_COLOR, VIN_VERTEX3_COLOR}
  };
}

gfx::CMeshVertex operator*(glm::mat4 const & value, gfx::CMeshVertex const & vertex) {
  return gfx::CMeshVertex{
    glm::vec3(value * glm::vec4(vertex.Pos, 1.0f)),
    glm::mat3(value) * vertex.Normal,
    vertex.Color
  };
}
