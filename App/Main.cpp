#include <CoreApp.h>

#include "TraderTask.h"

int WinMain(void* prevInst, void* thisInst, const char* szCmdLine, int show) {
  auto app = core::CApp<trader::CTraderTask>();
  return app.Execute();
}