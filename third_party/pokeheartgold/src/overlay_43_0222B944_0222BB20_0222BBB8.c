#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222AA70(PalPadGfx *);

void ov43_0222AD00(PalPadGfx *, u32);

void ov43_0222AD74(PalPadGfx *, u32, u32);

void ov43_0222AD98(PalPadGfx *, u8, u32);

void ov43_0222C9A4(u8 *, PalPadGfx *, PalPadScreens *);

void ov43_0222BBB8(u8 *param_1, void *param_2, PalPadGfx *param_3);

void ov43_0222BBB8(u8 *param_1, void *param_2, PalPadGfx *param_3) {
    NNSG2dScreenData *puVar1;
    u8 *iVar2;
    s32 iVar3;

    ov43_0222AA70(param_3);
    ClearWindowTilemapAndScheduleTransfer((Window *)(param_1 + 0xb8));
    iVar3 = 0;
    iVar2 = param_1 + 8;
    do {
        ov43_0222C9A4(iVar2, param_3, &param_3->screens);
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + 0x38;
    } while (iVar3 < 3);
    ov43_0222AD74(param_3, 0, 0xff);
    ov43_0222AD98(param_3, 1, 9);
    ov43_0222AD00(param_3, 0);
    puVar1 = param_3->screens.data[1];
    CopyToBgTilemapRect(param_3->bgConfig, 0, 0x11, 0x14, 0xe, 4, puVar1->rawData, 0x11, 0, (u8)(puVar1->screenWidth >> 3), (u8)(puVar1->screenHeight >> 3));
    puVar1 = param_3->screens.data[1];
    CopyToBgTilemapRect(param_3->bgConfig, 0, 0x10, 0, 0x10, 4, puVar1->rawData, 0x10, 4, (u8)(puVar1->screenWidth >> 3), (u8)(puVar1->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(param_3->bgConfig, 0);
    return;
}
