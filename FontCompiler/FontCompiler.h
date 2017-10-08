#pragma once

#include <CBStr/Defines.h>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>

struct CFontParams {
  cb::string FontPath;
  cb::string FontChars;
  cb::string OutputName;
  cb::string OutputPath;
  unsigned FontSize;
  unsigned TexSize;
  unsigned TexCharBorder;
  glm::vec4 CharColor;
};

namespace data {
  struct CFont;
}

extern bool CompileFont(CFontParams const& params);