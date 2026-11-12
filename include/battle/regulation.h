// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/regulation.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_REGULATION_H
#define BWPORT_BATTLE_REGULATION_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

struct Regulation {
    u8 unk0[2];
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u16 unk6;
    u8 unk8;
    u8 unk9;
    u8 unkA[0x52];
    u8 unk5C[0x4c];
    u16 unkA8;
    u8 unkAA;
    u8 unkAB;
    // 0xac to 0xb2: bit arrays, which only Regulation_CheckParamBit reads
    u8 unkAC[7];
    u8 unkB3;
    u8 unkB4;
    u8 unkB5;
    u8 unkB6;
    u8 unkB7;
    // Whether the partners' teams are shown in the selection, and its time limit
    u8 showPartners;
    u8 timeLimit;
    u8 unkBA;
    u8 unkBB;
};

// Reads a regulation from arc 105 (pokebw2's func_0201f744)
void func_0201a364(u32 fileId, Regulation *regulation);

// pokebw2's Regulation_GetParam
// A parameter of the regulation, which is 0 for the bit arrays
u32 func_0200ae54(Regulation *regulation, u32 param);

// The parameters of a regulation, for Regulation_GetParam, Regulation_SetParam and Regulation_CheckParamBit
enum {
    // The message of the regulation's name in the regulations' text bank
    REGULATION_PARAM_NAME,
    REGULATION_PARAM_1,
    REGULATION_PARAM_2,
    REGULATION_PARAM_3,
    REGULATION_PARAM_4,
    REGULATION_PARAM_5,
    REGULATION_PARAM_6,
    REGULATION_PARAM_7,
    REGULATION_PARAM_8,
    // Bit arrays, which only Regulation_CheckParamBit reads
    REGULATION_PARAM_BITS_A,
    REGULATION_PARAM_BITS_5C,
    REGULATION_PARAM_11,
    REGULATION_PARAM_12,
    REGULATION_PARAM_13,
    // A bit array as well
    REGULATION_PARAM_BITS_AC,
    REGULATION_PARAM_15,
    REGULATION_PARAM_16,
    REGULATION_PARAM_17,
    REGULATION_PARAM_18,
    REGULATION_PARAM_SHOW_PARTNERS,
    REGULATION_PARAM_TIME_LIMIT,
    REGULATION_PARAM_BATTLE_TYPE,
    REGULATION_PARAM_22
};

// Black 2's; twinned code confirms the values of REGULATION_RESULT_OK, REGULATION_RESULT_LEVEL_TOTAL,
// REGULATION_RESULT_SAME_SPECIES, REGULATION_RESULT_SAME_ITEM, REGULATION_RESULT_COUNT, REGULATION_RESULT_POKEMON and
// REGULATION_RESULT_REQUIRED in Black
// What a party fails a regulation by, which PokeRegulation_CheckPicked, PokeRegulation_CheckPartyCanEnter and PokeRegulation_CheckParty return
enum {
    REGULATION_RESULT_OK,
    // The levels of the Pokémon add up to more than the regulation allows
    REGULATION_RESULT_LEVEL_TOTAL,
    REGULATION_RESULT_SAME_SPECIES,
    REGULATION_RESULT_SAME_ITEM,
    // Too few Pokémon
    REGULATION_RESULT_COUNT,
    // A Pokémon the regulation doesn't allow
    REGULATION_RESULT_POKEMON,
    REGULATION_RESULT_6,
    // The Pokémon the regulation asks for is missing
    REGULATION_RESULT_REQUIRED
};

// Black 2's; twinned code confirms the values of REGULATION_FAIL_COUNT, REGULATION_FAIL_LEVEL, REGULATION_FAIL_SPECIES,
// REGULATION_FAIL_SAME_SPECIES, REGULATION_FAIL_SAME_ITEM, REGULATION_FAIL_EGG, REGULATION_FAIL_ITEM,
// REGULATION_FAIL_REQUIRED and REGULATION_FAIL_LEVEL_TOTAL in Black
// Every reason a Pokémon or a party fails a regulation, as bits
enum {
    REGULATION_FAIL_COUNT = 1 << 0,
    REGULATION_FAIL_LEVEL = 1 << 1,
    REGULATION_FAIL_SPECIES = 1 << 2,
    REGULATION_FAIL_SAME_SPECIES = 1 << 3,
    REGULATION_FAIL_SAME_ITEM = 1 << 4,
    REGULATION_FAIL_EGG = 1 << 5,
    REGULATION_FAIL_ITEM = 1 << 6,
    REGULATION_FAIL_REQUIRED = 1 << 7,
    REGULATION_FAIL_LEVEL_TOTAL = 1 << 8,
    REGULATION_FAIL_MOVE = 1 << 9,
    REGULATION_FAIL_TYPE = 1 << 10
};

