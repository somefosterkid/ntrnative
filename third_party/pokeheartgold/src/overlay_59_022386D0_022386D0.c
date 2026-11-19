#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "font.h"
#include "msgdata.h"
#include "text.h"

s32 ov59_022387D0(Ov59ApricornApp *, s32, s32, s32);

s32 ov59_022388C4(Ov59ApricornApp *);

s32 ov59_0223892C(Ov59ApricornApp *, s32);

s32 ov59_02238C24(Ov59ApricornApp *);

s32 ov59_02238C40(Ov59ApricornApp *, s32);

void ov59_022386D0(Ov59ApricornApp *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov59_022386D0(Ov59ApricornApp *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 width;
    u8 temp_r0;
    u32 var_r5;
    ov59_022387D0(arg0, 0, arg2, arg3);
    ov59_022388C4(arg0);
    ReadMsgDataIntoString(arg0->msgData, 0, arg0->unk_64);
    width = FontID_String_GetWidth(FONT_SYSTEM, arg0->unk_64, 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[3], FONT_SYSTEM, arg0->unk_64, (0x50 - width) / 2, 0U, 0U, 0xF0200U, 0L);
    var_r5 = 0;
    do {
        ov59_0223892C(arg0, (u8)var_r5);
        var_r5 += 1;
    } while (((s32)var_r5) < 7);
    temp_r0 = arg0->mode;
    switch (temp_r0) {
    case 1:
        ov59_02238C24(arg0);
        return;

    case 2:
        ov59_02238C40(arg0, 0xA);
        return;

    default:

    case 0:
        ov59_02238C40(arg0, 0);
        return;
    }
}
