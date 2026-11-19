#include "global.h"

#include "frontier/overlay_80_022340E8.h"

#include "player_avatar.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov80_02237FA4(FrontierSave *, u8, u32);

u8 ov83_022448AC(void *, s32, u8);

s32 ov83_02245094(void *);

s32 ov83_02245288(void *, s32);

s32 ov83_02245318(void *, s32);

s32 ov83_022453C0(void *);

s32 ov83_022453DC(void *, void *);

s32 ov83_02245A40(void *arg0, u32 arg1, s32 arg2);

s32 ov83_02245A40(void *arg0, u32 arg1, s32 arg2) {
    u32 v0;

    v0 = sub_0205C1F0(((BattleCastleOpponentApp *)arg0)->challengeType);
    if ((u32)FrontierSave_GetStat(((FrontierSave *)((BattleCastleOpponentApp *)arg0)->frontier), (s32)v0, (s32)sub_0205C268(sub_0205C1F0(((BattleCastleOpponentApp *)arg0)->challengeType))) < arg1) {
        ov83_022453C0(arg0);
        *((s8 *)&((BattleCastleOpponentApp *)arg0)->printerID) = ov83_022448AC(arg0, arg2, 1);
        *((s8 *)&((BattleCastleOpponentApp *)arg0)->subState) = 0x10;
        return 0;
    }
    if (ov80_02237D8C(((BattleCastleOpponentApp *)arg0)->challengeType) == 0) {
        ov83_02245094(arg0 + 0xC0);
        ov80_02237FA4(((FrontierSave *)((BattleCastleOpponentApp *)arg0)->frontier), ((BattleCastleOpponentApp *)arg0)->challengeType, arg1);
        ov83_022453DC(arg0, arg0 + 0x50);
        if (arg1 == 2) {
            ov83_02245288(arg0, ((BattleCastleOpponentApp *)arg0)->selectedMonSlot);
        } else {
            ov83_02245318(arg0, ((BattleCastleOpponentApp *)arg0)->selectedMonSlot);
        }
        *((s8 *)&((BattleCastleOpponentApp *)arg0)->subState) = 0xD;
        return 0;
    }
    *((s8 *)&((BattleCastleOpponentApp *)arg0)->syncPurchaseAction) = 1;
    return 1;
}
