#pragma once

#include "CoreAssetRepository.h"

namespace core {
  class CFont;
  class CFontRepository
    : public CAssetRepository<CFont> {
  public:
    CFontRepository(cb::string const& assetsDir);
    virtual ~CFontRepository();

    // Inherited via CAssetRepository
    virtual std::shared_ptr<CFont> Load(cb::string const & name) const override;
  };
}