#include "stdafx.h"

#include <iostream>

#include <glm/vec4.hpp>

#include <CBSDL/Consts.h>
#include <CBSDL/Window.h>
#include <CBSDL/Funcs.h>
#include <CBGL/Rendering.h>
#include <CBGL/State.h>
#include <CBGL/Program.h>

#include <CoreApp.h>
#include <CoreFont.h>
#include <CoreBLine.h>
#include <GFXMesh.h>
#include <GFXMeshView.h>
#include <GFXConsts.h>
#include <GFXCanvasVertex.h>
#include <ECOUniverse.h>
#include <ECOTradeRoute.h>
#include <ECOEntity.h>
#include <ECOEntityFactory.h>

#include <ECOCompMarket.h>
#include <ECOCompDock.h>
#include <ECOCompWarpDrive.h>
#include <ECOCompNavigation.h>
#include <ECOCompIndustry.h>
#include <ECOComponentFactory.h>

#include <GUIScreen.h>
#include <GUILayerStack.h>
#include <GUILayer.h>
#include <GUILabel.h>
#include <GUIRect.h>
#include <GUIAbsolute.h>
#include <GUIText.h>

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

    mMeshProgram = mRepositories->Shaders.Get(L"mesh_vs,mesh_fs"s);
    mMeshProgram->SetInLocation(gfx::CMeshVertex::Inputs);
    if(!mMeshProgram->Link()) {
      return false;
    }

    if(!InitGrid()) {
      return false;
    }

    if(!InitUniverse()) {
      return false;
    }

    if(!InitGUI()) {
      return false;
    }

    return true;
  }

  void CTraderTask::Update(core::CAppBase & app, float const & timeDelta) {
    if(mExit) {
      app.Quit();
      return;
    }

    mFrameTD += timeDelta;

    mEcoUniverse->UpdateEntities(timeDelta);
    mLayerStack->Update(timeDelta);
  }

  void CTraderTask::UpdateRender() {
    using namespace glm;

    auto textInfo = mLayerStack->FindById<gui::CText>(L"infoText"s);

    mEcoUniverseView->UpdateRender(mEcoUniverse, mRepositories->Meshes);
    mEcoUniverseView->SetEntityViewColorOverride(vec4(0.0f));
    if(mEcoSelectedEntity) {
      mEcoUniverseView->GetEntityView(mEcoSelectedEntity).SetColorOverride(mSelectedColor);
      auto buffer = cb::stringstream();
      mEcoSelectedEntity->PrintInfo(buffer);
      textInfo->SetText(buffer.str());
    }
    else {
      textInfo->SetText(L""s);
    }
    if(mEcoHighlightedEntity) {
      mEcoUniverseView->GetEntityView(mEcoHighlightedEntity).SetColorOverride(mHighLightColor);
    }

    mLayerStack->FindById<gui::CLabel>(L"labelFps"s)->SetText(cb::toStr(mFrameTD));
    mFrameTD = 0.0f;

    mLayerStack->UpdateRender(*mGuiFont);
    mScreen->UpdateRender(mLayerStack->GetCanvas(), mLayerStack->GetSize());
  }

  void CTraderTask::Render() {
    cb::gl::clear(cb::gl::ClearBuffer::COLOR | cb::gl::ClearBuffer::DEPTH);

    {
      auto gstate = cb::gl::bindStateEnabled(cb::gl::State::DEPTH_TEST, true);
      auto transform =
        mViewport.GetProjection() * mCamera.GetTransform();

      RenderGrid(transform);
      RenderUniverse(transform);
    }
    mScreen->Render();
  }

  void CTraderTask::OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) {
    if(button == cb::sdl::Button::LEFT) {
      mDrag = state == cb::sdl::KeyState::PRESSED;
    }
    if(button == cb::sdl::Button::LEFT && state == cb::sdl::KeyState::RELEASED) {
      mEcoSelectedEntity = mEcoHighlightedEntity;
    }
  }

  void CTraderTask::OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) {
    using namespace glm;

    auto lbpos = vec2(pos.x, 1.0f - pos.y);

    if(mDrag) {
      auto rot =
        angleAxis(radians(delta.x * 100.0f), vec3(0.0f, 1.0f, 0.0f)) *
        angleAxis(radians(delta.y * 100.0f), vec3(1.0f, 0.0f, 0.0f));

      mCamera.ModRotation(rot);
    }

    mLayerStack->FindById<gui::CAbsolute>(L"cursor"s)->SetPosition(lbpos);

    mEcoUniverseView->SetEntityViewColorOverride(vec4(0.0f));

    auto line = CreateScreenHitLine(lbpos);
    auto result = mEcoUniverseView->FindEntitiesByLine(line);
    if(!result.empty()) {
      mEcoHighlightedEntity = result.front();
      mEcoUniverseView->GetEntityView(mEcoHighlightedEntity).SetColorOverride(vec4(1.0f, 1.0f, 0.0f, 1.0f));
    }
    else {
      mEcoHighlightedEntity.reset();
    }
  }

  void CTraderTask::OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) {
    if(code == cb::sdl::ScanCode::ESCAPE && state == cb::sdl::KeyState::PRESSED) {
      mExit = true;
    }
  }

  bool CTraderTask::InitUniverse() {
    mFactoryTemplateRegistry = eco::CFactoryTemplateRegistry::GetInstance();
    mFactoryTemplateRegistry->Register(L"solarArray", eco::CFactoryTemplate(
      {},
      { {L"energy"s, 10.0f} }
    ));

    mComponentRegistry = eco::CComponentFactoryRegistry::GetInstance();
    mComponentRegistry->Register<eco::comp::CDock>();
    mComponentRegistry->Register<eco::comp::CIndustry>();
    mComponentRegistry->Register<eco::comp::CMarket>();
    mComponentRegistry->Register<eco::comp::CNavigation>();
    mComponentRegistry->Register<eco::comp::CWarpDrive>();

    mEntityRegistry = eco::CEntityFactoryRegistry::GetInstance();
    {
      auto ent = mEntityRegistry->Register(L"Planet"s);
      ent->SetComponent<eco::comp::CMarket>();
      ent->SetComponent<eco::comp::CDock>();
      ent->SetComponent<eco::comp::CIndustry>(cb::strvector{ L"solarArray" });
    }
    {
      auto ent = mEntityRegistry->Register(L"Ship"s);
      ent->SetComponent<eco::comp::CWarpDrive>();
      ent->SetComponent<eco::comp::CNavigation>();
    }

    mEcoUniverse = std::make_shared<eco::CUniverse>();

    {
      auto positions = std::vector<glm::vec2>{
        { 2.0f, -4.0f },
      { 6.4f, -2.0f },
      { 4.3f, 2.0f },
      { -5.4f, 0.5f },
      { 0.0f, 4.0f },
      { -3.8, -5.0f }
      };

      auto factory = mEntityRegistry->Get(L"Planet"s);
      for(auto& pos : positions) {
        auto entity = factory->Create();
        entity->SetPosition(pos);
        mEcoUniverse->AddEntity(entity);
      }
    }

    {
      auto factory = mEntityRegistry->Get(L"Ship"s);
      auto dock = mEcoUniverse->GetEntities(eco::GetComponentId<eco::comp::CDock>()).front();
      for(auto i = 0u; i < 8u; i++) {
        auto entity = factory->Create();
        mEcoUniverse->AddEntity(entity);
        dock->GetComponent<eco::comp::CDock>().DockShip(entity);
      }
    }

    mEcoUniverseView = std::make_unique<CUniverseView>();

    return true;
  }

  bool CTraderTask::InitGrid() {
    using namespace glm;
    auto mesh =
      rotate(mat4(1.0f), radians(90.0f), { 1.0f, 0.0f, 0.0f }) *
      gfx::CMesh::CreatePlane(vec2(20.0f), { 0.1f,0.1f,0.1f,1.0f }, uvec2(10), true);
    mGridMesh = std::make_shared<gfx::CMeshView>(mesh);
    return true;
  }

  bool CTraderTask::InitGUI() {
    using namespace glm;

    auto texAtlas = gfx::CTextureAtlas::Load(L"assets/gui/atlas_controls.xml"s);
    mGuiFont = mRepositories->Fonts.Get(L"Instruction"s);

    {
      auto shaderFont = mRepositories->Shaders.Get({
        L"font_vs"s,
        L"font_fs"s
                                                   });
      shaderFont->SetInLocation(gfx::CCanvasVertex::Inputs);
      if(!shaderFont->Link()) {
        return false;
      }

      auto textureFont = mRepositories->Textures.Get(mGuiFont->GetTextureFilePath());
      auto textureBase = mRepositories->Textures.Get(texAtlas.GetTextureFileName());

      mScreen = std::make_unique<gui::CScreen>(shaderFont, textureBase, textureFont);
    }

    mLayerStack = std::make_unique<gui::CLayerStack>(texAtlas, mViewport.CreateAspectCorrectSize(25));

    mLayerStack->Push(gui::CLayer::LoadPtr(L"assets/gui/layer_info.xml"s));
    mLayerStack->Push(gui::CLayer::LoadPtr(L"assets/gui/layer_cursor.xml"s));
    mLayerStack->Push(gui::CLayer::LoadPtr(L"assets/gui/layer_consttop.xml"s));

    cb::sdl::SetCursorVisible(false);

    return true;
  }

  void CTraderTask::RenderGrid(glm::mat4 const & transform) const {
    auto gprog = cb::gl::bind(*mMeshProgram);
    auto gmesh = cb::gl::bind(*mGridMesh);
    mMeshProgram->SetUniform(gfx::UNI_TRANSFORM, transform);
    mMeshProgram->SetUniform(gfx::UNI_MESHCOLOR, glm::vec4(0.0f));
    mGridMesh->Render();
  }

  void CTraderTask::RenderUniverse(glm::mat4 const transform) const {
    mEcoUniverseView->Render(transform, *mMeshProgram);
  }

  core::CBLine CTraderTask::CreateScreenHitLine(glm::vec2 lbpos) const {
    using namespace glm;
    auto transform = mCamera.GetTransform();
    auto proj = mViewport.GetProjection();
    auto viewport = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    auto beg = unProject(vec3(lbpos, 0.0f), transform, proj, viewport);
    auto end = unProject(vec3(lbpos, 1.0f), transform, proj, viewport);
    return { beg, end };
  }
}
