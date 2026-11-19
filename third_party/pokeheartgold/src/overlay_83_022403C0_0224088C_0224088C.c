#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_022479E4(Window *, MsgData *, s32, u32, u32, u32, u32, s32);

void ov83_0224088C(BattleCastleSelfApp *param_1);

void ov83_0224088C(BattleCastleSelfApp * param_1) {
    u8 bVar1;

    FillWindowPixelBuffer(&param_1->windows[57], 0);
    bVar1 = GetWindowWidth(&param_1->windows[57]);
    ov83_022479E4(&param_1->windows[57], param_1->msgLoader, 0x69, (bVar1 * 8) / 2, 0, 0, 0x10200, 2);
    ScheduleWindowCopyToVram(&param_1->windows[57]);
    return;
}
