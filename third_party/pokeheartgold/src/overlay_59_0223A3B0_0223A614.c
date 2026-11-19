#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "sprite.h"
#include "touchscreen.h"

u32 ov59_0223A48C(Ov59ApricornApp *param_1, s32 param_2);

extern u32 ov59_0223C908;

u32 ov59_0223A614(Ov59ApricornApp *param_1, u32 *param_2);

u32 ov59_0223A614(Ov59ApricornApp *param_1, u32 *param_2) {
    s32 iVar1;
    u32 uVar2;
    s32 iVar3;

    iVar1 = TouchscreenHitbox_FindHitboxAtTouchNew((TouchscreenHitbox *)(&ov59_0223C908));
    if (iVar1 == (-1)) {
        return 6;
    }
    if (((iVar1 - 4U) <= (2 - 1)) && ((param_1->maxQuantity) < 10)) {
        return 6;
    }
    if ((1 + 1) <= iVar1) {
        iVar3 = iVar1 + 9;
        Sprite_SetAnimActiveFlag(param_1->sprites[iVar3], 1);
        Sprite_ResetAnimCtrlState(param_1->sprites[iVar3]);
    }
    *param_2 = 1;
    param_1->menuInputState = MENU_INPUT_STATE_TOUCH;
    uVar2 = ov59_0223A48C(param_1, iVar1);
    return uVar2;
}
