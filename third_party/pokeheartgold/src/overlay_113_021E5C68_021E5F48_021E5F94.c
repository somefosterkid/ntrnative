#include "global.h"

#include "touchscreen.h"
#include "overlay_113.h"

u32 ov113_021E5ED0(UnownReportData *, s32 arg1);

extern u32 ov113_021E6B82;

u32 ov113_021E5F94(UnownReportData *param_1, u32 *param_2);

u32 ov113_021E5F94(UnownReportData *param_1, u32 *param_2) {
    s32 iVar1;
    u32 uVar2;

    iVar1 = TouchscreenHitbox_FindHitboxAtTouchNew((TouchscreenHitbox *)&ov113_021E6B82);
    if (iVar1 == -1) {
        return 2;
    }
    *param_2 = 1;
    param_1->menuInputState = MENU_INPUT_STATE_TOUCH;
    uVar2 = ov113_021E5ED0(param_1, iVar1);
    return uVar2;
}
