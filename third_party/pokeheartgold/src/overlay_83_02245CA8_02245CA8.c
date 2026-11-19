#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_0224755C(void *, s32);

void ov83_02245CA8(BattleCastleOpponentApp *arg0, s32 arg1);

void ov83_02245CA8(BattleCastleOpponentApp *arg0, s32 arg1) {
    if ((*((u8 *)((arg0->statsUnlocked) + arg1))) == 1) {
        ov83_0224755C(arg0->flagSprites[arg1][0], 1);
    }
    if ((*((u8 *)((arg0->movesUnlocked) + arg1))) == 1) {
        ov83_0224755C(arg0->flagSprites[arg1][1], 1);
    }
}
