#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "bg_window.h"
#include "render_window.h"

void ov59_02238D74(Ov59ApricornApp *param_1);

void ov59_02238D74(Ov59ApricornApp *param_1) {
  ClearFrameAndWindow2(&param_1->windows[0],1);
  ClearWindowTilemapAndScheduleTransfer(&param_1->windows[0]);
  return;
}
