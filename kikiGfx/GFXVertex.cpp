#include "stdafx.h"
#include "GFXVertex.h"
#include "GFXConsts.h"

namespace gfx {
  cb::gl::CVertexDefinition CVertex::Def{
    {IDX_VERTEX4, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), 0},
    {IDX_COLOR4, cb::gl::DataType::FLOAT, 4, sizeof(CVertex), sizeof(glm::vec4)},
  };
}