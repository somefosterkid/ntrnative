#include "global.h"

#include "constants/heap.h"

#include "pokemon_types_def.h"
#include "save.h"
#include "save_layout.h"
#include "seal_case.h"
#include "sav_chatot.h"

struct SOUND_CHATOT;
enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
};
typedef struct GlobalTrade {
    u8 storedPokemon[sizeof(Pokemon)];
    u16 pokemonListed;
    u16 neverSet_EE;
    u32 depositTradeDatestamp;
    u32 searchTradeDatestamp;
} GlobalTrade;
LAYOUT_CHECK(GlobalTrade_size, sizeof(GlobalTrade) == 0xF8);
LAYOUT_CHECK_OFFSET(GlobalTrade_pokemonListed, GlobalTrade, pokemonListed, 0xEC);

SOUND_CHATOT *sub_0202DB34(SaveData *saveData);
u32 sub_0202DB40(void);
void sub_0202DB44(void *globalTrade);

SOUND_CHATOT *sub_0202DB34(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_TABLE_ENTRY_CHATOT);
}

u32 sub_0202DB40(void) {
    return sizeof(GlobalTrade);
}

void sub_0202DB44(void *ptr) {
    GlobalTrade *globalTrade = ptr;
    MI_CpuClearFast(globalTrade, sizeof(GlobalTrade));
}
