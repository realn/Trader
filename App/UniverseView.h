#pragma once

#include <memory>
#include <vector>

#include <CBGL\Fwd.h>

namespace trader {
  class CEntityView;
  class CUniverseView {
  public:
    using EntitiesT = std::vector<std::shared_ptr<CEntityView>>;

  private:
    EntitiesT mEntities;

    EntitiesT const& GetEntities() const { return mEntities; }
  };
}