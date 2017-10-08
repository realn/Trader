#pragma once

#include <CBStr/Defines.h>

namespace gfx {
  using namespace std::literals;

  static const auto UNI_TRANSFORM = L"mTransform"s;
  static const auto UNI_BASE_TEXTURE = L"texBase"s;
  static const auto UNI_FONT_TEXTURE = L"texFont"s;
  static const auto VIN_VERTEX4 = L"vInVertex"s;
  static const auto VIN_COLOR4 = L"vInColor"s;

  constexpr auto IDX_VERTEX4 = 0u;
  constexpr auto IDX_COLOR4 = 1u;
}
