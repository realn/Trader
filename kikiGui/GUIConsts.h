#pragma once

#include <CBStr/Defines.h>

namespace gui {
  ENUM_FLAG(Align) {
    None = 0,
      Left = 0x1,
      Right = 0x2,
      Top = 0x4,
      Bottom = 0x8,
      Center = (Left | Right),
      Middle = (Top | Bottom),
      MidCenter = (Center | Middle),
      Default = (Top | Left),
  };

  enum class Orientation {
    Horizontal = 0,
    Vertical = 1,
  };

  constexpr auto TEXT_SCALE = 1.0f;
}
