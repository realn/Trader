#include "stdafx.h"
#include "CoreApp.h"
#include "CoreAppTask.h"

#include <CBSDL/System.h>
#include <CBSDL/GLContext.h>
#include <CBGL/System.h>

namespace core {
  CAppBase::CAppBase(CAppConfig const& config)
    : mConfig(config)
    , mSystem(cb::sdl::System::VIDEO | cb::sdl::System::TIMER | cb::sdl::System::EVENTS) 
  {}

  CAppBase::~CAppBase() {}

  int CAppBase::Execute() {
    if(!Init()) {
      return -1;
    }

    mTimer.Update();
    MainLoop();

    return 0;
  }

  void CAppBase::MainLoop() {
    auto frameTime = 0.0f;
    while(mRun) {
      frameTime += mTimer.GetTimeDelta();

      Render();
      
      ProcessEvents();
      Update(frameTime);

      mGLContext->SwapWindow(*mWindow);

      UpdateRender();

      mTimer.Update();
    }
  }

  bool CAppBase::Init() {
    mTask = CreateTask();
    mTask->PrepareConfig(mConfig);

    mWindow =
      std::make_unique<cb::sdl::CWindow>(L"Trader Alpha"s,
                                         mConfig.WindowPos, mConfig.WindowSize,
                                         cb::sdl::WindowFlag::OPENGL | cb::sdl::WindowFlag::POPUP_MENU);
    mWindow->Show();

    {
      using namespace cb::sdl;
      auto attribs = GLAttributeMapT{
        {GLAttribute::BUFFER_SIZE, 32},
        {GLAttribute::DEPTH_SIZE, 24},
        {GLAttribute::STENCIL_SIZE, 8},
        {GLAttribute::DOUBLEBUFFER, 1},
      };
      mGLContext = 
        std::make_unique<cb::sdl::CGLContext>(*mWindow, attribs);
    }
    mGLContext->MakeCurrent(*mWindow);

    return true;
  }

  void CAppBase::ProcessEvents() {
    using namespace cb::sdl;
    auto event = CEvent();
    for(int i = 0u; mConfig.EventMaxIters; i++) {
      if(!CEvent::Poll(event))
        return;

      if(event.GetType() == EventType::WINDOWEVENT &&
         event.Window().GetType() == WindowEventType::CLOSE) {
        mRun = false;
      }
    }
  }

  void CAppBase::Update(float& frameTime) {
    for(auto i = 0u; i < mConfig.UpdateMaxStepsPerFrame; i++) {
      if(frameTime < mConfig.UpdateTimeStep)
        return;

      UpdateFrame(mConfig.UpdateTimeStep);
      frameTime -= mConfig.UpdateTimeStep;
    }
  }

  void CAppBase::UpdateFrame(float const timeDelta) {
    mTask->Update(timeDelta);
  }

  void CAppBase::UpdateRender() {
    mTask->UpdateRender();
  }

  void CAppBase::Render() {
    mTask->Render();
  }
}
