#include "global.h"
struct BattleCastleOpponentApp;

#include "frontier/overlay_80_022340E8.h"

#include "unk_02005D10.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02244CD4(BattleCastleOpponentApp *arg0, s32 *arg1, s32 arg2);

void ov83_02244CD4(BattleCastleOpponentApp *arg0, s32 *arg1, s32 arg2) {
    arg0->subState = 0;
    *arg1 = arg2;
}

s32 ov83_02244D0C(BattleCastleOpponentApp *arg0, u32, s32);

s32 ov83_022450A8(void *, u16, u16);

void ov83_02244CDC(struct BattleCastleOpponentApp *arg0);

void ov83_02244CDC(BattleCastleOpponentApp *arg0) {
    PlaySE(0x5DCU);
    if (ov80_02237D8C(arg0->challengeType) == 1) {
        ov83_022450A8(arg0, 0x16, arg0->selectedMonSlot);
    }
    ov83_02244D0C(arg0, arg0->selectedMonSlot, 0);
}
