#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "yes_no_prompt.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

typedef u32 TextColor;

void ov73_021E63BC(Window *param0, String *param1, int param2);

void ov73_021E6400(UnkStruct_020961E8 *arg0);
s32 ov73_021E670C(void *, s32);
s32 ov73_021E756C(void *, s32, s32);
s32 ov73_021E6444(void *arg0, s32 arg1);
s32 ov73_021E6508();
s32 ov73_021E7340(void);
s32 ov73_021E7468(s32);
s32 ov73_021E6484(s32 arg0, s32 arg1);

void ov73_021E63BC(Window *param0, String *param1, int param2) {
    int v0 = FontID_String_GetWidth(FONT_MESSAGE, param1, 0);
    int v1 = (26 * 8 - v0) / 2;

    FillWindowPixelBuffer(param0, 0x0);
    AddTextPrinterParameterizedWithColor(param0, FONT_MESSAGE, param1, v1, 1, param2, ((TextColor)(((1 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
}

void ov73_021E6400(UnkStruct_020961E8 *arg0) {
    YesNoPrompt_Destroy(arg0->unk_384);
    RemoveWindow(arg0->unk_2FC);
    RemoveWindow(&arg0->unk_2FC[4]);
    RemoveWindow(&arg0->unk_2FC[1]);
    RemoveWindow(&arg0->unk_2FC[2]);
    RemoveWindow(&arg0->unk_2FC[3]);
}

s32 ov73_021E6444(void *arg0, s32 arg1) {
    sub_020398D4(0, 1);
    if (sub_0203769C() == 0) {
        if (sub_02037454() >= 2) {
            ov73_021E756C(arg0, 2, 0);
        }
    } else {
        ov73_021E756C(arg0, 8, 0);
    }
    ov73_021E670C(arg0, 3);
    return arg1;
}

s32 ov73_021E6484(s32 arg0, s32 arg1) {
    ov73_021E6508();
    if (sub_0203769C() == 0) {
        if (ov73_021E7340() != 1) {
            ov73_021E7468(arg0);
        }
    } else {
        ov73_021E7468(arg0);
    }
    return arg1;
}
