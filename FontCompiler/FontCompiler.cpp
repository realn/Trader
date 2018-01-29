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
  auto fontPath = cb::makepath(params.FontDir, params.FontFileName);

  auto font = cb::sdl::CFont(fontPath, params.FontSize);
  auto finalSurface = cb::sdl::CSurface(glm::uvec2(params.TexSize), 32, cb::sdl::PixelFormat::RGBA32);
  finalSurface.Fill(glm::vec4(0.0f));

  auto ctx = CCompileContext();
  ctx.LineHeight = font.GetHeight();
  ctx.Ascent = glm::ivec2(0, font.GetAscent());
  ctx.Descent = glm::ivec2(0, font.GetDescent());
  ctx.TexPos = glm::uvec2(params.TexCharBorder);

  auto outName = cb::filenamebase(params.OutputName);

  auto outFont = data::CFont();
  outFont.mName = font.GetName();
  outFont.mTexture = cb::makefilename(outName, L"png"s);
  outFont.mLineHeight = ctx.LineHeight;
  outFont.mTextureSize = glm::uvec2(params.TexSize);
  outFont.mAscent = font.GetAscent();
  outFont.mDescent = font.GetDescent();

  for(auto& item : params.FontChars) {
    auto glyphSurface = font.RenderGlyphBlended(item, params.CharColor);
    auto glyphSize = glyphSurface.GetSize();
    auto metrics = font.GetGlyphMetrics(item);

    auto fontChar = data::CFontChar{item};

    ctx.RowHeight = std::max(ctx.RowHeight, glyphSize.y);
    if(ctx.TexPos.x + glyphSize.x + params.TexCharBorder > params.TexSize) {
      ctx.TexPos.x = params.TexCharBorder;
      ctx.TexPos.y += ctx.RowHeight + params.TexCharBorder;
      if(ctx.TexPos.y > params.TexSize) {
        break;
      }
      ctx.RowHeight = 0;
    }

    finalSurface.Paste(ctx.TexPos, glyphSurface);
    fontChar.mTexMin = ctx.TexPos;// / glm::vec2(static_cast<float>(params.TexSize));
    fontChar.mTexMax = ctx.TexPos + glyphSize;// / glm::vec2(static_cast<float>(params.TexSize));

    ctx.TexPos.x += glyphSize.x + params.TexCharBorder;

    fontChar.mMin = metrics.min;// / glm::vec2(static_cast<float>(ctx.LineHeight));
    fontChar.mMax = metrics.max;// / glm::vec2(static_cast<float>(ctx.LineHeight));
    fontChar.mAdv = glm::ivec2(metrics.advance, 0);// / glm::vec2(static_cast<float>(ctx.LineHeight));

    outFont.mChars.push_back(fontChar);
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
