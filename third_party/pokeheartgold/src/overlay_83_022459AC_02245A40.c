#include "global.h"

#include "frontier/overlay_80_022340E8.h"

#include "player_avatar.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov80_02237FA4(FrontierSave *, u8, u32);

u8 ov83_022448AC(BattleCastleOpponentApp *param_1, s32, u8);

s32 ov83_02245094(Window *window);

s32 ov83_02245288(BattleCastleOpponentApp *arg0, s32);

s32 ov83_02245318(BattleCastleOpponentApp *arg0, s32);

s32 ov83_022453C0(BattleCastleOpponentApp *arg0);

s32 ov83_022453DC(BattleCastleOpponentApp *arg0, Window *arg1);

s32 ov83_02245A40(BattleCastleOpponentApp *arg0, u32 arg1, s32 arg2);

s32 ov83_02245A40(BattleCastleOpponentApp *arg0, u32 arg1, s32 arg2) {
    u32 v0;

    v0 = sub_0205C1F0(arg0->challengeType);
    if ((u32)FrontierSave_GetStat(((FrontierSave *)arg0->frontier), (s32)v0, (s32)sub_0205C268(sub_0205C1F0(arg0->challengeType))) < arg1) {
        ov83_022453C0(arg0);
        *((s8 *)&arg0->printerID) = ov83_022448AC(arg0, arg2, 1);
        *((s8 *)&arg0->subState) = 0x10;
        return 0;
    }
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        ov83_02245094(&arg0->windows[7]);
        ov80_02237FA4(((FrontierSave *)arg0->frontier), arg0->challengeType, arg1);
        ov83_022453DC(arg0, &arg0->windows[0]);
        if (arg1 == 2) {
            ov83_02245288(arg0, arg0->selectedMonSlot);
        } else {
            ov83_02245318(arg0, arg0->selectedMonSlot);
        }
        *((s8 *)&arg0->subState) = 0xD;
        return 0;
    }
    *((s8 *)&arg0->syncPurchaseAction) = 1;
    return 1;
}
