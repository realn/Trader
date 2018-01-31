#include "FontCompiler.h"
#include "../kikiCore/DataFont.h"

#include <glm/glm.hpp>
#include <CBSDL/Font.h>
#include <CBSDL/Surface.h>
#include <CBIO/File.h>
#include <CBIO/Path.h>

struct CCompileContext {
  glm::uvec2 TexPos;
  glm::ivec2 Descent;
  glm::ivec2 Ascent;
  unsigned LineHeight = 0u;
  unsigned RowHeight = 0u;
};

bool CompileFont(CFontParams const & params) {
  using namespace glm;

  auto fontPath = cb::makepath(params.FontDir, params.FontFileName);

  auto font = cb::sdl::CFont(fontPath, params.FontSize);
  auto finalSurface = cb::sdl::CSurface(uvec2(params.TexSize), 32, cb::sdl::PixelFormat::RGBA32);
  finalSurface.Fill(params.BackColor);

  auto ctx = CCompileContext();
  ctx.LineHeight = font.GetHeight();
  ctx.Ascent = ivec2(0, font.GetAscent());
  ctx.Descent = ivec2(0, font.GetDescent());
  ctx.TexPos = uvec2(params.TexCharBorder);

  auto outName = cb::filenamebase(params.OutputName);

  auto outFont = data::CFont();
  outFont.mName = font.GetName();
  outFont.mTexture = cb::makefilename(outName, L"png"s);
  outFont.mLineHeight = ctx.LineHeight;
  outFont.mLineSkip = font.GetLineSkip();
  outFont.mTextureSize = uvec2(params.TexSize);
  outFont.mAscent = font.GetAscent();
  outFont.mDescent = font.GetDescent();

  for(auto& item : params.FontChars) {
    auto glyphSurface = font.RenderGlyphBlended(item, params.CharColor);
    auto glyphSize = glyphSurface.GetSize();
    auto metrics = font.GetGlyphMetrics(item);

    finalSurface.Paste(ctx.TexPos, glyphSurface);

    auto fontChar = data::CFontChar();
    fontChar.mCode = item;

    fontChar.mTexMin = ctx.TexPos;
    fontChar.mTexMax = ctx.TexPos + glyphSize;

    fontChar.mMin = metrics.min;
    fontChar.mMax = metrics.max;
    fontChar.mAdv = glm::ivec2(metrics.advance, 0);

    outFont.mChars.push_back(fontChar);

    ctx.TexPos.x += glyphSize.x + params.TexCharBorder;
    ctx.RowHeight = std::max(ctx.RowHeight, glyphSize.y);
    if(ctx.TexPos.x + glyphSize.x + params.TexCharBorder > params.TexSize) {
      ctx.TexPos.x = params.TexCharBorder;
      ctx.TexPos.y += ctx.RowHeight + params.TexCharBorder;
      if(ctx.TexPos.y > params.TexSize) {
        break;
      }
      ctx.RowHeight = 0;
    }
  }

  finalSurface.SavePNG(cb::makepath(params.OutputTexDir, outFont.mTexture));

  auto xmlDoc = cb::CXmlDocument();
  cb::WriteXmlObject(xmlDoc.RootNode, outFont);
  xmlDoc.RootNode.SetName(L"Font");

  auto outfilename = cb::makefilename(outName, L"xml"s);
  auto result = xmlDoc.ToString();
  cb::writetextfileutf8(cb::makepath(params.OutputDir, outfilename), result);
  return true;
}
