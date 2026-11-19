#include "global.h"
#include "battle_castle_app_sprite.h"
#include "overlay_83_battle_castle_opponent_app.h"
struct BattleCastleOpponentApp;

void ov83_02247668(BattleCastleAppSprite *sprite, struct BoxPokemon *, int, u32);

u8 ov83_02247768(u8, u8, s32);

void ov83_02246988(struct BattleCastleOpponentApp *param_1, s32 arg1, s32 arg2);

s32 ov83_02244CDC();
void ov83_022469D8(struct BattleCastleOpponentApp *arg0, s8 arg1, s8 arg2);

void ov83_02246988(BattleCastleOpponentApp *param_1, s32 arg1, s32 arg2) {
    s32 iVar1;

    iVar1 = ov83_02247768(param_1->numSlots, param_1->selectedMonSlot, arg2);
    if (*(u8 *)(param_1->identityUnlocked + iVar1) == 0) {
        ov83_02247668(param_1->selectedMonSprite, param_1->boxMon, 0, param_1->personality);
        return;
    }
    ov83_02247668(param_1->selectedMonSprite, param_1->boxMon, param_1->species, param_1->personality);
    return;
}

void ov83_022469D8(BattleCastleOpponentApp *arg0, s8 arg1, s8 arg2) {
    *((s8 *)&arg0->selectedMonSlot) = arg1;
    *((s8 *)&arg0->previousSlot) = arg2;
    ov83_02244CDC();
}
