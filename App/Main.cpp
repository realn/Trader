#include "stdafx.h"
#include "TraderTask.h"

#include <CoreApp.h>

extern "C" {
  _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}

int WinMain(void* prevInst, void* thisInst, const char* szCmdLine, int show) {
  auto app = core::CApp<trader::CTraderTask>();
  return app.Execute();
}