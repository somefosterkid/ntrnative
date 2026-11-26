#include "global.h"
struct FieldFrontierDTO;

#include "battle/battle_setup.h"

#include "heap.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_80_field_frontier_dto.h"

s32 ov80_02236A34(BattleSetup *, void *, int, s32, enum HeapID);

u32 ov80_02236A88(u8);

BattleSetup *ov80_0223690C(void *arg0, struct FieldFrontierDTO *arg1);

BattleSetup *ov80_0223690C(void *arg0, FieldFrontierDTO *arg1) {
    u32 spC;
    Party *sp8;
    BattleSetup *temp_r7;
    BattleSetup *var_r1;
    Pokemon *temp_r4;
    s32 var_r2;
    s32 var_r6;
    u8 temp_r0;

    temp_r7 = BattleSetup_New(*(enum HeapID *)((u8 *)arg0 + 4), ov80_02236A88(*(u8 *)((u8 *)arg0 + 0xF)));
    sp8 = SaveArray_Party_Get(arg1->saveData);
    sub_02051D18(temp_r7, NULL, arg1->saveData, arg1->mapHeaderID, arg1->bagCursor, arg1->subscreenCursorOn);
    temp_r7->battleBg = BATTLE_BG_BATTLE_TOWER;
    temp_r7->terrain = TERRAIN_BATTLE_TOWER;
    temp_r4 = AllocMonZeroed(*(enum HeapID *)((u8 *)arg0 + 4));
    Party_InitWithMaxSize(temp_r7->party[0], (s32) * (u8 *)((u8 *)arg0 + 0xE));
    var_r6 = 0;
    if ((s32) * (u8 *)((u8 *)arg0 + 0xE) > 0) {
        do {
            CopyPokemonToPokemon(Party_GetMonByIndex(sp8, (s32) * (u8 *)((u8 *)(arg0 + var_r6) + 0x2A)), temp_r4);
            if (GetMonData(temp_r4, 0xA1, NULL) > 0x32U) {
                spC = GetMonExpBySpeciesAndLevel((s32)GetMonData(temp_r4, 5, NULL), 0x32);
                SetMonData(temp_r4, 8, &spC);
                CalcMonLevelAndStats(temp_r4);
            }
            BattleSetup_AddMonToParty(temp_r7, temp_r4, 0);
            var_r6 += 1;
        } while (var_r6 < (s32) * (u8 *)((u8 *)arg0 + 0xE));
    }
    Heap_Free(temp_r4);
    BattleSetup_SetAllySideBattlersToPlayer(temp_r7);
    ov80_02236A34(temp_r7, arg0 + 0x78, *(u8 *)((u8 *)arg0 + 0xE), 1, *(enum HeapID *)((u8 *)arg0 + 4));
    var_r2 = 0;
    var_r1 = temp_r7;
    do {
        var_r2 += 1;
        var_r1->trainer[0].data.aiFlags = 7;
        var_r1 = (void *)((u8 *)var_r1 + sizeof(var_r1->trainer[0]));
    } while (var_r2 < 4);
    temp_r0 = *(u8 *)((u8 *)arg0 + 0xF);
    switch (temp_r0) {
    case 2:
        ov80_02236A34(temp_r7, arg0 + 0x298 + (((u32)(*(u8 *)((u8 *)arg0 + 0x10) << 0x18) >> 0x1D) * 0x110), *(u8 *)((u8 *)arg0 + 0xE), 2, *(enum HeapID *)((u8 *)arg0 + 4));
    case 3:
    case 6:
        ov80_02236A34(temp_r7, arg0 + 0x188, *(u8 *)((u8 *)arg0 + 0xE), 3, *(enum HeapID *)((u8 *)arg0 + 4));
        break;
    }
    return temp_r7;
}
