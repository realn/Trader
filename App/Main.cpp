
#include <CoreApp.h>
#include <CoreAppTask.h>

class CTraderAppTask 
  : public core::IAppTask
{
public:

};

int WinMain(void* prevInst, void* thisInst, const char* szCmdLine, int show) {
  auto app = core::CApp();
  return app.Execute();
}