// Black 2's, not checked against Black
// A rule on top of a regulation, for PokeRegulation_CheckPokemonExtraRule
typedef struct {
    // The type a Pokémon must have, or TYPE_NULL
    u8 type;
    // Whether Metronome, Torment and Transform are banned
    u8 banMoves;
    // A banned move, or MOVE_NONE
    u16 move;
} RegulationExtraRule;

// The rules of a regulation as text: the message data and, for each of the six rules, its name and its value
typedef struct {
    MsgData *msgData;
    StrBuf *rule[6];
    StrBuf *value[6];
    // Whether the Wonder Launcher is on
    u8 launcher;
} RegulationPrintMsg;

// pokebw2's Regulation_GetParam
// A parameter of the regulation, which is 0 for the bit arrays
// pokebw2's Regulation_CheckParamBit
// Whether a bit of one of the regulation's bit arrays is set
BOOL func_0200afd4(Regulation *regulation, u32 param, int index);
// pokebw2's PokeRegulation_CheckPicked
// Checks the Pokémon picked from a party, by their slots from 1, against the regulation
u32 func_02019e54(Regulation *regulation, PokeParty *party, const u8 *picked);
// pokebw2's PokeRegulation_CheckPartyCanEnter
// Whether a team that the regulation allows can be picked from the party: REGULATION_RESULT_OK,
// REGULATION_RESULT_COUNT or REGULATION_RESULT_LEVEL_TOTAL
u32 func_02019ed4(Regulation *regulation, PokeParty *party);
// pokebw2's PokeRegulation_ApplyNicknamesToParty
// Gives the party's Pokémon their species' names unless the regulation allows nicknames
void func_0201a30c(Regulation *regulation, PokeParty *party, HeapID heapId);
// pokebw2's PokeRegulation_FreePrintMsg
void func_0201a570(void *printMsg);
// pokebw2's PokeRegulation_PickTeam
// Picks Pokémon of a party that the regulation allows, by their slots from 1, and returns whether it found a team
BOOL func_0201a59c(Regulation *regulation, PokeParty *party, u8 *picked);

void func_0200b040(Regulation *regulation, RegulationUnkAC *out);

// pokebw2's PokeRegulation_ApplyLevelToParty
void func_0201a2a8(Regulation *regulation, PokeParty *party);

// The regulation's name
StrBuf *makeStrbufLoadBattleTypeText(Regulation *regulation, HeapID heapId);

// pokebw2's Regulation_Copy
void func_0200ae08(const Regulation *src, Regulation *dest);

struct RegulationUnkAC {
    u8 unk0[0x7];
    u8 unk7;
};

// pokebw2's Regulation_Create
// The rules of a battle, such as the level cap
Regulation *func_0200adf0(HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PokeRegulation_CheckParty
// Checks a whole party against the regulation, setting the REGULATION_FAIL_* bits of every reason it fails by
u32 func_0201a090(Regulation *regulation, PokeParty *party, u32 *flags);
// pokebw2's PokeRegulation_CreatePrintMsg
// The regulation's rules as text (a RegulationPrintMsg). launcher is used when the regulation leaves the Wonder
// Launcher to the players
void *func_0201a4d4(Regulation *regulation, WordSet *wordset, HeapID heapId, BOOL launcher);
// Allocates a regulation read from arc 106
Regulation *func_0201a354(u32 fileId, HeapID heapId);
// pokebw2's Regulation_SetParam
// Values out of the parameter's range are ignored
void func_0200aee8(Regulation *regulation, u32 param, int value);

#endif // BWPORT_BATTLE_REGULATION_H
