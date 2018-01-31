#pragma once

#include <CBStr/Defines.h>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>

struct CFontParams {
  cb::string FontFileName;
  cb::string FontDir;
  cb::string FontChars;
  cb::string OutputName;
  cb::string OutputDir;
  cb::string OutputTexDir;
  unsigned FontSize;
  unsigned TexSize;
  unsigned TexCharBorder;
  glm::vec4 CharColor;
  glm::vec4 BackColor;
};

namespace data {
  struct CFont;
}

extern bool CompileFont(CFontParams const& params);