#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"
struct BattleCastleOpponentApp;

u8 ov83_022448AC(void *, s32, u8);

s32 ov83_02244A74(void *);

s32 ov83_02244A98(void *, s32, s32, s32, s32);

u16 ov83_02245068(s8);

void ov83_02245554(struct BattleCastleOpponentApp *arg0, s8 arg1);

void ov83_02245554(BattleCastleOpponentApp *arg0, s8 arg1) {
    ov83_02244A98(arg0, 0, ov83_02245068(arg1), 4, 0);
    *((s8 *)&arg0->printerID) = ov83_022448AC(arg0, 0x19, 1);
    ov83_02244A74(arg0);
    *((s8 *)&arg0->levelAdjustment) = arg1;
}
