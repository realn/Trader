#include "stdafx.h"
#include "GFXCanvasVertex.h"
#include "GFXConsts.h"

namespace gfx {
  auto constexpr VIDX_CANVAS_POSITION = 0u;
  auto constexpr VIDX_CANVAS_TEXCOORD = 1u;
  auto constexpr VIDX_CANVAS_COLOR = 2u;

  auto const VIN_CANVAS_POSITION = L"vInPosition"s;
  auto const VIN_CANVAS_TEXCOORD = L"vInTexCoord"s;
  auto const VIN_CANVAS_COLOR = L"vInColor";

  cb::gl::CVertexDefinition CCanvasVertex::Def{
    { VIDX_CANVAS_POSITION, cb::gl::DataType::FLOAT, 2, sizeof(CCanvasVertex), 0},
    { VIDX_CANVAS_TEXCOORD, cb::gl::DataType::FLOAT, 2, sizeof(CCanvasVertex), sizeof(glm::vec2)},
    { VIDX_CANVAS_COLOR, cb::gl::DataType::FLOAT, 4, sizeof(CCanvasVertex), sizeof(glm::vec2) + sizeof(glm::vec2)}
  };

  CCanvasVertex::InputsT CCanvasVertex::Inputs = {
    { VIDX_CANVAS_POSITION, VIN_CANVAS_POSITION },
    { VIDX_CANVAS_TEXCOORD, VIN_CANVAS_TEXCOORD },
    { VIDX_CANVAS_COLOR, VIN_CANVAS_COLOR },
  };
}
