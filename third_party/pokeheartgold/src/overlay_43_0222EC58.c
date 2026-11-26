#include "global.h"

#include "constants/heap.h"

#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_43_palpad_fns.h"
#include "pm_string.h"
#include "text.h"

void ov43_0222EC58(PalPadDetailsSlot *slot, u32 windowIndex, u32 windowOffset, PalPadShared *shared, PalPadGfx *gfx, u32 entryID, u32 xOffset, u32 yOffset, u32 color, String *string, String *fmtString, u32 align);
u8 ov43_0222ECD4(Window *window, PalPadShared *unused, PalPadGfx *gfx, s32 entryID, u32 x, u32 y, u32 color);

void ov43_0222EC58(PalPadDetailsSlot *slot, u32 windowIndex, u32 windowOffset, PalPadShared *shared, PalPadGfx *gfx, u32 entryID, u32 xOffset, u32 yOffset, u32 color, String *string, String *fmtString, u32 align) {
    u32 uVar1;

    ReadMsgDataIntoString(gfx->msgData, entryID, fmtString);
    StringExpandPlaceholders(gfx->msgFormat, string, fmtString);
    switch (align) {
    case 1:
        uVar1 = FontID_String_GetWidth(FONT_MESSAGE, string, 0);
        xOffset = xOffset - (uVar1 + 1 >> 1);
        break;
    case 2:
        uVar1 = FontID_String_GetWidth(FONT_MESSAGE, string, 0);
        xOffset = xOffset - uVar1;
        break;
    }
    AddTextPrinterParameterizedWithColor(slot->windows[windowIndex] + windowOffset, FONT_MESSAGE, string, xOffset, yOffset, 0xff, color, NULL);
    return;
}

u8 ov43_0222ECD4(Window *window, PalPadShared *unused, PalPadGfx *gfx, s32 entryID, u32 x, u32 y, u32 color) {
    u8 uVar1;

    ReadMsgDataIntoString(gfx->msgData, entryID, gfx->pageBuffer);
    StringExpandPlaceholders(gfx->msgFormat, gfx->pageText, gfx->pageBuffer);
    uVar1 = AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, gfx->pageText, x, y, 0xff, color, NULL);
    return uVar1;
}

void ov43_0222ED18(PalPadDetails *details, s32 delta) {
    s16 v0;
    s32 v1;

    if (details->hasFrontierPages == 1) {
        v1 = 8;
    } else {
        v1 = 3;
    }
    if (delta > 0) {
        details->prevPage = details->page;
        details->page = (s16)((s32)(details->page + delta) % v1);
        return;
    }
    if (delta < 0) {
        details->prevPage = details->page;
        details->page = (s16)(details->page + delta);
        v0 = details->page;
        if ((s32)v0 < 0) {
            details->page = (s16)(v0 + v1);
        }
    }
}
