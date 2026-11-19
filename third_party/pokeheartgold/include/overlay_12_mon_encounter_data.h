#ifndef POKEHEARTGOLD_OVERLAY_12_MON_ENCOUNTER_DATA_H
#define POKEHEARTGOLD_OVERLAY_12_MON_ENCOUNTER_DATA_H

// MonEncounterData: task data of the wild/opponent Pokemon slide-in (ov12_02259968 allocates it,
// ov12_0225B494 and ov12_0225B7B8 are the two SysTask handlers). Heap_Alloc(HEAP_ID_BATTLE, sizeof(MonEncounterData)).

#include "global.h"

enum PokemonCryMod {
    POKECRY_NORMAL = 0,
    POKECRY_HALF_DURATION,
    POKECRY_FIELD_EVENT,
    POKECRY_MID_MOVE,
    POKECRY_HYPERVOICE_1,
    POKECRY_FAINT,
    POKECRY_HYPERVOICE_2,
    POKECRY_HOWL_1,
    POKECRY_HOWL_2,
    POKECRY_UPROAR_1,
    POKECRY_UPROAR_2,
    POKECRY_PINCH_NORMAL,
    POKECRY_PINCH_HALF_DURATION,
    POKECRY_POKEDEX_CHORUS,
    POKECRY_POKEDEX,
};

struct BattleSystem;
struct OpponentData;
struct Pokepic;
struct UnkBattleSystemSub17C;

typedef struct MonEncounterData {
    struct BattleSystem *battleSys;         // 0x00
    struct OpponentData *battlerData;       // 0x04
    struct Pokepic *sprite;                 // 0x08
    struct UnkBattleSystemSub17C *terrain;  // 0x0c
    u8 command;                             // 0x10
    u8 battler;                             // 0x11
    u8 state;                               // 0x12
    u8 face;                                // 0x13
    s16 targetPos;                          // 0x14
    u16 species;                            // 0x16
    enum PokemonCryMod cryMod;              // 0x18
    int battlerType;                        // 0x1c
    int delay;                              // 0x20
    int nature;                             // 0x24
    BOOL isShiny;                           // 0x28
    u8 formNum;                             // 0x2c
    u8 padding_2D[3];
} MonEncounterData;                         // size: 0x30

#endif // POKEHEARTGOLD_OVERLAY_12_MON_ENCOUNTER_DATA_H
