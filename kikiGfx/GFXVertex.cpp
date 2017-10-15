#include "stdafx.h"
#include "GFXVertex.h"
#include "GFXConsts.h"

namespace gfx {
  cb::gl::CVertexDefinition CVertex::Def{
    {IDX_VERTEX2_POS, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), 0},
    {IDX_VERTEX2_COLOR, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), sizeof(glm::vec4)},
  };
}