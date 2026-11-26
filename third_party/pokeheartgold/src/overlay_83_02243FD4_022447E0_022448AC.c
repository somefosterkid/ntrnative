#include "global.h"

#include "bg_window.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_022447E0(BattleCastleOpponentApp *app, Window *window, s32 param_3, u32 param_4, u32 param_5, u32 param_6, u8 param_7, u8 param_8, u8 param_9, u8 param_10);

u8 ov83_022448AC(BattleCastleOpponentApp *param_1, s32 param_2, u8 param_3);

u8 ov83_022448AC(BattleCastleOpponentApp *param_1, s32 param_2, u8 param_3) {
    u8 uVar1;

    uVar1 = ov83_022447E0(param_1, &param_1->windows[7], param_2, 1, 1, 0xff, 1, 2, 0xf, param_3);
    ScheduleWindowCopyToVram(&param_1->windows[7]);
    return uVar1;
}
