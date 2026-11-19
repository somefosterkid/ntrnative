#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02020A0C.h"
#include "wild_encounter.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"

enum BattlePartyScreen {
    BATTLE_PARTY_SCREEN_POKEMON_PARTY = 0,
    BATTLE_PARTY_SCREEN_SELECT_POKEMON,
    BATTLE_PARTY_SCREEN_POKEMON_SUMMARY,
    BATTLE_PARTY_SCREEN_POKEMON_MOVES,
    BATTLE_PARTY_SCREEN_MOVE_SUMMARY,
    BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP,
    BATTLE_PARTY_SCREEN_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE_CONTEST_STATS,
};

extern s32 ov08_02224F80;

void ov08_0221D8B0(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_0221D8B0(BattleParty *battleParty, enum BattlePartyScreen screen) {
    NNSG2dScreenData *ppScrData;
    s32 *v2;
    u32 v1;
    void *v0;

    v1 = 0;
    v2 = &ov08_02224F80 + screen * 2;
    do {
        v0 = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_7_1, *v2, battleParty->context->heapID);
        NNS_G2dGetUnpackedScreenData(v0, &ppScrData);
        LoadRectToBgTilemapRect(battleParty->background, (u8)(v1 + 6), ppScrData->rawData, 0U, 0U, 0x20U, 0x18U);
        ScheduleBgTilemapBufferTransfer(battleParty->background, (u8)(v1 + 6));
        Heap_Free(v0);
        v1 += 1;
        v2 += 1;
    } while (v1 < 2U);
}
