#pragma once

#include <CoreAssetRepository.h>
#include <CBGL/Fwd.h>

namespace gfx {
  class CTextureRepository
    : public core::CAssetRepository<cb::gl::CTexture> {
  public:
    CTextureRepository(cb::string const& assetsDir);
    virtual ~CTextureRepository();

    // Inherited via CAssetRepository
    virtual std::shared_ptr<cb::gl::CTexture> Load(cb::string const & name) const override;

  };
}