
#include <CoreApp.h>
#include <CoreAppTask.h>

class CTraderAppTask 
  : public core::IAppTask
{
public:
  CTraderAppTask(core::CAppBase& app)
    : core::IAppTask(app) {}
  virtual ~CTraderAppTask() = default;

  // Inherited via IAppTask
  virtual bool InitTask(core::CAppBase & app) override {
    return true;
  }

  virtual void Update(float const & timeDelta) override {}

  virtual void UpdateRender() override {}

  virtual void Render() override {}
};

int WinMain(void* prevInst, void* thisInst, const char* szCmdLine, int show) {
  auto app = core::CApp<CTraderAppTask>();
  return app.Execute();
}