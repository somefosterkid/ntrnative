#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

u8 ov112_021EA010(Ov112PokewalkerApp *, s32, String *, s32, s32);

void ov112_021EA044(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov112_021EA044(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno, s32 arg3) {
    String *string;

    string = NewString_ReadMsgData(arg0->msgData, strno);
    StringExpandPlaceholders(arg0->msgFormat, arg0->unk_1E454, string);
    ov112_021EA010(arg0, arg1, arg0->unk_1E454, 0, arg3);
    String_Delete(string);
}

u8 ov112_021EA08C(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2);

u8 ov112_021EA08C(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno) {
    String *string;
    u8 v0;

    string = NewString_ReadMsgData(arg0->msgData, strno);
    DrawFrameAndWindow2(&arg0->windows[arg1], 1, 0x3E2U, 0xDU);
    FillWindowPixelBuffer(&arg0->windows[arg1], 0xFFU);
    StringExpandPlaceholders(arg0->msgFormat, arg0->unk_1E454, string);
    v0 = AddTextPrinterParameterized(&arg0->windows[arg1], FONT_MESSAGE, arg0->unk_1E454, 0U, 0U, (u32)Options_GetTextFrameDelay(arg0->options), NULL);
    String_Delete(string);
    return v0;
}

void ov112_021EA10C(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2);

void ov112_021EA10C(Ov112PokewalkerApp *arg0, s32 arg1, s32 strno) {
    String *string;

    string = NewString_ReadMsgData(arg0->msgData, strno);
    DrawFrameAndWindow2(&arg0->windows[arg1], 1, 0x3E2U, 0xDU);
    FillWindowPixelBuffer(&arg0->windows[arg1], 0xFFU);
    StringExpandPlaceholders(arg0->msgFormat, arg0->unk_1E454, string);
    AddTextPrinterParameterized(&arg0->windows[arg1], FONT_MESSAGE, arg0->unk_1E454, 0U, 0U, 0U, NULL);
    String_Delete(string);
}

void ov112_021EA17C(Ov112PokewalkerApp *arg0);

void ov112_021EA17C(Ov112PokewalkerApp *arg0) {
    arg0->waitingIcon = WaitingIcon_New(&arg0->windows[2], 0x3E2U);
}
