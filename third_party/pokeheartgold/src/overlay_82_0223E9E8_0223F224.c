#include "global.h"

#include "heap.h"
#include "overlay_82.h"
#include "party.h"
#include "player_data.h"
#include "pokedex_util.h"
#include "save_special_ribbons.h"
#include "unk_02088288.h"
#include "overlay_82_battle_hall_app.h"

extern u8 _0223FE20;

void ov82_0223F224(void *arg0) {
    *((PokemonSummaryArgs **)&((BattleHallApp *)arg0)->monSummary) = Heap_Alloc(HEAP_ID_105, sizeof(PokemonSummaryArgs));
    memset(((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary), 0, sizeof(PokemonSummaryArgs));
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->party = ((BattleHallApp *)arg0)->party;
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->unk11 = 1;
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->options = ((BattleHallApp *)arg0)->options;
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->unk12 = 1;
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->partyCount = (u8)Party_GetCount(((BattleHallApp *)arg0)->party);
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->partySlot = 0;
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->moveToLearn = 0;
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->natDexEnabled = SaveArray_IsNatDexEnabled(((BattleHallApp *)arg0)->saveData);
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->unk2C = sub_02088288(((BattleHallApp *)arg0)->saveData);
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->ribbons = Save_SpecialRibbons_Get(((BattleHallApp *)arg0)->saveData);
    (((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary))->isFlag982Set = 0;
    sub_02089D40(((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary), &_0223FE20);
    sub_0208AD34(((PokemonSummaryArgs *)((BattleHallApp *)arg0)->monSummary), Save_PlayerData_GetProfile(((BattleHallApp *)arg0)->saveData));
}

void ov82_0223F2F8(void *arg0, s32 *arg1, s32 arg2);

void ov82_0223F2F8(void *arg0, s32 *arg1, s32 arg2) {
    *(s8 *)((u8 *)arg0 + 8) = 0;
    *arg1 = arg2;
}
