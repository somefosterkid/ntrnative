#ifndef POKEHEARTGOLD_POKEMON_SUMMARY_ARGS_H
#define POKEHEARTGOLD_POKEMON_SUMMARY_ARGS_H

#include "global.h"

struct Party;
struct Options;
struct SaveSpecialRibbons;
struct SOUND_CHATOT;
struct MenuInputStateMgr;

// Argument block passed to the Pokemon Summary / Learn-Forget-Move application
// (Platinum's struct PokemonSummary). Kept free of heavy includes so that files
// embedding it by value can include just this header.
typedef struct PokemonSummaryArgs {
    // What the pointer addresses depends on unk11: 0 = Pokemon[], 1 = Party, 2 = BoxPokemon[].
    union {
        struct Party *party;
        void *monData;
    };
    struct Options *options;
    const u16 *OTName;
    u32 OTID;
    u8 OTGender;
    u8 unk11;
    u8 unk12;
    u8 partyCount;
    u8 partySlot;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u16 moveToLearn;
    u8 unk1A[0x2];
    BOOL natDexEnabled;
    struct SaveSpecialRibbons *ribbons;
    u8 unk24[0x4];
    struct SOUND_CHATOT *unk28;
    int unk2C;
    struct MenuInputStateMgr *menuInputStatePtr;
    BOOL isFlag982Set;
    BOOL unk38;
} PokemonSummaryArgs;

#endif // POKEHEARTGOLD_POKEMON_SUMMARY_ARGS_H
