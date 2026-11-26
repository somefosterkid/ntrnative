#ifndef POKEHEARTGOLD_OVERLAY_82_BATTLE_HALL_APP_H
#define POKEHEARTGOLD_OVERLAY_82_BATTLE_HALL_APP_H

#include "bg_window.h"
#include "g2d_renderer.h"

// BattleHallApp: merged by tools/hg/promote.py from 12 m2c local copies (5 variants, base
// src/overlay_82_0223E9E8_0223EC0C_0223EC0C.c) and 213 accesses; fields at every accessed offset, pads elsewhere.

struct ApplicationManager;
struct BattleHallAppSprite;
struct BgConfig;
struct FontSpecialCharsContext;
struct MsgData;
struct NARC;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct Options;
struct PaletteData;
struct Party;
struct PokemonSummary;
struct SaveData;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct StringTemplate;

typedef struct BattleHallAppSpriteManager {
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resourceCollection[4];
    struct SpriteResource *resources[2][4];
} BattleHallAppSpriteManager;

typedef struct BattleHallApp {
    struct ApplicationManager *unused;
    struct ApplicationManager *monSummaryAppMan;
    u8 subState;
    u8 challengeType;
    u8 printerID;
    u8 summaryScreenOpened;
    u8 prevCursorPos;
    u8 cursorPos;
    u8 unused2;
    u8 yesNoMenuOpen;
    u16 summaryScreenOpen;
    u16 isSynced;
    u16 openMenuDelay;
    u8 numSelectionMsgsReceived;
    u8 startMultiSelectionConfirm;
    u8 selectionID;
    u8 selectionCanceled;
    u8 selectedCursorPos;
    u8 fadeOutDelay;
    u16 currentStreak;
    u8 matronBattleAvailable;
    u8 savedCursorPos;
    struct MsgData *msgLoader;
    struct StringTemplate *strTemplate;
    struct String *displayStr;
    struct String *fmtStr;
    struct String *yesNoStrs[2];
    u16 unused3[8];
    struct BgConfig *bgConfig;
    Window windows[4];
    u32 unk_8C;
    u8 pad_90[0x4];
    struct PaletteData *plttData;
    struct FontSpecialCharsContext *specialChars;
    struct Options *options;
    struct SaveData *saveData;
    struct PokemonSummary *monSummary;
    BattleHallAppSpriteManager spriteMan;
    struct BattleHallAppSprite *cursorSprite;
    struct BattleHallAppSprite *monSprite;
    int monIndex;
    u16 *selectedCellPtr;
    struct Party *party;
    u8 *typeRanks;
    u16 *partnersLevel;
    struct NARC *narc;
    u16 commPayload[44];
    u8 partnersSelectedCell;
    u8 partnersDecision;
    u8 unused4;
    u8 pad_27F[0x1];
    u32 unused5;
    u8 pad_284[0x18];
} BattleHallApp;

// sizeof(BattleHallApp) == 0x29c

#endif // POKEHEARTGOLD_OVERLAY_82_BATTLE_HALL_APP_H
