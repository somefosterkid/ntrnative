#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "overlay_59_juice_feed_app.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"

extern const u32 ov59_0223C954[];

void ov59_0223AEB0(Ov59JuiceFeedApp *param_1, s32 param_2) {
    s32 iVar2;
    const u32 *puVar1;
    u32 y;

    switch (param_2) {
    case 3:
        ClearWindowTilemapAndScheduleTransfer(&param_1->windows[1]);
        Sprite_SetDrawFlag(param_1->sprites[0], 0);
        Sprite_SetDrawFlag(param_1->sprites[1], 0);
        return;
    case 2:
        FillWindowPixelBuffer(&param_1->windows[1], 0);
        puVar1 = ov59_0223C954;
        iVar2 = 0;
        y = 4;
        for (; iVar2 < 2; iVar2++) {
            ReadMsgDataIntoString(param_1->msgData, iVar2 + 1, param_1->unk_68);
            AddTextPrinterParameterizedWithColor(&param_1->windows[1], (enum Font)4, param_1->unk_68, 0, y, 0xff, *puVar1, (PrinterCallback_t)0x0);
            Sprite_SetDrawFlag(param_1->sprites[iVar2], 1);
            puVar1 = puVar1 + 1;
            y = y + 0x18;
        }
        ScheduleWindowCopyToVram(&param_1->windows[1]);
        return;
    case 0:
    case 1:
        Sprite_SetAnimCtrlSeq(param_1->sprites[param_2], param_2 * 2 + 1);
        Sprite_ResetAnimCtrlState(param_1->sprites[param_2]);
        break;
    }
    return;
}
