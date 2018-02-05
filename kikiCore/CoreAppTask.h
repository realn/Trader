#pragma once

#include <CBStr/Defines.h>
#include <CBSDL/Fwd.h>
#include <glm/fwd.hpp>

namespace core {
  class CAppConfig;
  class CAppBase;

  class IAppTask {
  public:
    IAppTask(CAppBase&) {}
    IAppTask(IAppTask const&) = delete;
    IAppTask(IAppTask&&) = default;
    virtual ~IAppTask() = default;

    virtual void PrepareConfig(CAppConfig& config) {}
    virtual bool Init(CAppBase& app) = 0;

    virtual void Update(CAppBase& app, float const& timeDelta) = 0;
    virtual void UpdateRender(float const timeDelta) = 0;
    virtual void Render() = 0;

  };
}