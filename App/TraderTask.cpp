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

    mViewport.SetAspectRatio(config.WindowSize);
  }

  bool CTraderTask::Init(core::CAppBase & app) {
    core::bind<core::IInputMouseEvents>(app, *this);
    core::bind<core::IInputKeyEvents>(app, *this);

    mRepositories = std::make_unique<CRepositories>(ASSETS_DIR);

    cb::gl::clearColor({0.0f, 0.0f, 0.01f, 1.0f});
    cb::gl::clearDepth(1.0f);

    cb::gl::setState({cb::gl::DepthFunc::LEQUAL});
    cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, true);

    mMeshProgram = mRepositories->Shaders.Get(L"mesh_vs,mesh_fs"s);
    mMeshProgram->SetInLocation(gfx::CMeshVertex::Inputs);
    if(!mMeshProgram->Link()) {
      return false;
    }

    mPlanetPositions = std::vector<glm::vec3>{
      {2.0f, 0.0f, -4.0f},
      {6.4f, 0.0f, -2.0f},
      {4.3f, 0.0f, 2.0f},
      {-5.4f, 0.0f, 0.5f},
      {0.0f, 0.0f, 4.0f},
      {-3.8, 0.0f, -5.0f}
    };

    {
      using namespace glm;
      auto mesh =
        rotate(mat4(1.0f), radians(90.0f), {1.0f, 0.0f, 0.0f}) *
        gfx::CMesh::CreatePlane(vec2(20.0f), {0.1f,0.1f,0.1f,1.0f}, uvec2(10), true);
      mGridMesh = std::make_unique<gfx::CMeshView>(mesh);
    }

    {
      using namespace glm;
      auto mesh = gfx::CMesh::CreateTube({0.0f, 0.0f}, {0.2f, 0.2f}, 0.3f, two_pi<float>(), {8, 1}, false, {1.0f, 0.0f, 0.0f, 1.0f});
      mShipMesh = std::make_unique<gfx::CMeshView>(mesh);
    }

    {
      using namespace glm;
      auto c = vec4(1.0f, 1.0f, 0.0f, 1.0f);
      auto mesh = gfx::CMesh(cb::gl::PrimitiveType::LINES);
      for(auto& a : mPlanetPositions) {
        for(auto& b : mPlanetPositions) {
          if(a == b)
            continue;

          mesh += gfx::CMesh::CreateLine(a, b, {1.0f, 1.0f, 0.0f, 1.0f});

          auto road = static_cast<float>(rand()) / rand.max();

          auto v1 = vec3(0.0f, 0.0f, -1.0f);
          auto v2 = normalize(b - a);

          auto rot = normalize(angleAxis(dot(v1, v2), cross(v1, v2)));

          mEntities.push_back(CEntity(mix(a, b, road), rot));
        }
      }
      mLaneMesh = std::make_unique<gfx::CMeshView>(mesh);
    }

    {
      using namespace glm;
      auto mesh =
        rotate(mat4(1.0f), radians(-90.0f), {1.0f, 0.0f, 0.0f}) *
        //gfx::CMesh::CreateTube({0.2f, 0.2f}, {0.2f,0.2f}, 0.4f, two_pi<float>(), {12, 1}, false, {0.0f, 1.0f, 0.0f, 1.0f});
        gfx::CMesh::CreateSphere(vec3(1.0f), {16, 8}, {0.0f, 1.0f, 0.0f, 1.0f}, true);
      mPlanetMesh = std::make_unique<gfx::CMeshView>(mesh);
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

    auto transform =
      mViewport.GetProjection() * mCamera.GetTransform();

    auto gprog = cb::gl::bind(*mMeshProgram);
    mMeshProgram->SetUniform(gfx::UNI_TRANSFORM, transform);

    {
      auto gmesh = cb::gl::bind(*mPlanetMesh);
      mPlanetMesh->Render();
    }
    //{
    //  auto gmesh = cb::gl::bind(*mGridMesh);
    //  mGridMesh->Render();
    //}
    //{
    //  auto gmesh = cb::gl::bind(*mLaneMesh);
    //  mLaneMesh->Render();
    //}
    //{
    //  auto gmesh = cb::gl::bind(*mPlanetMesh);

    //  for(auto& pos : mPlanetPositions) {
    //    mMeshProgram->SetUniform(gfx::UNI_TRANSFORM, transform * glm::translate(glm::mat4(1.0f), pos));
    //    mPlanetMesh->Render();
    //  }
    //}
    //{
    //  auto gmesh = cb::gl::bind(*mShipMesh);

    //  for(auto& ent : mEntities) {
    //    mMeshProgram->SetUniform(gfx::UNI_TRANSFORM, transform * ent.GetTransform());
    //    mShipMesh->Render();
    //  }
    //}
  }

  void CTraderTask::OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) {
    if(button == cb::sdl::Button::LEFT) {
      mDrag = state == cb::sdl::KeyState::PRESSED;
    }
  }

  void CTraderTask::OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) {
    using namespace glm;
    if(mDrag) {
      auto rot =
        angleAxis(radians(delta.x * 100.0f), vec3(0.0f, 1.0f, 0.0f)) *
        angleAxis(radians(delta.y * 100.0f), vec3(1.0f, 0.0f, 0.0f));

      mCamera.ModRotation(rot);
      //mRotation += glm::vec3(delta, 0.0f) * 10.0f;
      //mRotation = glm::mod(mRotation, glm::radians(360.0f));
    }
  }

  void CTraderTask::OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) {
    if(code == cb::sdl::ScanCode::ESCAPE && state == cb::sdl::KeyState::PRESSED) {
      mExit = true;
    }
  }
}