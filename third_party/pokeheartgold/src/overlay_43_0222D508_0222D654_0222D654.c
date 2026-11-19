#include "global.h"

#include "bg_window.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_43_palpad.h"

void ov43_0222DB94(PalPadDetails *, PalPadGfx *, s16);

void ov43_0222A9F4(PalPadGfx *, Window *, u32);

void ov43_0222AB20(PalPadGfx *, SaveData *, s32, enum HeapID);

void ov43_0222AD00(PalPadGfx *, s32);

void ov43_0222AD40(PalPadGfx *, u32, u32);

void ov43_0222AD98(PalPadGfx *, u8, u32);

void ov43_0222D8B8(PalPadDetails *, PalPadShared *, PalPadGfx *, enum HeapID);

void ov43_0222DAE8(PalPadDetails *, PalPadShared *, PalPadGfx *, u32, s32, enum HeapID);

void ov43_0222D654(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222D654(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    NNSG2dScreenData *screen;

    ov43_0222AB20(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend], heapID);
    ov43_0222A9F4(gfx, &details->profileWindows[1], 0xd);
    screen = details->screens[3].data;
    LoadRectToBgTilemapRect(gfx->bgConfig, 5, screen->rawData, 0, 0, screen->screenWidth / 8, screen->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 5);
    screen = details->screens[0].data;
    LoadRectToBgTilemapRect(gfx->bgConfig, 0, screen->rawData, 0, 0, screen->screenWidth / 8, screen->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 0);
    screen = details->screens[1].data;
    LoadRectToBgTilemapRect(gfx->bgConfig, 2, screen->rawData, 0, 0, screen->screenWidth / 8, screen->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2);
    ov43_0222D8B8(details, shared, gfx, heapID);
    ov43_0222AD98(gfx, 0, 0);
    ov43_0222DAE8(details, shared, gfx, 0, details->page, heapID);
    ov43_0222DB94(details, gfx, 0xff);
    Sprite_SetPositionXY(gfx->arrows[0], 0xf4, 0x18);
    ov43_0222AD40(gfx, 2, 0);
    Sprite_SetPositionXY(gfx->arrows[1], 0xf4, 0x88);
    ov43_0222AD40(gfx, 3, 0);
    ov43_0222AD00(gfx, 1);
    SetBgPriority(2, 0);
}
