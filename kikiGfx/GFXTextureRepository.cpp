#include "stdafx.h"
#include "GFXTextureRepository.h"

#include <CBCore/Path.h>
#include <CBSDL/Surface.h>
#include <CBGL/Texture.h>

namespace gfx {
  CTextureRepository::CTextureRepository(cb::string const & assetsDir)
    : core::CAssetRepository<cb::gl::CTexture>(assetsDir, L"png"s)
  {}

  CTextureRepository::~CTextureRepository() {}

  std::shared_ptr<cb::gl::CTexture> CTextureRepository::Load(cb::string const & name) const {
    auto path = GetAssetPath(cb::filenamebase(name));
    auto surface = cb::sdl::CSurface::Load(path);
    surface = surface.Convert(cb::sdl::PixelFormat::RGBA32);

    auto texture = std::make_shared<cb::gl::CTexture>(surface.GetSize(), cb::gl::TextureFormat::RGBA8);
    texture->SetData(cb::gl::InputFormat::RGBA, surface.GetPixels());

    return texture;
  }
}
