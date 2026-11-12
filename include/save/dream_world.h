// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/dream_world.h, the lines in use so far.
#ifndef BWPORT_SAVE_DREAM_WORLD_H
#define BWPORT_SAVE_DREAM_WORLD_H

#include "types.h"
#include "struct_decls.h"
#include "pml/poke_party.h"

// pokebw2's DreamWorldSave_IsPokemonAsleep
BOOL func_02009304(DreamWorldSave *dreamWorld);
// pokebw2's func_020099f4, func_020099e0, func_02009a98, func_02009ae0, func_02009b20, func_02009b30 and
// func_02009af8
u8 func_020092ec(DreamWorldSave *dreamWorld);
u8 func_020092d8(DreamWorldSave *dreamWorld);
u16 *func_02009390(DreamWorldSave *dreamWorld, int index);
u32 func_020093d8(DreamWorldSave *dreamWorld);
u32 func_02009418(DreamWorldSave *dreamWorld);
void func_02009428(DreamWorldSave *dreamWorld, u32 value);
void func_020093f0(DreamWorldSave *dreamWorld, int value);

// Black 2's, not checked against Black
#define DREAM_WORLD_ITEM_MAX 20
#define DREAM_WORLD_ITEM_COUNT_MAX 20

#define DREAM_WORLD_RESULT_MAX 5

// The chosen result entry when there is none: it holds an index into results otherwise
#define DREAM_WORLD_RESULT_INDEX_NONE 0x7f

// Black 2's, not checked against Black
#define DREAM_WORLD_RESULT_WORDS 13

// The first word of a result entry until a result comes
#define DREAM_WORLD_RESULT_NONE 0x7e

// Black 2's; matching code confirms the offsets of syncDate, lastResultId, pkm, items, itemCounts, results, asleep,
// unk1A4, idShown, resultIndex, unk1A6_7, unk1A7, unk1A8 and unk1A9 in Black
// Block 0x23 (swan's getDreamWorldStuffAddress): the Pokémon sent to the Dream World, the items and the result
// that came back, and the Game Sync's state
struct DreamWorldSave {
    // The date of the last Game Sync, packed as year, month, day and weekday from the top byte down
    u32 syncDate;
    // The ID of the last result taken from the Dream World, -1 when there is none
    u32 lastResultId;
    PartyPkm pkm;
    u16 items[DREAM_WORLD_ITEM_MAX];
    u8 itemCounts[DREAM_WORLD_ITEM_MAX];
    // The entries of the Dream World's result: a word that is 0x7e until a result comes, then a fixed string
    u16 results[DREAM_WORLD_RESULT_MAX][DREAM_WORLD_RESULT_WORDS];
    u8 unk1A2;
    u8 asleep;
    // From the Dream World's result
    u8 unk1A4;
    // Set once the Game Sync ID was shown
    u8 idShown;
    // The index of the chosen result entry, DREAM_WORLD_RESULT_INDEX_NONE for none, and a flag with it
    u8 resultIndex : 7;
    u8 unk1A6_7 : 1;
    u8 unk1A7;
    u8 unk1A8;
    u8 unk1A9;
    u8 padding1AA[2];
}; // size: 0x1ac

u8 func_02009330(DreamWorldSave *dreamWorld, int index);
void func_02009348(DreamWorldSave *dreamWorld, int index, u16 item, u8 count);
// How many items the Dream World sent
s32 func_02009370(DreamWorldSave *dreamWorld);
void func_020093a8(DreamWorldSave *dreamWorld, int index, const void *entry);
void func_02009230(DreamWorldSave *dreamWorld);
void func_02009294(DreamWorldSave *dreamWorld, PartyPkm *pkm);
// Whether a species' bit is set in a list of species flags
BOOL func_020092ac(const u8 *flags, int species);
void func_020092d0(DreamWorldSave *dreamWorld, u8 value);
void func_020092e0(DreamWorldSave *dreamWorld, u8 value);
void func_020092f8(DreamWorldSave *dreamWorld, u8 asleep);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_020093cc(DreamWorldSave *dreamWorld, u32 date);
// The items sent from the Dream World and their counts, 0 when out of range
u16 func_02009310(DreamWorldSave *dreamWorld, int index);
void func_02009364(DreamWorldSave *dreamWorld, int index);
u32 func_020093c8(DreamWorldSave *dreamWorld);
u32 func_020093d0(DreamWorldSave *dreamWorld);
void func_020093d4(DreamWorldSave *dreamWorld, u32 id);
// The Pokémon sent to the Dream World, and whether it is there
PartyPkm *func_02009290(DreamWorldSave *dreamWorld);

#endif // BWPORT_SAVE_DREAM_WORLD_H
