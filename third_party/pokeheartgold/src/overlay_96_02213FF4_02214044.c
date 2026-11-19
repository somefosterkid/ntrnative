#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"

#include "overlay_96_app_02210C98.h"

void ov96_02214044(Ov96App_02210C98 *param_1, u8 param_2, s32 param_3);

void ov96_02214044(Ov96App_02210C98 *param_1, u8 param_2, s32 param_3) {
    u32 color;
    String *string;
    Window *iVar1;
    s32 iVar2;
    iVar1 = &param_1->unk_8[1];
    iVar2 = param_2;
    FillWindowPixelBuffer(&iVar1[iVar2], 0);
    if (param_3 != 0) {
        if (param_3 == 2) {
            string = param_1->unk_7B8[0];
        } else if (param_3 == 1) {
            string = param_1->unk_7B8[1];
        } else if (param_3 == 3) {
            string = param_1->unk_7B8[2];
        } else {
            GF_AssertFail();
            string = param_1->unk_7B8[0];
        }
        AddTextPrinterParameterizedWithColor(&iVar1[iVar2], FONT_UNK_4, string, 0, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    } else {
        if (!(param_1->unk_818[param_2])) {
            color = 0x10200;
        } else {
            color = 0x30400;
        }
        AddTextPrinterParameterizedWithColor(&iVar1[iVar2], FONT_UNK_4, param_1->unk_7C4[param_2], 0, 0, 0xff, color, (PrinterCallback_t)0x0);
    }
    CopyWindowToVram(&iVar1[iVar2]);
    return;
}
