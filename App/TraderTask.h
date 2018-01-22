#pragma once

#include <random>
#include <CoreAppTask.h>
#include <CoreInputEvents.h>
#include <GFXFwd.h>
#include <GUIFwd.h>

#include "Camera.h"

namespace eco {
  class CUniverse;
}

namespace trader {
  class CRepositories;
  class CUniverseView;
  class CTraderTask
    : public core::IAppTask
    , public core::IEventTarget<core::IInputMouseEvents> 
    , public core::IEventTarget<core::IInputKeyEvents>
  {
  private:
    std::random_device rand;

    std::unique_ptr<CRepositories> mRepositories;
    std::shared_ptr<cb::gl::CProgram> mMeshProgram;

    std::shared_ptr<gfx::CMeshView> mGridMesh;

    std::vector<glm::vec3> mPlanetPositions;

    std::shared_ptr<eco::CUniverse> mEcoUniverse;
    std::unique_ptr<CUniverseView> mEcoUniverseView;

    std::unique_ptr<gui::CLayerStack> mLayerStack;
    std::unique_ptr<gui::CScreen> mScreen;

    CViewportConfig mViewport;
    CCamera mCamera;
    bool mExit = false;
    bool mDrag = false;

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

  private:
    bool InitUniverse();
    bool InitGrid();
    bool InitGUI();

    void RenderGrid(glm::mat4 const& transform) const;
  };

}