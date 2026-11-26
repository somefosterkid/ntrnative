#include "global.h"
struct FieldFrontierDTO;

#include "constants/battle.h"
#include "constants/heap.h"

#include "battle/battle_setup.h"

#include "bag_cursor.h"
#include "heap.h"
#include "overlay_80_02237A70.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "trainer_data.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "use_item_on_mon.h"
#include "overlay_80_field_frontier_dto.h"
#include "overlay_80_battle_castle.h"

void *ov80_02229F04(s32 *, s32, s32, s32);

s32 ov80_0222A480(BattleSetup *, s32 *, s32, s32, s32);

u8 ov80_02237B58(u8, s32);

u16 ov80_02237E88(BattleCastle *);

BattleSetup *ov80_02237B8C(struct BattleCastle *castle, FieldFrontierDTO *arg1);

BattleSetup *ov80_02237B8C(struct BattleCastle *castle, FieldFrontierDTO *arg1) {
    s32 sp18[12];
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    BattleSetup *temp_r0;
    BattleSetup *var_r6_2;
    Pokemon *temp_r7;
    Pokemon *temp_r7_2;
    Pokemon *temp_r7_3;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 var_r6;
    s32 var_r6_3;
    s32 var_r6_4;
    s32 var_r7;
    u8 temp_r0_2;

    sp10 = (s32)ov80_02237B24(castle->challengeType, 0);
    spC = ov80_02237B58(castle->challengeType, 0);
    HealParty(castle->opponentsParty);
    temp_r0 = BattleSetup_New(HEAP_ID_FIELD2, ov80_02237D5C(castle->challengeType));
    sub_02051D18(temp_r0, NULL, arg1->saveData, arg1->mapHeaderID, arg1->bagCursor, arg1->subscreenCursorOn);
    temp_r0->battleBg = BATTLE_BG_BATTLE_CASTLE;
    temp_r0->terrain = TERRAIN_BATTLE_CASTLE;
    Party_InitWithMaxSize(temp_r0->party[0], sp10);
    if (sub_0203769C() == 0) {
        var_r6 = 0;
    } else {
        var_r6 = 2;
    }
    temp_r7 = AllocMonZeroed(HEAP_ID_FIELD2);
    sp14 = 0;
    if (sp10 > 0) {
        do {
            CopyPokemonToPokemon(Party_GetMonByIndex(castle->playersParty, var_r6), temp_r7);
            BattleSetup_AddMonToParty(temp_r0, temp_r7, 0);
            var_r6 += 1;
            temp_r1 = sp14 + 1;
            sp14 = temp_r1;
        } while (temp_r1 < sp10);
    }
    Heap_Free(temp_r7);
    BattleSetup_SetAllySideBattlersToPlayer(temp_r0);
    Heap_Free(ov80_02229F04(sp18, castle->trainerIDs[castle->currentBattle], 0xB, 0xCC));
    ov80_0222A480(temp_r0, sp18, spC, 1, 0xB);
    Party_InitWithMaxSize(temp_r0->party[1], ov80_02237B58(castle->challengeType, 0));
    var_r7 = 0;
    var_r6_2 = temp_r0;
    do {
        var_r6_2->trainer[0].data.aiFlags = ov80_02237E88(castle);
        var_r7 += 1;
        var_r6_2 = (void *)((u8 *)var_r6_2 + (0x34));
    } while (var_r7 < 4);
    temp_r7_2 = AllocMonZeroed(HEAP_ID_FIELD2);
    var_r6_3 = 0;
    if (spC > 0) {
        do {
            CopyPokemonToPokemon(Party_GetMonByIndex(castle->opponentsParty, var_r6_3), temp_r7_2);
            BattleSetup_AddMonToParty(temp_r0, temp_r7_2, 1);
            var_r6_3 += 1;
        } while (var_r6_3 < spC);
    }
    Heap_Free(temp_r7_2);
    temp_r0_2 = castle->challengeType;
    if (temp_r0_2 != 2) {
        if (temp_r0_2 == 3) {
            goto block_11;
        }
    } else {
block_11:
        BattleSetup_SetAllySideBattlersToPlayer(temp_r0);
        PlayerProfile_Copy(sub_02034818(1 - sub_0203769C()), temp_r0->profile[2]);
        Heap_Free(ov80_02229F04(sp18, castle->trainerIDs[castle->currentBattle + 7], 0xB, 0xCC));
        ov80_0222A480(temp_r0, sp18, spC, 3, 0xB);
        Party_InitWithMaxSize(temp_r0->party[3], ov80_02237B58(castle->challengeType, 0));
        temp_r7_3 = AllocMonZeroed(HEAP_ID_FIELD2);
        sp8 = 0;
        if (spC > 0) {
            var_r6_4 = spC;
            do {
                CopyPokemonToPokemon(Party_GetMonByIndex(castle->opponentsParty, var_r6_4), temp_r7_3);
                BattleSetup_AddMonToParty(temp_r0, temp_r7_3, 3);
                var_r6_4 += 1;
                temp_r1_2 = sp8 + 1;
                sp8 = temp_r1_2;
            } while (temp_r1_2 < spC);
        }
        Heap_Free(temp_r7_3);
    }
    return temp_r0;
}
