#include "stdafx.h"
#include "TraderTask.h"

#include <CoreApp.h>

int WinMain(void* prevInst, void* thisInst, const char* szCmdLine, int show) {
  auto app = core::CApp<trader::CTraderTask>();
  return app.Execute();
}