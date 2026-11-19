#ifndef POKEHEARTGOLD_EVOLUTION_DATA_H
#define POKEHEARTGOLD_EVOLUTION_DATA_H

#include "constants/heap.h"

#include "bg_window.h"
#include "pokemon_types_def.h"
#include "save_layout.h"

// EvolutionData: merged by tools/hg/promote.py from 7 m2c local copies (2 variants, base
// src/unk_020755E8_02075D4C_02075E14.c) and 110 accesses; fields at every accessed offset, pads elsewhere.
// The evolution task's work area (the constructor sub_02075A7C allocates and fills it, the destructor sub_02075D4C
// frees it): every member is reached by name, never by a raw DS offset, so a host with 8-byte pointers lays it out
// itself. The comments give the DS offsets.

struct Bag;
struct BgConfig;
struct GF3DVramMan;
struct GameStats;
struct MessageFormat;
struct MsgData;
struct NARC;
struct Options;
struct OverlayManager;
struct PaletteData;
struct Party;
struct Pokedex;
struct Pokepic;
struct PokepicAnimManager;
struct PokepicManager;
struct PokemonSummaryArgs;
struct ManagedSprite;
struct Poketch;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct UnkStruct_020163E0;

typedef struct EvolutionData {
    struct BgConfig *bgConfig;                   // 0x00
    struct Window *window;                       // 0x04
    struct MsgData *msgLoader;                   // 0x08
    struct MessageFormat *strTemplate;           // 0x0C
    struct String *string;                       // 0x10
    struct PaletteData *paletteData;             // 0x14
    struct PokepicManager *monSpriteMan;         // 0x18
    struct Pokepic *monSprites[2];               // 0x1C
    struct Party *party;                         // 0x24
    Pokemon *mon;                                // 0x28
    struct Options *options;                     // 0x2C
    void *unk_30;                                // 0x30
    struct GF3DVramMan *pipelineBuffers;         // 0x34
    struct OverlayManager *summaryScreenAppMan;  // 0x38
    struct PokemonSummaryArgs *monSummary;       // 0x3C
    void *menu;                                  // 0x40
    struct PokepicAnimManager *monAnimMan;       // 0x44
    struct Pokedex *pokedex;                     // 0x48
    struct Bag *bag;                             // 0x4C
    struct GameStats *records;                   // 0x50
    struct Poketch *poketch;                     // 0x54
    struct UnkStruct_020163E0 *unk_58;           // 0x58
    enum HeapID heapID;                          // 0x5C
    u16 currentSpecies;                          // 0x60
    u16 targetSpecies;                           // 0x62
    u8 state;                                    // 0x64
    u8 printerID;                                // 0x65
    u8 delay;                                    // 0x66
    u8 done;                                     // 0x67
    int levelUpMoveIndex;                        // 0x68
    u16 moveID;                                  // 0x6C
    u8 moveSlot;                                 // 0x6E
    u8 unused;                                   // 0x6F
    u8 shrinkAndGrowState;                       // 0x70
    u8 attributeDelta;                           // 0x71
    u8 windowLeft;                               // 0x72
    u8 windowTop;                                // 0x73
    u8 windowRight;                              // 0x74
    u8 windowBottom;                             // 0x75
    u8 pad_76[0x2];                              // 0x76
    int method;                                  // 0x78
    int flags;                                   // 0x7C
    u8 form;                                     // 0x80
    u8 pad_81[0x3];                              // 0x81
    struct NARC *narc;                           // 0x84
    u8 pad_88[0x2];                              // 0x88 (the two animation ids sub_020729A4 writes)
    u8 unk_8A;                                   // 0x8A
    u8 unk_8B;                                   // 0x8B
    Window windows[2];                           // 0x8C (0x10 bytes each on the DS)
    struct SpriteSystem *unk_AC;                 // 0xAC
    struct SpriteManager *unk_B0;                // 0xB0
    struct ManagedSprite *unk_B4;                // 0xB4
    BOOL unk_B8;                                 // 0xB8 (TRUE while the sprite system is up)
} EvolutionData;

// sizeof(EvolutionData) == 0xbc on the DS (a host with 8-byte pointers has a bigger one: the allocation uses sizeof)
#ifdef __MWERKS__
LAYOUT_CHECK(EvolutionData_size, sizeof(EvolutionData) == 0xBC);
#endif

#endif // POKEHEARTGOLD_EVOLUTION_DATA_H
