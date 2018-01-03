#include "stdafx.h"

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
#include <ECOUniverse.h>
#include <ECOEntity.h>
#include <ECOTradeRoute.h>

#include <ECOCompMarket.h>
#include <ECOCompDock.h>
#include <ECOCompWarpDrive.h>
#include <ECOCompNavigation.h>
#include <ECOCompIndustry.h>

#include "UniverseView.h"
#include "Repositories.h"
#include "TraderTask.h"

namespace trader {
  static const auto ASSETS_DIR = L"assets"s;

  CTraderTask::CTraderTask(core::CAppBase& app)
    : core::IAppTask(app) {}

  CTraderTask::~CTraderTask() {}

  void CTraderTask::PrepareConfig(core::CAppConfig & config) {
    config.WindowPos = cb::sdl::CWindow::PosCentered;
    config.WindowSize = { 1024, 600 };

    mViewport.SetAspectRatio(config.WindowSize);
    mCamera.SetOffset(glm::vec3(0.0f, 0.0f, -10.0f));
  }

  bool CTraderTask::Init(core::CAppBase & app) {
    core::bind<core::IInputMouseEvents>(app, *this);
    core::bind<core::IInputKeyEvents>(app, *this);

    mRepositories = std::make_unique<CRepositories>(ASSETS_DIR);

    cb::gl::clearColor({ 0.0f, 0.0f, 0.01f, 1.0f });
    cb::gl::clearDepth(1.0f);

    cb::gl::setState({ cb::gl::DepthFunc::LEQUAL });
    cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, true);

    mMeshProgram = mRepositories->Shaders.Get(L"mesh_vs,mesh_fs"s);
    mMeshProgram->SetInLocation(gfx::CMeshVertex::Inputs);
    if(!mMeshProgram->Link()) {
      return false;
    }

    mEcoUniverse = std::make_shared<eco::CUniverse>();

    {
      auto positions = std::vector<glm::vec2>{
        {2.0f, -4.0f},
        {6.4f, -2.0f},
        {4.3f, 2.0f},
        {-5.4f, 0.5f},
        {0.0f, 4.0f},
        {-3.8, -5.0f}
      };

      eco::CFactoryTemplate solarTemplate;
      solarTemplate.SetOutput(L"energy", 10.0f);

      auto planetId = L"Planet"s;
      for(auto& pos : positions) {
        auto entity = std::make_shared<eco::CEntity>(planetId);
        entity->SetPosition(pos);
        entity->SetComponent<eco::comp::CMarket>();
        entity->SetComponent<eco::comp::CDock>();
        entity->SetComponent<eco::comp::CIndustry>();
        {
          auto& industry = entity->GetComponent<eco::comp::CIndustry>();
          industry.SetFactory(L"solarArray", solarTemplate);
        }
        mEcoUniverse->AddEntity(entity);
      }
    }

    for(auto i = 0u; i < 8u; i++)
    {
      auto entity = std::make_shared<eco::CEntity>(L"Ship");
      entity->SetComponent<eco::comp::CWarpDrive>();
      entity->SetComponent<eco::comp::CNavigation>();

      mEcoUniverse->AddEntity(entity);

      auto docks = mEcoUniverse->GetEntities(eco::GetComponentId<eco::comp::CDock>());
      docks.front()->GetComponent<eco::comp::CDock>().DockShip(entity);
    }

    mEcoUniverseView = std::make_unique<CUniverseView>(mEcoUniverse);

    {
      using namespace glm;
      auto mesh =
        rotate(mat4(1.0f), radians(90.0f), { 1.0f, 0.0f, 0.0f }) *
        gfx::CMesh::CreatePlane(vec2(20.0f), { 0.1f,0.1f,0.1f,1.0f }, uvec2(10), true);
      mGridMesh = std::make_shared<gfx::CMeshView>(mesh);
    }

    return true;
  }

  void CTraderTask::Update(core::CAppBase & app, float const & timeDelta) {
    if(mExit) {
      app.Quit();
      return;
    }

    mEcoUniverse->UpdateEntities(timeDelta);
  }

  void CTraderTask::UpdateRender() {}

  void CTraderTask::Render() {
    cb::gl::clear(cb::gl::ClearBuffer::COLOR | cb::gl::ClearBuffer::DEPTH);

    auto transform =
      mViewport.GetProjection() * mCamera.GetTransform();

    auto gprog = cb::gl::bind(*mMeshProgram);
    RenderGrid(transform);
    mEcoUniverseView->Render(transform, *mMeshProgram, mRepositories->Meshes);

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
    }
  }

  void CTraderTask::OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) {
    if(code == cb::sdl::ScanCode::ESCAPE && state == cb::sdl::KeyState::PRESSED) {
      mExit = true;
    }
  }

  void CTraderTask::RenderGrid(glm::mat4 const & transform) const {
    auto gmesh = cb::gl::bind(*mGridMesh);
    mMeshProgram->SetUniform(gfx::UNI_TRANSFORM, transform);
    mGridMesh->Render();
  }
}