#include "stdafx.h"
#include "GFXHelper.h"

#include <CoreAssetRepository.h>

cb::gl::CProgram gfx::CreateShaderProgram(core::CAssetRepository<cb::gl::CShader>& repo,
                                          cb::strvector const & shaders,
                                          streammap const & inputs,
                                          streammap const & outputs) {
  auto program = cb::gl::CProgram();
  for(auto& shader : shaders) {
    auto shaderobj = repo.Get(shader);
    program.Attach(*shaderobj);
  }
  if(!inputs.empty())
    program.SetInLocation(inputs);
  if(!outputs.empty())
    program.SetOutLocation(outputs);

  if(!program.Link()) {
    cb::error(L"Failed to link program");
    cb::debug(program.GetLinkLog());
    throw std::exception("Shader program linking failed.");
  }
  return program;
}
