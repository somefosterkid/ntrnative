#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"
struct BattleCastleOpponentApp;

u8 ov83_02247768(u8 numSlots, u8 slot);

void ov83_02245300(struct BattleCastleOpponentApp *param_1, u8 slot);

void ov83_02245300(BattleCastleOpponentApp *param_1, u8 slot) {
    s32 iVar1;

    iVar1 = ov83_02247768(param_1->numSlots, slot);
    *(u8 *)(*((u8 * *)&param_1->movesUnlocked) + iVar1) = 1;
    return;
}
