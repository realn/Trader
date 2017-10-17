#include "stdafx.h"
#include "TraderTask.h"
#include "Repositories.h"

#include <glm/vec4.hpp>

#include <CBSDL/Consts.h>
#include <CBSDL/Window.h>
#include <CBGL/Rendering.h>
#include <CBGL/State.h>
#include <CBGL/Program.h>

#include <CoreApp.h>
#include <GFXMesh.h>
#include <GFXConsts.h>

namespace trader {
  static const auto ASSETS_DIR = L"assets"s;

  CTraderTask::CTraderTask(core::CAppBase& app)
    : core::IAppTask(app) {}

  CTraderTask::~CTraderTask() {}

  void CTraderTask::PrepareConfig(core::CAppConfig & config) {
    config.WindowPos = cb::sdl::CWindow::PosCentered;
    config.WindowSize = {1024, 600};
  }

  bool CTraderTask::Init(core::CAppBase & app) {
    core::bind<core::IInputMouseEvents>(app, *this);
    core::bind<core::IInputKeyEvents>(app, *this);

    mRepositories = std::make_unique<CRepositories>(ASSETS_DIR);

    cb::gl::clearColor({0.1f, 0.1f, 0.1f, 1.0f});
    cb::gl::clearDepth(1.0f);
    
    cb::gl::setState({cb::gl::DepthFunc::LEQUAL});
    cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, true);

    mMeshProgram = mRepositories->Shaders.Get(L"mesh"s);
    mMeshProgram->SetInLocation({
      {gfx::IDX_VERTEX3_POS, gfx::VIN_VERTEX3_POS},
      {gfx::IDX_VERTEX3_NORMAL, gfx::VIN_VERTEX3_NORMAL},
      {gfx::IDX_VERTEX3_COLOR, gfx::VIN_VERTEX3_COLOR},
    });
    if(!mMeshProgram->Link()) {
      return false;
    }

    return true;
  }

  void CTraderTask::Update(core::CAppBase & app, float const & timeDelta) {
    if(mExit) {
      app.Quit();
    }
  }

  void CTraderTask::UpdateRender() {}

  void CTraderTask::Render() {
    cb::gl::clear(cb::gl::ClearBuffer::COLOR | cb::gl::ClearBuffer::DEPTH);

  }

  void CTraderTask::OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) {
    if(button == cb::sdl::Button::LEFT && state == cb::sdl::KeyState::PRESSED) {
    }
  }

  void CTraderTask::OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) {}

  void CTraderTask::OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) {
    if(code == cb::sdl::ScanCode::ESCAPE && state == cb::sdl::KeyState::PRESSED) {
      mExit = true;
    }
  }
}