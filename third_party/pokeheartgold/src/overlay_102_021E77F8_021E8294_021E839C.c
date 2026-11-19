#include "global.h"

#include "touchscreen.h"

extern u32 ov102_021EC600;

u32 ov102_021E839C(void *param_1);

u32 ov102_021E839C(void *param_1) {
    s32 iVar1;

    iVar1 = TouchscreenHitbox_FindRectAtTouchNew((TouchscreenHitbox *)&ov102_021EC600);
    if (iVar1 == -1) {
        return 0;
    }
    switch (iVar1) {
    case 0:
        return 1;
    case 1:
        if (*(u8 *)(param_1 + 0x6b) == 0) {
            return 0;
        }
        break;
    case 2:
        if (*(u8 *)(param_1 + 0x6b) == 1) {
            return 0;
        }
        break;
    }
    return 2;
}
