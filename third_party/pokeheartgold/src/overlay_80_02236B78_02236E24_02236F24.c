#include "global.h"
struct FieldFrontierDTO;

#include "constants/battle.h"
#include "constants/heap.h"

#include "battle/battle_setup.h"

#include "bag_cursor.h"
#include "heap.h"
#include "overlay_80_02236B78.h"
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
#include "overlay_80_battle_factory.h"

void *ov80_02229F04(s32 *, s32, s32, s32);

s32 ov80_0222A480(BattleSetup *, s32 *, s32, s32, s32);

u16 ov80_02237264(void *);

BattleSetup *ov80_02236F24(BattleFactory *factory, struct FieldFrontierDTO *arg1);

BattleSetup *ov80_02236F24(BattleFactory *factory, FieldFrontierDTO *arg1) {
    s32 sp14[12];
    s32 sp10;
    s32 spC;
    s32 sp8;
    BattleSetup *temp_r0;
    BattleSetup *var_r6_2;
    Pokemon *temp_r7;
    Pokemon *temp_r7_2;
    Pokemon *temp_r7_3;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 var_r6;
    s32 var_r6_3;
    s32 var_r6_4;
    s32 var_r7;
    u8 temp_r0_3;

    temp_r0_2 = sp10 = (s32)ov80_02236DD4(factory->challengeType);
    spC = (s32)ov80_02236DF8(factory->challengeType, 0);
    HealParty(factory->playersParty);
    HealParty(factory->opponentsParty);
    temp_r0 = BattleSetup_New(HEAP_ID_FIELD2, ov80_022370F4(factory->challengeType));
    sub_02051D18(temp_r0, NULL, arg1->saveData, *((u32 *)&arg1->unk_18), *((BagCursor * *)&arg1->journalEntry), *((void * *)&arg1->mapHeaderID));
    temp_r0->battleBg = BATTLE_BG_BATTLE_FACTORY;
    temp_r0->terrain = TERRAIN_BATTLE_FACTORY;
    Party_InitWithMaxSize(temp_r0->party[0], sp10);
    temp_r7 = AllocMonZeroed(HEAP_ID_FIELD2);
    var_r6 = 0;
    if (temp_r0_2 > 0) {
        do {
            CopyPokemonToPokemon(Party_GetMonByIndex(factory->playersParty, var_r6), temp_r7);
            BattleSetup_AddMonToParty(temp_r0, temp_r7, 0);
            var_r6 += 1;
        } while (var_r6 < sp10);
    }
    Heap_Free(temp_r7);
    BattleSetup_SetAllySideBattlersToPlayer(temp_r0);
    Heap_Free(ov80_02229F04(sp14, factory->trainerIDs[factory->currentBattle], 0xB, 0xCC));
    ov80_0222A480(temp_r0, sp14, spC, 1, 0xB);
    Party_InitWithMaxSize(temp_r0->party[1], (s32)ov80_02236DF8(factory->challengeType, 0));
    var_r7 = 0;
    var_r6_2 = temp_r0;
    do {
        var_r6_2->trainer[0].data.aiFlags = ov80_02237264(factory);
        var_r7 += 1;
        var_r6_2 = (void *)((u8 *)var_r6_2 + (0x34));
    } while (var_r7 < 4);
    temp_r7_2 = AllocMonZeroed(HEAP_ID_FIELD2);
    var_r6_3 = 0;
    if (spC > 0) {
        do {
            CopyPokemonToPokemon(Party_GetMonByIndex(factory->opponentsParty, var_r6_3), temp_r7_2);
            BattleSetup_AddMonToParty(temp_r0, temp_r7_2, 1);
            var_r6_3 += 1;
        } while (var_r6_3 < spC);
    }
    Heap_Free(temp_r7_2);
    temp_r0_3 = factory->challengeType;
    if (temp_r0_3 != 2) {
        if (temp_r0_3 == 3) {
            goto block_8;
        }
    } else {
block_8:
        BattleSetup_SetAllySideBattlersToPlayer(temp_r0);
        PlayerProfile_Copy(sub_02034818(1 - sub_0203769C()), temp_r0->profile[2]);
        Heap_Free(ov80_02229F04(sp14, factory->trainerIDs[factory->currentBattle + 7], 0xB, 0xCC));
        ov80_0222A480(temp_r0, sp14, spC, 3, 0xB);
        Party_InitWithMaxSize(temp_r0->party[3], (s32)ov80_02236DF8(factory->challengeType, 0));
        temp_r7_3 = AllocMonZeroed(HEAP_ID_FIELD2);
        sp8 = 0;
        if (spC > 0) {
            var_r6_4 = spC;
            do {
                CopyPokemonToPokemon(Party_GetMonByIndex(factory->opponentsParty, var_r6_4), temp_r7_3);
                BattleSetup_AddMonToParty(temp_r0, temp_r7_3, 3);
                var_r6_4 += 1;
                temp_r1 = sp8 + 1;
                sp8 = temp_r1;
            } while (temp_r1 < spC);
        }
        Heap_Free(temp_r7_3);
    }
    return temp_r0;
}
