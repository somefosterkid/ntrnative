#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222C3E8(void *param_1, PalPadGfx *param_2, s8 param_3);

void ov43_0222C3E8(void *param_1, PalPadGfx *param_2, s8 param_3) {
    NNSG2dScreenData *puVar1;
    puVar1 = param_2->screens.data[1];
    CopyToBgTilemapRect(param_2->bgConfig, 0, 0x10, 0, 0x10, 4, puVar1->rawData, 0, 4, (u8)(puVar1->screenWidth >> 3), (u8)(puVar1->screenHeight >> 3));
    puVar1 = param_2->screens.data[1];
    CopyToBgTilemapRect(param_2->bgConfig, 0, (param_3 * 4) + 0x10, 0, 4, 4, puVar1->rawData, param_3 << 2, 8, (u8)(puVar1->screenWidth >> 3), (u8)(puVar1->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(param_2->bgConfig, 0);
    return;
}
