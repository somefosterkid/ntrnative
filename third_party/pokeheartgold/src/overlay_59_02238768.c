#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "unk_02031B0C.h"
#include "bg_window.h"
#include "font.h"
#include "sprite.h"
#include "text.h"

u8 sub_02031C08(SaveApricornBox *);

s32 sub_02031CA0(SaveApricornBox *);

u8 sub_02031CE8(SaveApricornBox *);

s32 sub_02031DA0(void *, void *);

u32 sub_02031FE8(void *);

void ov59_02238768(Ov59ApricornApp *arg0);

void ov59_02238768(Ov59ApricornApp *arg0) {
    s32 temp_r0;
    u32 var_r4;

    arg0->unk_18 = sub_02031C08(arg0->apricornBox);
    arg0->unk_14 = sub_02031CE8(arg0->apricornBox);
    sub_02031CEC(arg0->apricornBox, 8U, &arg0->juice);
    arg0->unk_16 = (u8) arg0->juice.mildness;
    arg0->unk_17 = sub_02031FE8(&arg0->juice);
    arg0->unk_15 = (s8) sub_02031D80(&arg0->juice);
    sub_02031DA0(&arg0->juice, &arg0->ranks[0]);
    arg0->unk_19 = sub_02031CA0(arg0->apricornBox);
    var_r4 = 0;
    do {
        temp_r0 = ApricornBox_CountApricorn(arg0->apricornBox, var_r4);
        arg0->apricornCounts[var_r4] = temp_r0;
        var_r4 += 1;
    } while ((s32) var_r4 < 7);
}

void ov59_022387D0(Ov59ApricornApp *arg0, s32 arg1);

void ov59_022387D0(Ov59ApricornApp *arg0, s32 arg1) {
    Sprite_SetAnimCtrlSeq(arg0->sprites[0], arg1);
    Sprite_ResetAnimCtrlState(arg0->sprites[0]);
    FillWindowPixelBuffer(&arg0->windows[2], 0U);
    AddTextPrinterParameterizedWithColor(&arg0->windows[2], FONT_UNK_4, arg0->unk_6C, (u32) (0x30 - FontID_String_GetWidth(FONT_UNK_4, arg0->unk_6C, 0U)) >> 1, 0U, 0xFFU, 0x10200U, NULL);
    ScheduleWindowCopyToVram(&arg0->windows[2]);
}
