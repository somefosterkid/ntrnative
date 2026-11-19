#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"

s32 ov43_0222A9F4(PalPadGfx *, Window *, s32);
s32 ov43_0222AD00(PalPadGfx *, s32);
s32 ov43_0222AD74(PalPadGfx *, s32, s32);
s32 ov43_0222AD98(PalPadGfx *, u8, s32);
s32 ov43_0222C378(PalPadRoster *, PalPadShared *, PalPadGfx *, s16, s32, s32, enum HeapID);
void ov43_0222BB20(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222BB20(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID) {
    NNSG2dScreenData *screen;

    ov43_0222A9F4(param2, &param2->titleWindow, 5);
    screen = param2->screens.data[1];
    CopyToBgTilemapRect(param2->bgConfig, 0U, 0x11U, 0x14U, 0xEU, 4U, screen->rawData, 3U, 0U, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleWindowCopyToVram(&param0->returnWindow);
    ScheduleBgTilemapBufferTransfer(param2->bgConfig, 0U);
    ov43_0222AD00(param2, 1);
    ov43_0222C378(param0, param1, param2, param0->page, (s32)param0->cursor, 0, heapID);
    ov43_0222AD74(param2, 1, 1);
    ov43_0222AD98(param2, 1, 0x10);
}
