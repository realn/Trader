#pragma once

#include <CoreAssetRepository.h>
#include <CBGL/Fwd.h>

namespace gfx {
  class CShaderRepository
    : public core::CAssetRepository<cb::gl::CProgram> {
  public:
    CShaderRepository(cb::string const& assetsDir);
    virtual ~CShaderRepository();

    // Inherited via CAssetRepository
    virtual std::shared_ptr<cb::gl::CProgram> Load(cb::string const & name) const override;
    virtual std::shared_ptr<cb::gl::CProgram> Load(cb::strvector const & names) const override;

  private:
    std::shared_ptr<cb::gl::CProgram> CreateProgram(std::vector<cb::gl::CShader>& shaders) const;
    cb::gl::CShader LoadShader(cb::gl::ShaderType const type, cb::string const& path) const;
    cb::gl::ShaderType FindShaderType(cb::string const& name) const;
  };
}