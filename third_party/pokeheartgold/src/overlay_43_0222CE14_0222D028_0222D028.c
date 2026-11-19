#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "overlay_43_palpad.h"
#include "player_data.h"

void ov43_0222A9F4(PalPadGfx *gfx, Window *window, u32 msgId);

void ov43_0222D028(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);

void ov43_0222D028(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx) {
    BufferPlayersName(gfx->msgFormat, 0, Save_PlayerData_GetProfile(shared->saveData));
    ov43_0222A9F4(gfx, &gfx->titleWindow, 0x32);
    LoadRectToBgTilemapRect(gfx->bgConfig, 2, friendCode->screen->rawData, 0, 0, friendCode->screen->screenWidth / 8, friendCode->screen->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2);
    ScheduleWindowCopyToVram(&friendCode->windows[0]);
    ScheduleWindowCopyToVram(&friendCode->windows[1]);
    ScheduleWindowCopyToVram(&friendCode->windows[2]);
}
