#ifndef POKEHEARTGOLD_POKEMON_SUMMARY_APP_H
#define POKEHEARTGOLD_POKEMON_SUMMARY_APP_H

#include "bg_window.h"
#include "pokepic_anim_script.h"

// PokemonSummary: merged by tools/hg/promote.py from 18 m2c local copies (1 variants, base
// src/unk_02088288_02088424.c) and 909 accesses; fields at every accessed offset, pads elsewhere.

struct UnkStruct_0201956C;
struct BgConfig;
struct Camera;
struct MessageFormat;
struct MessagePrinter;
struct MsgData;
struct NARC;
struct PokemonSummaryArgs;
struct Pokepic;
struct PokepicAnimManager;
struct PokepicManager;
struct Sprite;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct Window;

typedef struct PokemonSummaryMonData {
    struct String *speciesName;
    struct String *nickname;
    struct String *otName;
    u16 species;
    u16 heldItem;
    u8 type1;
    u8 type2;
    u8 level : 7;
    u8 noPrintGender : 1;
    u8 gender : 2;
    u8 pokeball : 6;
    u32 otId;
    u32 exp;
    u32 curLevelExp;
    u32 nextLevelExp;
    u16 hp;
    u16 maxHp;
    u16 attack;
    u16 defense;
    u16 spAttack;
    u16 spDefense;
    u16 speed;
    u8 ability;
    u8 nature;
    u16 moves[4];
    u8 pp[4];
    u8 maxPP[4];
    u8 otGender;
    u8 contestStats[6];
    u8 favoriteFlavor;
    u16 markings;
    u16 form;
    u32 status : 28;
    u32 isEgg : 1;
    u32 shiny : 1;
    u32 pokerus : 2;
    u32 ribbons[4];
    u8 shinyLeaves[6];
} PokemonSummaryMonData;

typedef struct PokemonSummary {
    struct BgConfig *bgConfig;
    struct Window windows[34];
    struct Window *pageWindows;
    u32 numPageWindows;
    struct PokemonSummaryArgs *args;
    PokemonSummaryMonData mon;
    struct Camera *camera;
    struct PokepicManager *pokepicManager;
    PokepicAnimScript pokepicAnimScript[10];
    struct PokepicAnimManager *pokepicAnimManager;
    struct Pokepic *pokepic;
    u32 pokepicAnimReverseX;
    struct { struct { s16 x; s16 y; s16 z; } vertices[4]; } conditionChartFrom[4];
    struct { struct { s16 x; s16 y; s16 z; } vertices[4]; } conditionChartStep[4];
    struct { struct { s16 x; s16 y; s16 z; } vertices[4]; } conditionChartTo[4];
    u32 conditionChartDone;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct Sprite *sprites[115];
    u8 unk5D0[460];
    struct MessagePrinter *numberPrinter;
    struct MsgData *msgData;
    struct MsgData *ribbonMsgData;
    struct MessageFormat *msgFormat;
    struct String *buffer;
    struct String *playerName;
    struct MsgData *moveNames;
    struct NARC *pokepicAnimNarc;
    s8 page;
    u8 moveSlot : 4;
    u8 swapMoveSlot : 4;
    u8 unk7BE;
    u8 unk7BF_0 : 4;
    u8 exitRequested : 4;
    u8 sparkleState;
    u8 numSparkles;
    u8 unk7C2;
    u8 unk7C3;
    u8 ribbonCursor;
    u8 ribbonRow;
    u8 ribbonCount;
    u8 ribbonId;
    u8 flashX;
    u8 flashY;
    u8 flashWidth;
    u8 flashHeight;
    u8 flashPalette : 4;
    u8 restorePalette : 4;
    u8 flashBgId;
    u8 flashTimer;
    u8 flashNextState;
    struct UnkStruct_0201956C *cellManager;
    u32 boxIconSprite;
} PokemonSummary;

// sizeof(PokemonSummary) == 0x7d8

#endif // POKEHEARTGOLD_POKEMON_SUMMARY_APP_H
