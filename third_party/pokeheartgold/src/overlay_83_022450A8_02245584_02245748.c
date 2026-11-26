#include "global.h"

#include "options.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02005D10.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_022448AC(BattleCastleOpponentApp *param_1, u32, u8);

void ov83_022449D4(BattleCastleOpponentApp *arg0, Window *window);

void ov83_02244AB0(BattleCastleOpponentApp *app, u32, BoxPokemon *boxmon);

void ov83_02245D48(BattleCastleOpponentApp *arg0);

void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);

void ov83_02246988(BattleCastleOpponentApp *param_1);

void ov83_0224755C(struct BattleCastleAppSprite *sprite, BOOL flag);

u8 ov83_02247768(u8 numSlots, u8 slot);

void ov83_02247944(Window *window, u32);

void ov83_02245748(BattleCastleOpponentApp *param_1, u8 param_2);

void ov83_02245748(BattleCastleOpponentApp *param_1, u8 param_2) {
    u8 uVar1;
    s32 iVar2;
    Pokemon *mon;
    u32 uVar3;
    BoxPokemon *pBVar4;

    iVar2 = ov83_02247768(param_1->numSlots, param_2);
    mon = Party_GetMonByIndex(param_1->opponentsParty, iVar2);
    uVar3 = Options_GetFrame(param_1->options);
    ov83_02247944(&param_1->windows[7], uVar3);
    pBVar4 = Mon_GetBoxMon(mon);
    ov83_02244AB0(param_1, 0, pBVar4);
    uVar1 = ov83_022448AC(param_1, 0x14, 1);
    param_1->printerID = uVar1;
    iVar2 = ov83_02247768(param_1->numSlots, param_2);
    param_1->identityUnlocked[iVar2] = 1;
    iVar2 = ov83_02247768(param_1->numSlots, param_2);
    ov83_0224755C(param_1->ballSprites[iVar2], FALSE);
    iVar2 = ov83_02247768(param_1->numSlots, param_2);
    ov83_0224755C(param_1->monSprites[iVar2], TRUE);
    ov83_022449D4(param_1, &param_1->windows[2]);
    if (param_1->selectedMonSlot == param_2) {
        ov83_02245D48(param_1);
        ov83_02246114(param_1, 0);
        ov83_02246988(param_1);
    }
    iVar2 = ov83_02247768(param_1->numSlots, param_2);
    ov83_0224755C(param_1->barSprites[iVar2], TRUE);
    PlaySE(0x623);
    return;
}
