#pragma once

namespace core {
  class CAppConfig;
  class CAppBase;

  class IAppTask {
  public:
    IAppTask(IAppTask const&) = delete;
    IAppTask(IAppTask&&) = default;
    virtual ~IAppTask() = default;

    virtual void PrepareConfig(CAppConfig& config) {}
    virtual bool InitTask(CAppBase& app) {}

    virtual void Update(float const& timeDelta) = 0;
    virtual void UpdateRender() = 0;
    virtual void Render() = 0;
  };
}