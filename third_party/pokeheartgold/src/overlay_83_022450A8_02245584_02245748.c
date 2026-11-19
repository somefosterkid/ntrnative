#include "global.h"

#include "options.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02005D10.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_022448AC(u8 *, u32, u8);

void ov83_022449D4(u8 *, u8 *);

void ov83_02244AB0(u8 *, u32, BoxPokemon *);

void ov83_02245D48(u8 *);

void ov83_02246114(u8 *, u32);

void ov83_02246988(u8 *);

void ov83_0224755C(void *, u32);

u8 ov83_02247768();

void ov83_02247944(void *, u32);

void ov83_02245748(u8 *param_1, u32 param_2);

void ov83_02245748(u8 *param_1, u32 param_2) {
    u8 uVar1;
    s32 iVar2;
    Pokemon *mon;
    u32 uVar3;
    BoxPokemon *pBVar4;

    iVar2 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots);
    mon = Party_GetMonByIndex(((BattleCastleOpponentApp *)param_1)->opponentsParty, iVar2);
    uVar3 = Options_GetFrame(((BattleCastleOpponentApp *)param_1)->options);
    ov83_02247944(param_1 + 0xc0, uVar3);
    pBVar4 = Mon_GetBoxMon(mon);
    ov83_02244AB0(param_1, 0, pBVar4);
    uVar1 = ov83_022448AC(param_1, 0x14, 1);
    ((BattleCastleOpponentApp *)param_1)->printerID = uVar1;
    iVar2 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
    *(u8 *)(((BattleCastleOpponentApp *)param_1)->identityUnlocked + iVar2) = 1;
    iVar2 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
    ov83_0224755C(*(void **)(param_1 + iVar2 * 4 + 0x4f4), 0);
    iVar2 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
    ov83_0224755C(*(void **)(param_1 + iVar2 * 4 + 0x4e4), 1);
    ov83_022449D4(param_1, param_1 + 0x70);
    if (((BattleCastleOpponentApp *)param_1)->selectedMonSlot == param_2) {
        ov83_02245D48(param_1);
        ov83_02246114(param_1, 0);
        ov83_02246988(param_1);
    }
    iVar2 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
    ov83_0224755C(*(void **)(param_1 + iVar2 * 4 + 0x510), 1);
    PlaySE(0x623);
    return;
}
