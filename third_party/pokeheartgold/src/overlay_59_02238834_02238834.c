#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"

extern u32 _0223CB80;

void ov59_02238834(Ov59ApricornApp *param_1, s32 param_2, s32 param_3);

void ov59_02238834(Ov59ApricornApp *param_1, s32 param_2, s32 param_3) {
    s32 iVar1;
    u32 uVar2;

    FillWindowPixelBuffer(&param_1->windows[param_2 + 6], 0);
    ReadMsgDataIntoString(param_1->msgData, param_2 + 5, param_1->unk_68);
    uVar2 = FontID_String_GetWidth(FONT_UNK_4, param_1->unk_68, 0);
    AddTextPrinterParameterizedWithColor(&param_1->windows[param_2 + 6], FONT_UNK_4, param_1->unk_68, ((s32)(0x30 - uVar2)) / 2, 0, 0xff, *((u32 *)((&_0223CB80) + param_2)), (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&param_1->windows[param_2 + 6]);
    iVar1 = param_2 + 9;
    uVar2 = 0x1c;
    Sprite_SetAnimCtrlSeq(param_1->sprites[iVar1], param_3 + ((param_2 * 2) + uVar2));
    Sprite_ResetAnimCtrlState(param_1->sprites[iVar1]);
}
