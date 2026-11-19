#include "global.h"

#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite_system.h"
#include "overlay_113.h"

s32 ov113_021E6238(void *, void *, u16);

s32 ov113_021E629C(Window *, String *, s32, s32);

void ov113_021E5FC0(UnownReportData *arg0);

void ov113_021E5FC0(UnownReportData *arg0) {
    s32 v0;

    ov113_021E6238(arg0, &arg0->screens[3], 6);
    ov113_021E6238(arg0, arg0->screens, 2);
    v0 = 0;
    do {
        ManagedSprite_SetDrawFlag(arg0->monIcons[v0], 0);
        v0 += 1;
    } while (v0 < 0xE);
    ReadMsgDataIntoString(arg0->msgData, 1, arg0->strBuf2);
    ov113_021E629C(&arg0->windows[1], arg0->strBuf2, 0U, 0x38);
    ov113_021E629C(&arg0->windows[1], arg0->playerName, (u16)(0xB0 - FontID_String_GetWidth(FONT_SYSTEM, arg0->playerName, 0U)), 0x48);
    ReadMsgDataIntoString(arg0->msgData, 3, arg0->strBuf2);
    ov113_021E629C(&arg0->windows[2], arg0->strBuf2, 4U, 0x3C);
    ReadMsgDataIntoString(arg0->msgData, 4, arg0->strBuf1);
    BufferIntegerAsString(arg0->msgFormat, 0U, (s32) * &arg0->numFormsSeen, 2U, PRINTING_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(arg0->msgFormat, arg0->strBuf2, arg0->strBuf1);
    ov113_021E629C(&arg0->windows[2], arg0->strBuf2, 4U, 0x4C);
}
