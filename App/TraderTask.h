#pragma once

#include <CoreAppTask.h>
#include <CoreInputEvents.h>

namespace trader {
  class CTraderTask
    : public core::IAppTask
    , public core::IEventTarget<core::IInputMouseEvents> 
    , public core::IEventTarget<core::IInputKeyEvents>
  {
  private:
    bool mExit = false;

  public:
    CTraderTask(core::CAppBase& app);
    virtual ~CTraderTask();

    // Inherited via IAppTask
    virtual void PrepareConfig(core::CAppConfig& config);

    virtual bool Init(core::CAppBase & app) override;

    virtual void Update(core::CAppBase& app, float const & timeDelta) override;

    virtual void UpdateRender() override;

    virtual void Render() override;

    // Inherited via IEventTarget
    virtual void OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) override;

    virtual void OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) override;

    virtual void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) override;

  };

}