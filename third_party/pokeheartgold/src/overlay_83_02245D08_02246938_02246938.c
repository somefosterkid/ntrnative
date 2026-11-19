#include "global.h"

#include "bg_window.h"
#include "battle_castle_app_sprite.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_0224755C(BattleCastleAppSprite *sprite, BOOL flag);

void ov83_02246938(u8 *param_1, s32 param_2);

void ov83_02246938(u8 *param_1, s32 param_2) {
    if (param_2 == 1) {
        u32 uVar1;
        Window *pWVar2;
        uVar1 = 0xc;
        pWVar2 = (Window *)(param_1 + 0x110);
        do {
            ScheduleWindowCopyToVram(pWVar2);
            uVar1 = uVar1 + 1;
            pWVar2 = pWVar2 + 1;
        } while (uVar1 <= (0x23 - 1));
        ov83_0224755C(((BattleCastleOpponentApp *)param_1)->selectedMonSprite, 1);
        return;
    }
    {
        Window *pWVar2;
        u32 uVar1;
        uVar1 = 0xc;
        pWVar2 = (Window *)(param_1 + 0x110);
        do {
            ClearWindowTilemapAndScheduleTransfer(pWVar2);
            uVar1 = uVar1 + 1;
            pWVar2 = pWVar2 + 1;
        } while (uVar1 <= (0x23 - 1));
        ov83_0224755C(((BattleCastleOpponentApp *)param_1)->selectedMonSprite, 0);
    }
}
