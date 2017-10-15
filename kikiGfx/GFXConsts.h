#pragma once

#include <CBStr/Defines.h>

namespace gfx {
  using namespace std::literals;

  static const auto UNI_TRANSFORM = L"mTransform"s;
  static const auto UNI_BASE_TEXTURE = L"texBase"s;
  static const auto UNI_FONT_TEXTURE = L"texFont"s;

  static const auto VIN_VERTEX2_POS = L"vInVertex"s;
  static const auto VIN_VERTEX2_COLOR = L"vInColor"s;

  constexpr auto IDX_VERTEX2_POS = 0u;
  constexpr auto IDX_VERTEX2_COLOR = 1u;

  static const auto VIN_VERTEX3_POS = L"vInVertex"s;
  static const auto VIN_VERTEX3_NORMAL = L"vInNormal"s;
  static const auto VIN_VERTEX3_COLOR = L"vInColor"s;

  constexpr auto IDX_VERTEX3_POS = 0u;
  constexpr auto IDX_VERTEX3_NORMAL = 1u;
  constexpr auto IDX_VERTEX3_COLOR = 2u;
}
