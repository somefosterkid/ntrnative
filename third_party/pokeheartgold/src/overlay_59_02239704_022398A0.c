#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "bg_window.h"
#include "yes_no_prompt.h"

void ov59_022398A0(Ov59ApricornApp *param_1);

void ov59_022398A0(Ov59ApricornApp *param_1) {
  s32 iVar1;
  Window *window;
  
  iVar1 = 0;
  window = &param_1->windows[0];
  do {
    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
    iVar1 = iVar1 + 1;
    window = window + 1;
  } while (iVar1 < 0x12);
  YesNoPrompt_Destroy(param_1->yesNoPrompt);
  return;
}
