#include "stdafx.h"
#include "GFXCanvasVertex.h"
#include "GFXConsts.h"

namespace gfx {
  cb::gl::CVertexDefinition CCanvasVertex::Def{
    {IDX_VERTEX2_POS, cb::gl::DataType::FLOAT, 4, sizeof(CCanvasVertex), 0},
    {IDX_VERTEX2_COLOR, cb::gl::DataType::FLOAT, 4, sizeof(CCanvasVertex), sizeof(glm::vec4)},
  };

  std::map<cb::u32, cb::string> CCanvasVertex::Inputs = {
    {IDX_VERTEX2_POS, VIN_VERTEX2_POS},
    {IDX_VERTEX2_COLOR, VIN_VERTEX2_COLOR}
  };
}
