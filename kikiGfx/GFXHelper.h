#pragma once

#include <memory>

#include <CBCore/Defines.h>
#include <CBGL/Fwd.h>
#include <CoreFwd.h>

namespace gfx {
  using streammap = std::map<cb::u32, cb::string>;

  extern cb::gl::CProgram CreateShaderProgram(core::CAssetRepository<cb::gl::CShader>& repo,
                                              cb::strvector const& shaders,
                                              streammap const& inputs = streammap(),
                                              streammap const& outputs = streammap());
}