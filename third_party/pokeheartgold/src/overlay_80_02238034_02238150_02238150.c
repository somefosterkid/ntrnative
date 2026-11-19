#include "global.h"

#include "battle/battle_setup.h"
#include "frontier/overlay_80_02238034.h"

#include "heap.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "use_item_on_mon.h"

void *ov80_02229F04(s32 *, s32, s32, s32);

s32 ov80_0222A480(BattleSetup *, s32 *, s32, s32, s32);

u32 ov80_02238344(u8);

u16 ov80_02238444(ArcadeContext *);

BattleSetup *BattleArcade_NewBattleSetup(ArcadeContext *ctx, FrontierLaunchArgs *args) {
    s32 sp20[12];
    s32 sp1C;
    Party *sp18;
    Party *sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    BattleSetup *temp_r0;
    BattleSetup *var_r5_2;
    Party *temp_r1;
    Pokemon *temp_r6;
    Pokemon *temp_r6_2;
    Pokemon *temp_r6_3;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 var_r5;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r6;
    u8 temp_r0_2;

    sp10 = (s32)BattleArcade_GetMonCount(ctx->type, 0);
    spC = (s32)BattleArcade_GetOpponentMonCount(ctx->type, 0);
    temp_r0 = BattleSetup_New(HEAP_ID_FIELD2, ov80_02238344(ctx->type));
    sub_02051D18(temp_r0, NULL, args->saveData, args->mapId, args->bagCursor, args->unk1C);
    temp_r0->battleBg = BATTLE_BG_BATTLE_ARCADE;
    temp_r0->terrain = TERRAIN_BATTLE_ARCADE;
    temp_r0->weatherType = ctx->weather;
    temp_r1 = ctx->playerParty;
    sp14 = ctx->opponentParty;
    sp18 = temp_r1;
    if (ctx->unk13 == 0x1B) {
        sp18 = sp14;
        sp14 = temp_r1;
    }
    Party_InitWithMaxSize(temp_r0->party[0], sp10);
    if (sub_0203769C() == 0) {
        var_r5 = 0;
    } else {
        var_r5 = 2;
    }
    temp_r6 = AllocMonZeroed(HEAP_ID_FIELD2);
    sp1C = 0;
    if (sp10 > 0) {
        do {
            CopyPokemonToPokemon(Party_GetMonByIndex(sp18, var_r5), temp_r6);
            BattleSetup_AddMonToParty(temp_r0, temp_r6, 0);
            var_r5 += 1;
            temp_r1_2 = sp1C + 1;
            sp1C = temp_r1_2;
        } while (temp_r1_2 < sp10);
    }
    Heap_Free(temp_r6);
    BattleSetup_SetAllySideBattlersToPlayer(temp_r0);
    Heap_Free(ov80_02229F04(sp20, ctx->unk74[ctx->unk11], 0xB, 0xCC));
    ov80_0222A480(temp_r0, sp20, spC, 1, 0xB);
    Party_InitWithMaxSize(temp_r0->party[1], (s32)BattleArcade_GetOpponentMonCount(ctx->type, 0));
    var_r6 = 0;
    var_r5_2 = temp_r0;
    do {
        var_r5_2->trainer[0].data.aiFlags = ov80_02238444(ctx);
        var_r5_2 = (void *)((u8 *)var_r5_2 + (0x34));
        var_r6 += 1;
    } while (var_r6 < 4);
    temp_r6_2 = AllocMonZeroed(HEAP_ID_FIELD2);
    var_r5_3 = 0;
    if (spC > 0) {
        do {
            CopyPokemonToPokemon(Party_GetMonByIndex(sp14, var_r5_3), temp_r6_2);
            BattleSetup_AddMonToParty(temp_r0, temp_r6_2, 1);
            var_r5_3 += 1;
        } while (var_r5_3 < spC);
    }
    Heap_Free(temp_r6_2);
    temp_r0_2 = ctx->type;
    switch (temp_r0_2) {
    case 2:
    case 3:
        BattleSetup_SetAllySideBattlersToPlayer(temp_r0);
        PlayerProfile_Copy(sub_02034818(1 - sub_0203769C()), temp_r0->profile[2]);
        Heap_Free(ov80_02229F04(sp20, ctx->unk74[ctx->unk11 + 7], 0xB, 0xCC));
        ov80_0222A480(temp_r0, sp20, spC, 3, 0xB);
        Party_InitWithMaxSize(temp_r0->party[3], (s32)BattleArcade_GetOpponentMonCount(ctx->type, 0));
        temp_r6_3 = AllocMonZeroed(HEAP_ID_FIELD2);
        sp8 = 0;
        if (spC > 0) {
            var_r5_4 = spC;
            do {
                CopyPokemonToPokemon(Party_GetMonByIndex(sp14, var_r5_4), temp_r6_3);
                BattleSetup_AddMonToParty(temp_r0, temp_r6_3, 3);
                var_r5_4 += 1;
                temp_r1_3 = sp8 + 1;
                sp8 = temp_r1_3;
            } while (temp_r1_3 < spC);
        }
        Heap_Free(temp_r6_3);
        break;
    }
    HealParty(ctx->playerParty);
    HealParty(ctx->opponentParty);
    return temp_r0;
}
