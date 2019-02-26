#pragma once

#include <CBCore/Defines.h>

namespace gfx {
  using namespace std::literals;

  static const auto UNI_TRANSFORM = L"mTransform"s;
  static const auto UNI_MESHCOLOR = L"vMeshColor"s;

  static const auto VIN_VERTEX3_POS = L"vInVertex"s;
  static const auto VIN_VERTEX3_NORMAL = L"vInNormal"s;
  static const auto VIN_VERTEX3_COLOR = L"vInColor"s;

  constexpr auto IDX_VERTEX3_POS = 0u;
  constexpr auto IDX_VERTEX3_NORMAL = 1u;
  constexpr auto IDX_VERTEX3_COLOR = 2u;
}
