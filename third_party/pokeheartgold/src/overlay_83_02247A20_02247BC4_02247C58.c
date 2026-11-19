#include "global.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02242AC0(BattleCastleSelfApp *, u32, u32, u32);

void ov83_02242AE0(BattleCastleSelfApp *, s32);

extern u8 ov83_02248570;

extern u8 ov83_02248610;

extern u8 ov83_02248611;

void ov83_02247C58(BattleCastleSelfApp *param_1, s32 param_2);

void ov83_02247C58(BattleCastleSelfApp *param_1, s32 param_2) {
    ov83_02242AC0(param_1, (&ov83_02248610)[param_2 * 8], (&ov83_02248611)[param_2 * 8], *(u32 *)(&ov83_02248570 + param_2 * 4));
    ov83_02242AE0(param_1, param_2);
    return;
}
