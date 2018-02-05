#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include <CBStr/Defines.h>
#include <CBSDL/Timer.h>
#include <CBSDL/System.h>
#include <CBSDL/Fwd.h>

#include "CoreEvents.h"

namespace core {
  class CAppConfig {
  public:
    float UpdateTimeStep = 0.0025f;
    cb::u32 UpdateMaxStepsPerFrame = 20;
    cb::u32 EventMaxIters = 20;
    glm::ivec2 WindowPos = {20, 20};
    glm::uvec2 WindowSize = {800, 480};
  };

  class IAppTask;
  class IInputMouseEvents;
  class IInputKeyEvents;

  class CAppBase
    : public IEventSource<IInputMouseEvents>
    , public IEventSource<IInputKeyEvents>
  {
  private:
    CAppConfig mConfig;
    cb::sdl::CSystem mSystem;
    cb::sdl::CPerfTimer mTimer;
    std::unique_ptr<cb::sdl::CWindow> mWindow;
    std::unique_ptr<cb::sdl::CGLContext> mGLContext;
    std::unique_ptr<IAppTask> mTask;
    bool mRun = true;

  public:
    CAppBase(CAppConfig const& config = CAppConfig());
    CAppBase(CAppBase&&) = default;
    virtual ~CAppBase();

    int Execute();

    void Quit() { mRun = false; }

  private:
    void MainLoop();

    bool Init();

    void ProcessEvents();
    void Update(float& frameTime);
    void UpdateFrame(float const timeDelta);
    void UpdateRender(float const timeDelta);
    void Render();

    void ProcessWindowEvent(cb::sdl::CEvent const& event);
    void ProcessMouseEvent(cb::sdl::CEvent const& event);
    void ProcessKeyEvent(cb::sdl::CEvent const& event);

  protected:
    virtual std::unique_ptr<IAppTask> CreateTask() = 0;
  };

  template<typename _Type>
  class CApp
    : public CAppBase {
  public:
    CApp(CAppConfig const& config = CAppConfig()) : CAppBase(config) {}
    CApp(CApp&&) = default;
    virtual ~CApp() = default;

  protected:
    std::unique_ptr<IAppTask> CreateTask() override {
      return std::make_unique<_Type>(*this);
    }
  };
}