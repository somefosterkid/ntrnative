#include "global.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02242F18(BattleCastleSelfApp *arg0, s16 arg1);

void ov83_02242F18(BattleCastleSelfApp *arg0, s16 arg1) {
    arg0->highlightIdx = arg1;
    arg0->highlightState = 0;
    arg0->highlightTimer = 0;
}
