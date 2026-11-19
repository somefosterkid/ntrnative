#include "global.h"
#include "menu_input_state.h"
#include "touchscreen.h"
#include "overlay_59_juice_feed_app.h"

u32 ov59_0223B2B4(void *param_1, u32 param_2);


u32 ov59_0223B358(Ov59JuiceFeedApp *param_1);

u32 ov59_0223B358(Ov59JuiceFeedApp *param_1) {
  u32 uVar1;
  
  if ((((u32 *)&gSystem)[0x12] & 0xcf3) != 0) {
    param_1->menuInputState = MENU_INPUT_STATE_BUTTONS;
  }
  if ((((u32 *)&gSystem)[0x12] & 2) != 0) {
    uVar1 = ov59_0223B2B4(param_1,1);
    return uVar1;
  }
  if ((((u32 *)&gSystem)[0x12] & 1) != 0) {
    uVar1 = ov59_0223B2B4(param_1,0);
    return uVar1;
  }
  if ((((u32 *)&gSystem)[0x12] & 0x40) != 0) {
    uVar1 = ov59_0223B2B4(param_1,2);
    return uVar1;
  }
  if ((((u32 *)&gSystem)[0x12] & 0x80) != 0) {
    uVar1 = ov59_0223B2B4(param_1,3);
    return uVar1;
  }
  return 2;
}

extern u32 ov59_0223C95C;

u32 ov59_0223B3B0(Ov59JuiceFeedApp *param_1, u32 *param_2);

u32 ov59_0223B3B0(Ov59JuiceFeedApp *param_1, u32 *param_2) {
  s32 iVar1;
  u32 uVar2;
  
  iVar1 = TouchscreenHitbox_FindHitboxAtTouchNew((TouchscreenHitbox *)&ov59_0223C95C);
  if (iVar1 == -1) {
    return 2;
  }
  *param_2 = 1;
  param_1->menuInputState = MENU_INPUT_STATE_TOUCH;
  uVar2 = ov59_0223B2B4(param_1,iVar1);
  return uVar2;
}
