#include "global.h"
#include "frontier/overlay_80_022340E8.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02240F7C(BattleCastleSelfApp *arg0, s16 *arg1, s16 *arg2, s16 *arg3, s16 *arg4);

void ov83_02240F7C(BattleCastleSelfApp *arg0, s16 *arg1, s16 *arg2, s16 *arg3, s16 *arg4) {
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        *arg1 = 0x28;
        *arg2 = 0;
        *arg3 = 0;
        *arg4 = 0;
        return;
    }
    *arg1 = 0;
    *arg2 = 0;
    *arg3 = 0x80;
    *arg4 = 0;
}
