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

template<class _Type>
_Type takeArg(cb::strvector const& args, cb::string const& arg, _Type const& defValue) {
  auto it = std::find(args.begin(), args.end(), arg);
  if(it == args.end()) {
    return defValue;
  }
  it++;
  if(it == args.end()) {
    return defValue;
  }
  auto value = _Type();
  if(cb::fromStr(*it, value)) {
    return value;
  }
  return defValue;
}

static auto const DEF_FONT_CHARS_LOWER = L"abcdefghijklmnopqrstuvwxyz"s;
static auto const DEF_FONT_CHARS_UPPER = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"s;
static auto const DEF_FONT_CHARS_NUMBER = L"0123456789"s;
static auto const DEF_FONT_CHARS_SPECIAL = L" `~!@#$^&*()-_=+[]{};:'\",./<>?|\\"s;
static auto const DEF_FONT_CHARS = 
  DEF_FONT_CHARS_LOWER + 
  DEF_FONT_CHARS_UPPER + 
  DEF_FONT_CHARS_NUMBER + 
  DEF_FONT_CHARS_SPECIAL;

int main(int argc, char* argv[]) {
  auto args = convMainArgs(argc, argv);

  auto params = CFontParams();
  params.FontFileName = takeArg(args, L"-font"s, L"Instruction.otf"s);
  params.FontDir = takeArg(args, L"-path"s, L"assets"s);
  params.TexSize = takeArg(args, L"-texSize"s, 512u);
  params.FontSize = takeArg(args, L"-fontSize"s, 54u);
  params.FontChars = DEF_FONT_CHARS;
  params.TexCharBorder = takeArg(args, L"-texBorder"s, 2u);
  params.CharColor = glm::vec4(1.0f);
  params.BackColor = glm::vec4(0.0f);
  params.OutputName = takeArg(args, L"-out"s, params.FontFileName);
  params.OutputDir = takeArg(args, L"-odir"s, L"../App/assets/fonts"s);
  params.OutputTexDir = takeArg(args, L"-otexdir"s, L"../App/assets/textures"s);

  auto sdlSys = cb::sdl::CSystem(cb::sdl::System::VIDEO);

  if(!CompileFont(params)) {
    return -1;
  }

  return 0;
}