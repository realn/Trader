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
#include <GFXMeshView.h>
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

    mMeshProgram = mRepositories->Shaders.Get(L"mesh_vs,mesh_fs"s);
    mMeshProgram->SetInLocation(gfx::CMeshVertex::Inputs);
    if(!mMeshProgram->Link()) {
      return false;
    }

    auto colors = std::array<glm::vec4, 6>{
      glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
        glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
        glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 0.0f, 1.0f},
        glm::vec4{0.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 0.0f, 1.0f, 1.0f}
    };
    //auto mesh = gfx::CMesh::CreateCube({1.0f, 1.0f, 1.0f}, colors, {3, 3, 3}, true);
    //auto mesh = gfx::CMesh::CreateCircle({1.0f, 1.0f}, 18, {1.0f, 1.0f, 0.0f, 1.0f}, 1.0f);
    auto mesh = 
      gfx::CMesh::CreateTube({1.0f, 1.0f}, {1.0f, 1.0f}, 1.0f, glm::two_pi<float>(), {18, 4}, true, {1.0f, 1.0f,1.0f,1.0f});
    mMeshView = std::make_unique<gfx::CMeshView>(mesh);

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

    auto transform =
      glm::perspective(glm::radians(60.0f), 16.0f / 9.0f, 1.0f, 100.0f) *
      glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -2.5f}) *
      glm::rotate(glm::mat4(1.0f), mRotation.x, {0.0f, 1.0f, 0.0f}) *
      glm::rotate(glm::mat4(1.0f), mRotation.y, {1.0f, 0.0f, 0.0f});

    auto gprog = cb::gl::bind(*mMeshProgram);
    auto gmesh = cb::gl::bind(*mMeshView);

    mMeshProgram->SetUniform(gfx::UNI_TRANSFORM, transform);
    mMeshView->Render();
  }

  void CTraderTask::OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) {
    if(button == cb::sdl::Button::LEFT) {
      mDrag = state == cb::sdl::KeyState::PRESSED;
    }
  }

  void CTraderTask::OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) {
    if(mDrag) {
      mRotation += glm::vec3(delta, 0.0f) * 10.0f;
      mRotation = glm::mod(mRotation, glm::radians(360.0f));
    }
  }

  void CTraderTask::OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) {
    if(code == cb::sdl::ScanCode::ESCAPE && state == cb::sdl::KeyState::PRESSED) {
      mExit = true;
    }
  }
}