#include "global.h"
#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_59_juice_feed_app.h"

void ov59_0223B7D8(Ov59JuiceFeedApp *param_1);

void ov59_0223B7D8(Ov59JuiceFeedApp *param_1) {
  s32 iVar1;
  Window *window;
  
  iVar1 = 0;
  window = param_1->windows;
  do {
    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
    iVar1 = iVar1 + 1;
    window = window + 1;
  } while (iVar1 < 10);
  YesNoPrompt_Destroy(param_1->yesNoPrompt);
  return;
}
