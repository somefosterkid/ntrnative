#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02245D48(BattleCastleOpponentApp *arg0);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);
#else
void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02246988(BattleCastleOpponentApp *param_1);
#else
void ov83_02246988(BattleCastleOpponentApp *param_1);
#endif

void ov83_0224691C(void *arg0, s32 arg1);

void ov83_0224691C(void *arg0, s32 arg1) {
    ov83_02245D48(arg0);
    ov83_02246114(arg0, arg1);
    ov83_02246988(arg0);
}
