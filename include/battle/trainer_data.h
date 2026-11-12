// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/trainer_data.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_TRAINER_DATA_H
#define BWPORT_BATTLE_TRAINER_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

u32 TrainerData_GetParam(int trainerId, u32 param);

// Black 2's, not checked against Black
// What each party entry holds besides the Pokémon itself, OR'ed together, as tools/data/trainer_data.py packs it
#define PARTY_MOVES 1

// Black 2's, not checked against Black
#define PARTY_ITEMS 2
// The first ID of the Battle Institute's and similar facilities' trainers, whose records and messages are kept apart
#define TRAINER_ID_BATTLE_INST_START 0x500

// Black 2's, not checked against Black
// A flag of GetTrSpriteBaseDatID: the back sprite rather than the front one
#define TRAINER_SPRITE_BACK 1

// Black 2's, not checked against Black
// The number of classes with a battle BGM or pedestal of their own, and what GetTrainerClassSpecialId returns for
// the others
#define TRAINER_CLASS_SPECIAL_COUNT 29

// A trainer's record in ARCID_TRDATA, packed from data/trainers/ by tools/data/trainer_data.py
struct TrainerData {
    u8 partyKind; // PARTY_*
    u8 trainerClass;
    u8 battleStyle; // BTL_STYLE_*
    u8 pokeCount;
    u16 items[4];
    u32 aiFlags;
    u8 heals : 1;
    // The prize money's multiplier
    u8 money;
    u16 rewardItem;
};

// A field of a trainer's record, for TrainerData_GetParam
enum {
    TRAINER_PARAM_PARTY_KIND,
    TRAINER_PARAM_CLASS,
    TRAINER_PARAM_BATTLE_STYLE,
    TRAINER_PARAM_POKE_COUNT,
    TRAINER_PARAM_ITEM_1,
    TRAINER_PARAM_ITEM_2,
    TRAINER_PARAM_ITEM_3,
    TRAINER_PARAM_ITEM_4,
    TRAINER_PARAM_AI_FLAGS,
    TRAINER_PARAM_HEALS,
    TRAINER_PARAM_MONEY,
    TRAINER_PARAM_REWARD_ITEM,
};

// Black 2's, not checked against Black
// A Pokémon of a trainer's party in ARCID_TRPOKE, in one of four layouts by the party's kind
struct TrainerPoke {
    // 0 to 255 for IVs of 0 to 31
    u8 difficulty;
    // The sex in the low nibble (1 male, 2 female, 0 either) and the ability in the high one (1 or 2 the first or
    // second, 3 the hidden one, 0 either)
    u8 genderAbility;
    u16 level;
    u16 species;
    u16 form;
};

// Black 2's, not checked against Black
struct TrainerPokeMoves {
    TrainerPoke base;
    u16 moves[4];
};

// Black 2's, not checked against Black
struct TrainerPokeItem {
    TrainerPoke base;
    u16 item;
};

// Black 2's, not checked against Black
struct TrainerPokeItemMoves {
    TrainerPoke base;
    u16 item;
    u16 moves[4];
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u8 GetTrainerClassBGMGroupId(u32 trainerClass);
// The battle terrain the class's trainers fight on, 0x11 for the field's own
u32 func_0202a420(u32 trainerClass);
// Black's takes no game data (from BtlSetup_LoadTrainer's call)
void TrainerUtil_LoadTrainer(int trainerId, BtlSetupTrainer *trainer, HeapID heapId);
void TrainerUtil_LoadParty(int trainerId, PokeParty *party, HeapID heapId);
// The BGM played when the class's trainers spot the player
u16 GetTrainerClassEyeBGMID(u32 trainerClass);
// pokebw2's TrainerMsg_CheckExists
u32 func_0202a298(u32 id, u32 trainerClass, HeapID heapId);
// pokebw2's TrainerMsg_Load
// Loads the trainer's message of the type, or clears strbuf if it has none. Battle institution trainers only have types 0
// and 1, and any other type leaves strbuf as it was
void func_0202a310(u16 trainerId, u16 trainerClass, StrBuf *strBuf, HeapID heapId);

// Black's func_ov093_021d7f38: whether the trainer class plays the second BGM group
BOOL func_ov093_021d7f38(u32 trainerClass);
// src/system/tr_tool.c: checks a trainer's message of the type exists

#endif // BWPORT_BATTLE_TRAINER_DATA_H
