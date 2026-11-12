// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/high_link.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SAVE_HIGH_LINK_H
#define BWPORT_SAVE_HIGH_LINK_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Black 2's; matching code confirms the offsets of level1 and level2, and its size in Black
struct PassPowerLevel {
    u8 unk00[6];
    u8 unk06[6];
    u16 level1;
    u16 level2;
};

void setupProbabilityTable(PassPowerLevel *levels);
void func_0200be6c(SaveControl *save, PassPowerLevel *levels);
// Copies the levels into the save, and out of it
void func_0200be54(SaveControl *save, const PassPowerLevel *levels);

// Black 2's, not checked against Black
#define HIGH_LINK_POWER_MAX 3

#define HIGH_LINK_POWER_NONE 0x30

// One of the 20 entries in HighLinkSave's unk10 (0x14 bytes each), as the code reads it.
typedef struct HighLinkEntry {
    u16 words[8];   // 0x00
    u16 unk10;      // 0x10: 0 when the slot is free
    u8 unk12;       // 0x12
    u8 unk13;       // 0x13
} HighLinkEntry;

// Black 2's; twinned code confirms the offsets of levels in Black
struct HighLinkSave {
    PassPowerLevel levels;
    HighLinkEntry unk10[20];
    // The pass power in each of the three slots, or HIGH_LINK_POWER_NONE
    // Black uses powers[0] alone; powers[1] and powers[2] hold a bit for each special pass power received
    // (func_0200bf18), where Black 2 has specialFlags at 0x1a4
    u8 powers[HIGH_LINK_POWER_MAX];
    u8 padding1A3;
    // Counted up to 99999 by func_0200bf78 when a mission's Pass Orbs are given, read by func_0200bf70. Black has
    // this word where Black 2 has specialFlags[2] and six bytes: the struct is 0x1b0 bytes here
    // (getSizeofProbabilityTable)
    // Black's is a count, at most 99999, that func_0200bf78 adds one to as the comm reward gives Pass Orbs and
    // func_0200bf70 reads (Black 2 has the special pass powers' flags and six bytes here: the struct is 0x1b0 bytes
    // in Black, getSizeofProbabilityTable)
    u32 unk1A4;
    // The play time counted by HighLinkSave_CountPlaySeconds
    u64 seconds;
};

// Black 2's, not checked against Black
#define PASS_POWER_SPECIAL_FIRST 0x21

// Black 2's, not checked against Black
#define PASS_POWER_SPECIAL_LAST 0x26
#define PASS_POWER_BW1_MAX 0x2e

#define PASS_POWER_NONE HIGH_LINK_POWER_NONE

// Black 2's, not checked against Black
#define PASS_POWER_MAX 0x40

// Black has 13 effects, 0 to 12; Black 2 added three more (exploring, grotto, lucky) after them
// What a pass power does. One pass power of each effect runs at a time
typedef enum {
    PASS_POWER_EFFECT_ENCOUNTER_UP,
    PASS_POWER_EFFECT_ENCOUNTER_DOWN,
    PASS_POWER_EFFECT_HATCH,
    PASS_POWER_EFFECT_BEFRIEND,
    PASS_POWER_EFFECT_BARGAIN,
    PASS_POWER_EFFECT_HP_RESTORE,
    PASS_POWER_EFFECT_PP_RESTORE,
    PASS_POWER_EFFECT_EXP_UP,
    PASS_POWER_EFFECT_EXP_DOWN,
    PASS_POWER_EFFECT_PRIZE_MONEY,
    PASS_POWER_EFFECT_CAPTURE,
    PASS_POWER_EFFECT_11,
    PASS_POWER_EFFECT_12,
    PASS_POWER_EFFECT_COUNT,
} PassPowerEffect;

// Black 2's; matching code confirms the offsets of unk02 and effect in Black
// A pass power of the table that PassPowerData_Create reads: the two levels it is unlocked by, White then Black,
// its name in message file 263 and its description in message file 264
typedef struct {
    u8 level1;
    u8 level2;
    // The price in Pass Orbs, which the description shows
    u16 unk02;
    u16 name;
    u16 info;
    // The strength of the effect: a percentage, an amount or a level
    u16 value;
    // PASS_POWER_EFFECT_*
    u8 effect;
    // How long it lasts, in minutes
    u8 minutes;
} PassPowerData;

// The pass power that runs with the effect of the given one, and with a given effect, or PASS_POWER_NONE
u32 PassPower_GetUsedIDByPassPower(u32 passPower, void *data);
// The time left of the Pass Power in use, or 0
s32 func_02010eec(void);
// The time an effect has left, at least 1 while it runs
u32 PassPower_GetRemainingSeconds(int effect);
// Ends the pass power that runs with the effect of the given one
void PassPower_Deactivate(u32 passPower, void *data);
// A pass power's effect and its price in Pass Orbs, from the table of PassPowerData_Create
u8 PassPowerData_GetEffect(void *data, u32 passPower);
u16 PassPowerData_GetPrice(void *data, u32 passPower);


// The pass power that runs with the effect of the given one, and with a given effect, or PASS_POWER_NONE

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The pass powers that act at once: they restore the party's HP or PP
u32 PassPower_ApplyHPRestore(PokeParty *party);
u32 PassPower_ApplyPPRestore(PokeParty *party);
u32 PassPower_ApplyPrizeMoney(u32 money);
s32 PassPower_ApplyCapture(s32 rate);
// The pass powers on wild encounters: the encounter rate, the rolls for a shiny and the row of the slots' odds
u32 PassPower_ApplyEncounter(u32 rate);
void PassPowerData_Free(void *data);
int PassPower_ApplyBefriend(int friendship);
// The block's size and its initial state; swan's names for them are misnomers
u32 getSizeofProbabilityTable(void);
void probability(HighLinkSave *save);
// Not decompiled yet: the prototypes are from overlay 11's use
void func_0200be84(HighLinkSave *save, const void *data);
u32 func_0200bf70(HighLinkSave *save);
// Counts unk1A4 up, to 99999 at most, and returns it
u32 func_0200bf78(HighLinkSave *save);
u64 func_0200bf94(HighLinkSave *save);
u64 func_0200bfa4(HighLinkSave *save, PlayTime *playTime, u32 a2);
void func_0200bf54(HighLinkSave *save, u8 *dest, u32 count);
// Set and get the pass power in the first slot
void func_0200bf08(HighLinkSave *save, u32 power);
u32 func_0200bf10(HighLinkSave *save);
u16 func_0200bdf8(void *levels);
u16 func_0200bdfc(void *levels);
// pokebw2's HighLinkSave_AddWhiteLevel
// Add to the White and the Black level, up to 999
void func_0200be00(PassPowerLevel *levels, int add);
// pokebw2's HighLinkSave_AddBlackLevel
void func_0200be24(PassPowerLevel *levels, int add);
u32 PassPower_GetUsedIDByEffect(int effect);
// The pass powers' table, read from its archive
void *PassPowerData_Create(HeapID heapId);
// An egg's steps, and the friendship a Pokémon gains
u32 PassPower_ApplyHatch(u32 steps);
u32 PassPower_ApplyBargain(u32 price);
u32 PassPower_ApplyEXP(u32 exp);
// Counts the pass powers' time down, once a frame
void PassPower_Update(void);

#endif // BWPORT_SAVE_HIGH_LINK_H
