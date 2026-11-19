#ifndef POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_OPPONENT_APP_H
#define POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_OPPONENT_APP_H

#include "battle_castle_app_sprite_manager.h"
#include "g2d_renderer.h"

// BattleCastleOpponentApp: merged by tools/hg/promote.py from 11 m2c local copies (6 variants, base
// src/overlay_83_02244DF4_02244F60.c) and 470 accesses; fields at every accessed offset, pads elsewhere.

struct ApplicationManager;
struct BattleCastleAppSprite;
struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct BoxPokemon;
struct BattleFrontierSave;
struct BgConfig;
struct FontSpecialCharsContext;
struct ListMenu;
struct Menu;
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
struct SaveData;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct StringList;
struct StringTemplate;
struct Window;

typedef struct BattleCastleOpponentApp {
    struct ApplicationManager *appMan;
    struct BattleFrontierSave *frontier;
    u8 subState;
    u8 challengeType;
    u8 printerID;
    u8 unused;
    u8 previousSlot;
    u8 selectedMonSlot;
    u8 levelAdjustment;
    u8 arrowSpriteVisible : 1;
    u8 listMenuIsOpen : 1;
    u8 simpleMenuIsOpen : 1;
    u8 syncPurchaseState : 5;
    u8 syncPurchaseAction;
    u8 slotID;
    u8 selectedLevelAdjustment;
    u8 selectedMenuEntry;
    u8 numSlots;
    u8 exitSlot;
    u8 syncDelay;
    u8 msgsReceived;
    u16 menuPos;
    u16 unused2;
    u16 monMenuListPos;
    u16 monMenuCursorPos;
    struct MsgData *msgLoader;
    struct StringTemplate *strTemplate;
    struct String *displayStr;
    struct String *fmtStr;
    struct String *simpleMenuStr[3];
    u16 unused3[8];
    struct BgConfig *bgConfig;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } windows[12];
    struct { struct StringList *choices; struct Window *window; u8 fontID; u8 xSize; u8 ySize; u8 lineSpacing : 4; u8 suppressCursor : 2; u8 loopAround : 2; } simpleMenuTemplate;
    struct Menu *simpleMenu;
    struct { void *entry; u8 unk_4_anon[0x4]; } simpleStrList[3];
    struct ListMenu *listMenu;
    struct StringList *listMenuStrList;
    struct PaletteData *plttData;
    struct FontSpecialCharsContext *unk_144_;
    struct Options *unk_148;
    struct SaveData *unk_14C;
    struct BattleCastleSave *unk_150;
    struct BattleCastlePersistentSave *unk_154;
    u8 pad_158[0x158];
    u32 unk_2B0;
    struct FontSpecialCharsContext *specialChars;
    struct Options *options;
    struct SaveData *saveData;
    struct BattleCastleSave *castleSave;
    struct BattleCastlePersistentSave *persistentSave;
    BattleCastleAppSpriteManager spriteMan;
    struct BattleCastleAppSprite *monSprites[4];
    struct BattleCastleAppSprite *ballSprites[4];
    struct BattleCastleAppSprite *arrowSprite;
    struct BattleCastleAppSprite *cursorSprite;
    struct BattleCastleAppSprite *partnerCursorSprite;
    struct BattleCastleAppSprite *barSprites[4];
    struct BattleCastleAppSprite *flagSprites[4][2];
    struct BattleCastleAppSprite *sparklesSprite;
    struct BattleCastleAppSprite *selectedMonSprite; // ov83_022474C4 sprite of the mon being viewed
    u16 *selectedMonSlotOut;
    u8 *identityUnlocked;
    u8 *levelAdjustments;
    u8 *statsUnlocked;
    u8 *movesUnlocked;
    struct Party *opponentsParty;
    struct NARC *narc;
    u16 commPayload[40];
    u8 partnersSelectedSlot;
    u8 partnerSlot;
    u8 partnerIsExiting;
    u8 partnersRanks[3];
    u16 partnersCP;
    void *mon; // Pokemon *
    struct BoxPokemon *boxMon;
    u16 species;
    u8 gender : 7;
    u8 noPrintGender : 1;
    s8 unk_5C7;
    s8 unk_5C8;
    u8 nature;
    s16 unk_5CA;
    u32 personality;
    s16 stats[7];
    s8 unk_5DE;
    u16 moves[4];
    s8 movePP[4];
    u8 moveMaxPP[4];
    u32 unk_5F0;
    u32 unk_5F4;
    u32 unk_5F8;
    u32 unk_5FC;
    u32 unk_600;
    u32 unk_604;
    u32 unk_608;
    u8 pad_60C[0x8];
} BattleCastleOpponentApp;

// sizeof(BattleCastleOpponentApp) == 0x614

#endif // POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_OPPONENT_APP_H
