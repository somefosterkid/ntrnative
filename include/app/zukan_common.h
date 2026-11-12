// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_common.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKAN_COMMON_H
#define BWPORT_APP_ZUKAN_COMMON_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

// What the Pokédex's list is searched with, 7 bytes: the order, the kind of list, and what to match: the initial's
// group, two types, a color and a shape, each 0xff for any
typedef struct {
    u8 regional;
    u8 order;
    u8 initial;
    u8 type1;
    u8 type2;
    u8 color;
    u8 shape;
} ZukanSearchCriteria;

// Black 2's, not checked against Black
// How many areas the habitat list has
#define ZUKAN_HABITAT_COUNT 57

// Black 2's, not checked against Black
// An area has four seasons and three kinds of habitat
#define ZUKAN_SEASON_COUNT 4

// Black 2's, not checked against Black
#define ZUKAN_HABITAT_TYPE_COUNT 3

// Black 2's, not checked against Black
// The habitats that the player has reached: the first is always true, and the others are flags of the story. An area
// is shown if one of the conditions that it names is true
typedef struct {
    BOOL conditions[3];
} ZukanHabitatConditions;

// Black 2's, not checked against Black
// A species in an area, in its file
typedef struct {
    u16 species;
    // Whether it is there, by season and kind of habitat
    u8 present[ZUKAN_SEASON_COUNT][ZUKAN_HABITAT_TYPE_COUNT];
    // The same, for the other half of the data
    u8 other[ZUKAN_SEASON_COUNT][ZUKAN_HABITAT_TYPE_COUNT];
    u16 unk1a;
} ZukanHabitatFileEntry;

// Black 2's, not checked against Black
// An area's file, from archive 296 and 0x352 bytes
typedef struct {
    u8 values[3];
    // The conditions that make the area shown, 1 for each
    u8 conditions[3];
    u16 unk6;
    u16 count;
    ZukanHabitatFileEntry entries[30];
} ZukanHabitatFile;

// Black 2's, not checked against Black
// A species of an area in the list that the habitat screen shows, 0x1c bytes
typedef struct {
    u16 species;
    u8 present[ZUKAN_SEASON_COUNT][ZUKAN_HABITAT_TYPE_COUNT];
    u8 other[ZUKAN_SEASON_COUNT][ZUKAN_HABITAT_TYPE_COUNT];
    // 1 if the species is not seen, 2 if it is only seen, 3 if it is caught
    u8 status;
    u8 unk1b;
} ZukanHabitatSpecies;

// Black 2's, not checked against Black
// An area in the list that the habitat screen shows, 0x364 bytes
typedef struct {
    u16 zone;
    u16 values[3];
    u32 unk8;
    ZukanHabitatSpecies species[30];
    // For each condition: 2 if all of its species are caught, 1 if all are seen, else 0
    u32 states[3];
    // 1 if every condition that applies is complete
    u32 complete;
} ZukanHabitatArea;

// Black 2's, not checked against Black
typedef struct {
    GameData *gameData;
    u32 count;
    ZukanHabitatArea areas[ZUKAN_HABITAT_COUNT];
    u32 result;
} ZukanHabitatListParam;

// Black 2's, not checked against Black
// An area in the list of the screen of the Pokédex's awards, 0x2c bytes
typedef struct {
    u16 zone;
    u8 conditions[3];
    u8 unk5;
    u32 states[3];
    // 4 for a condition that does not apply, else 1 to 3
    u32 display[3];
    u32 conditionsMet[3];
} ZukanHabitatSummary;

// Black 2's, not checked against Black
typedef struct {
    GameData *gameData;
    u8 count;
    ZukanHabitatSummary areas[ZUKAN_HABITAT_COUNT];
    u32 result;
} ZukanHabitatSummaryParam;

// Black 2's; twinned code confirms the offsets of setup, chars, palette, cells and surface in Black
// What an actor of the Pokédex's screens is created from: where it starts, its resources and its surface
typedef struct {
    ClActorSetup setup;
    u32 chars;
    u32 palette;
    u32 cells;
    u16 unk14;
    u16 surface;
} ZukanActorTemplate;

// pokebw2's ZukanCommon_SelectSeen
// Makes the Pokédex's last species one that the list has seen, if it is not
void func_ov212_021c92ec(PokeDexSave *pokedex, const u16 *list, u16 count);
// pokebw2's ZukanCommon_IsFirstShown
// Whether the list starts with its first entry, the Pokédex's first species, which it hides before it is seen
BOOL func_ov212_021c9348(PokeDexSave *pokedex);
// pokebw2's ZukanCommon_InitCriteria
void func_ov212_021c9380(PokeDexSave *pokedex, ZukanSearchCriteria *criteria);
// pokebw2's ZukanCommon_WipeIn
// Wipe in and wipe out of the Pokédex's screens
void func_ov212_021c93a8(HeapID heapId);
// pokebw2's ZukanCommon_WipeOut
void func_ov212_021c93c4(HeapID heapId);
// pokebw2's ZukanCommon_ScrollBGs
// Scrolls two BGs by a pixel every four frames
void func_ov212_021c93e4(u8 bg1, u8 bg2, int *timer);
// pokebw2's ZukanCommon_CreateActor
ClActor *func_ov212_021c940c(ClActUnit *unit, const ZukanActorTemplate *template, HeapID heapId);

// The work of the Pokédex's screens and its parts (Black's layout; the sizes are the allocations' in zukan.c)
// What the Pokédex is opened with, and the mode it returns
typedef struct {
    GameData *gameData;
    PokeDexSave *pokedex;
    PlayerInfo *playerInfo;
    u16 mode;
    u16 result;
} ZukanCommonParam;

// What the detail screen is given, 0x14 bytes
typedef struct {
    GameData *gameData;
    u32 unk_4;
    u16 *species;
    u16 count;
    u16 index;
    u32 state;
} ZukanCommonList;

typedef struct {
    ZukanCommonParam *param;
    GameProcManager *procMgr;
    BOOL running;
    u16 *list;
    u16 count;
    u16 searched;
    ZukanSearchCriteria criteria;
    u32 state;
    u32 nextState;
    u32 unk_24;
    void *subParam;
} ZukanCommonWork;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Selects the list's species, frees the list's work and returns 9 or 3, by the state the list was in
u32 func_ov212_021c9244(ZukanCommonWork *wk);
// pokebw2's ZukanCommon_CreateList
// Loads the species of the Pokédex's list, in the regional or the national order, and returns how many of its first
// entries to show: up to the last that the player has seen
u32 func_ov212_021c9288(PokeDexSave *pokedex, u16 **list, HeapID heapId);
// Loads the species of one order into a list, and returns how many there are
u32 func_ov212_021c976c(BOOL regional, HeapID heapId, u16 **list);

// The Pokédex's proc, from the field menu
extern const GameProcFunctions data_ov212_021c9938;

#endif // BWPORT_APP_ZUKAN_COMMON_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_common.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKAN_COMMON_H

#include "types.h"

// pokebw2's ZukanCommon_ScrollBGs
// Scrolls two BGs by a pixel every four frames

// pokebw2's ZukanCommon_CreateActor

#include "types.h"
#include "gfl/heap.h"

// pokebw2's ZukanCommon_WipeIn
// Wipe in and wipe out of the Pokédex's screens
// pokebw2's ZukanCommon_WipeOut

#endif // BWPORT_APP_ZUKAN_COMMON_H
