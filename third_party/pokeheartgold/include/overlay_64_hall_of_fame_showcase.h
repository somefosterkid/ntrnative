#ifndef POKEHEARTGOLD_OVERLAY_64_HALL_OF_FAME_SHOWCASE_H
#define POKEHEARTGOLD_OVERLAY_64_HALL_OF_FAME_SHOWCASE_H

#include "bg_window.h"
#include "hall_of_fame.h"

// HallOfFameShowcase: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_64_021E5A18.c) and 141 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct GridInputHandler;
struct HallOfFame;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NARC;
struct SpriteManager;
struct SpriteSystem;
struct String;

typedef struct HallOfFameShowcase {
    struct HallOfFame *hallOfFame;
    struct BgConfig *bgConfig;
    Window windows[17];
    struct MsgData *msgData;
    struct MsgData *speciesNames;
    struct MsgData *moveNames;
    struct MessageFormat *msgFormat;
    struct String *stringBuf;
    struct String *stringBuf2;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprites[18];
    struct GridInputHandler *gridInput;
    struct NARC *pokegraNarc;
    SHOW_HOFMON mon;
    struct String *nickname;
    struct String *otName;
    u32 numMons;
    int recordNum;
    int record;
    int numRecords;
    u32 pressedButton;
    u32 buttonAnimTimer;
    u16 monDetailSet;
    u16 iconSet;
    int state;
    int nextState;
} HallOfFameShowcase;

// sizeof(HallOfFameShowcase) == 0x1d0

#endif // POKEHEARTGOLD_OVERLAY_64_HALL_OF_FAME_SHOWCASE_H
