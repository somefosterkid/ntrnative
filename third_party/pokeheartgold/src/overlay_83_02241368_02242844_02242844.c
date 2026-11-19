#include "global.h"

#include "bg_window.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_0224755C(void *, u32);

void ov83_02242844(BattleCastleSelfApp *param_1, s32 param_2);

void ov83_02242844(BattleCastleSelfApp *param_1, s32 param_2) {
    if (param_2 == 1) {
        u32 uVar1 = 0x12;
        Window *pWVar2 = &param_1->windows[18];
        do {
            ScheduleWindowCopyToVram(pWVar2);
            uVar1 = uVar1 + 1;
            pWVar2 = pWVar2 + 1;
        } while (uVar1 <= (0x30 - 1));
        ov83_0224755C(param_1->selectedMonSprite, 1);
        return;
    }
    {
        Window *pWVar2;
        u32 uVar1;
        uVar1 = 0x12;
        pWVar2 = &param_1->windows[18];
        do {
            ClearWindowTilemapAndScheduleTransfer(pWVar2);
            uVar1 = uVar1 + 1;
            pWVar2 = pWVar2 + 1;
        } while (uVar1 <= (0x30 - 1));
    }
    ov83_0224755C(param_1->selectedMonSprite, 0);
    return;
}
