#include <iostream>
#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <CBStr/StringEx.h>

#include <CBSDL/System.h>
#include <CBSDL/Font.h>
#include <CBSDL/Surface.h>

#include <CBIO/File.h>

#include "FontCompiler.h"

using namespace std::literals;

cb::strvector convMainArgs(int argc, char* argv[]) {
  auto result = cb::strvector();
  std::transform(argv, argv + argc, std::back_inserter(result), [](auto& item) -> auto{
    return cb::fromUtf8(item);
  });
  return result;
}

int main(int argc, char* argv[]) {
  auto args = convMainArgs(argc, argv);

  auto params = CFontParams();
  params.FontPath = L"Instruction.otf"s;
  params.TexSize = 512u;
  params.FontSize = 46u;
  params.FontChars = L"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$^&*()-_=+[]{};:'\",./<>?|\\"s;
  params.TexCharBorder = 6u;
  params.CharColor = glm::vec4(1.0f);
  params.OutputName = L"font";
  params.OutputPath = L"../PongGame/";

  auto sdlSys = cb::sdl::CSystem(cb::sdl::System::VIDEO);

  if(!CompileFont(params)) {
    return -1;
  }

  return 0;
}