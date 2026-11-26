#include "global.h"

#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_022448E4(BattleCastleOpponentApp *arg0, Window *window);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02245748(BattleCastleOpponentApp *param_1, u8 param_2);
#else
void ov83_02245748(BattleCastleOpponentApp *param_1, u32 param_2);
#endif

void ov83_02245824(BattleCastleOpponentApp *arg0, s32 arg1);

void ov83_02245824(BattleCastleOpponentApp *arg0, s32 arg1) {
    ov83_02245748(arg0, arg1);
    ov83_022448E4(arg0, &arg0->windows[3]);
}
