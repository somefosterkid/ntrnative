#include "global.h"
struct BattleCastleSelfApp;
#include "unk_02035900.h"
#include "use_item_on_mon.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02241504(struct BattleCastleSelfApp *arg0);

void ov83_02241510(u32 param_1, u32 param_2, u16 *param_3, BattleCastleSelfApp *param_4);

void ov83_0224152C(Pokemon *arg0, u16 arg1);

void ov83_02241504(BattleCastleSelfApp *arg0) {
    *((s16 *)&arg0->commPayload[0]) = 1;
}

void ov83_02241510(u32 param_1, u32 param_2, u16 *param_3, BattleCastleSelfApp *param_4) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if (param_1 != uVar1) {
        param_4->partnerIsExiting = (s8)*param_3;
    }
    return;
}

void ov83_0224152C(Pokemon *mon, u16 itemID) {
    UseItemOnPokemon(mon, itemID, 0U, 0U, HEAP_ID_107);
}
