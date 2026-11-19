#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"
struct BattleCastleOpponentApp;

u8 ov83_02247768(u8);

void ov83_02245300(struct BattleCastleOpponentApp *param_1);

void ov83_02245300(BattleCastleOpponentApp *param_1) {
    s32 iVar1;

    iVar1 = ov83_02247768(param_1->numSlots);
    *(u8 *)(*((u8 * *)&param_1->movesUnlocked) + iVar1) = 1;
    return;
}
