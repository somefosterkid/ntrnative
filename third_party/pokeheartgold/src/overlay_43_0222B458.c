#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "text.h"
#include "overlay_43_palpad.h"

void ov43_0222AA70(PalPadGfx *gfx);
void ov43_0222AD74(PalPadGfx *gfx, u32, u32);
void ov43_0222B458(PalPadMenu *menu, PalPadGfx *gfx);
void ov43_0222B4BC(PalPadMenu *menu, PalPadGfx *gfx, s32 strno, enum HeapID heapID);
s32 ov43_0222B534(PalPadMenu *menu, PalPadShared *shared);
void ov43_0222B55C(PalPadMenu *menu);

void ov43_0222B458(PalPadMenu *menu, PalPadGfx *gfx) {
    ov43_0222AA70(gfx);
    FillBgTilemapRect(gfx->bgConfig, 2, 0, 0, 0, 0x20, 0x18, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2);
    FillBgTilemapRect(gfx->bgConfig, 3, 0, 0, 0, 0x20, 0x18, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 3);
    ClearWindowTilemapAndScheduleTransfer(&menu->menuWindow);
    ov43_0222AD74(gfx, 0, 0xff);
    return;
}

void ov43_0222B4BC(PalPadMenu *menu, PalPadGfx *gfx, s32 strno, enum HeapID heapID) {
    String *string;

    string = String_New(0x80U, heapID);
    FillWindowPixelBuffer(&menu->msgWindow, 0xFU);
    ReadMsgDataIntoString(gfx->msgData, strno, string);
    StringExpandPlaceholders(gfx->msgFormat, menu->msgText, string);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    menu->printerId = AddTextPrinterParameterizedWithColor(&menu->msgWindow, FONT_MESSAGE, menu->msgText, 0U, 0U, menu->textSpeed, 0x10200U, NULL);
    DrawFrameAndWindow2(&menu->msgWindow, 1, 0x46U, 0xCU);
    ScheduleWindowCopyToVram(&menu->msgWindow);
    String_Delete(string);
}

s32 ov43_0222B534(PalPadMenu *menu, PalPadShared *shared) {
    if (TextPrinterCheckActive((u8)menu->printerId) == 0) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
        shared->inputState = TextFlags_GetIsTouchSpeedingUpPrint();
        return 1;
    }
    return 0;
}

void ov43_0222B55C(PalPadMenu *menu) {
    ClearFrameAndWindow2(&menu->msgWindow, 1);
    ClearWindowTilemapAndScheduleTransfer(&menu->msgWindow);
    return;
}
