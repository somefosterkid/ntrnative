#include "global.h"

#include "options.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02005D10.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_022448AC(u8 *, u32, u8);

void ov83_022448E4(u8 *, u8 *);

void ov83_022449D4(u8 *, u8 *);

void ov83_02244AB0(u8 *, u32, BoxPokemon *);

void ov83_02245D48(u8 *);

void ov83_02246114(u8 *, u32);

u8 ov83_02247768(u8, u32);

void ov83_02247944(void *, u32);

void ov83_02245838(u8 *param_1, u32 param_2, s32 param_3);

void ov83_02245838(u8 *param_1, u32 param_2, s32 param_3) {
    u8 uVar1;
    u32 uVar2;
    s32 iVar3;
    Pokemon *mon;
    BoxPokemon *pBVar4;
    u32 uStack_18;

    uVar2 = Options_GetFrame(((BattleCastleOpponentApp *)param_1)->options);
    ov83_02247944(param_1 + 0xc0, uVar2);
    iVar3 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
    mon = Party_GetMonByIndex(((BattleCastleOpponentApp *)param_1)->opponentsParty, iVar3);
    pBVar4 = Mon_GetBoxMon(mon);
    ov83_02244AB0(param_1, 0, pBVar4);
    if (param_3 == 1) {
        uVar1 = ov83_022448AC(param_1, 0x1f, 1);
        ((BattleCastleOpponentApp *)param_1)->printerID = uVar1;
        PlaySE(0x632);
    } else {
        uVar1 = ov83_022448AC(param_1, 0x20, 1);
        ((BattleCastleOpponentApp *)param_1)->printerID = uVar1;
        PlaySE(0x633);
    }
    iVar3 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
    if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->levelAdjustments + iVar3) == 0) {
        iVar3 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
        *(s8 *)(((BattleCastleOpponentApp *)param_1)->levelAdjustments + iVar3) = (s8)param_3;
    } else {
        iVar3 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
        *(u8 *)(((BattleCastleOpponentApp *)param_1)->levelAdjustments + iVar3) = 0;
    }
    iVar3 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
    if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->levelAdjustments + iVar3) == 0) {
        uVar2 = GetMonData(mon, 5, (void *)0x0);
        uStack_18 = GetMonExpBySpeciesAndLevel(uVar2, 0x32);
        SetMonData(mon, 8, &uStack_18);
        CalcMonLevelAndStats(mon);
    } else {
        iVar3 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, param_2);
        if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->levelAdjustments + iVar3) == 1) {
            uVar2 = GetMonData(mon, 5, (void *)0x0);
            uStack_18 = GetMonExpBySpeciesAndLevel(uVar2, 0x37);
            SetMonData(mon, 8, &uStack_18);
            CalcMonLevelAndStats(mon);
        } else {
            uVar2 = GetMonData(mon, 5, (void *)0x0);
            uStack_18 = GetMonExpBySpeciesAndLevel(uVar2, 0x2d);
            SetMonData(mon, 8, &uStack_18);
            CalcMonLevelAndStats(mon);
        }
    }
    ov83_022448E4(param_1, param_1 + 0x80);
    ov83_022449D4(param_1, param_1 + 0x70);
    if (((BattleCastleOpponentApp *)param_1)->selectedMonSlot == param_2) {
        ov83_02245D48(param_1);
        ov83_02246114(param_1, 0);
    }
    return;
}
