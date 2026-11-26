#ifndef POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_OPPONENT_APP_H
#define POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_OPPONENT_APP_H

#include "battle_castle_app_sprite_manager.h"
#include "bg_window.h"
#include "overlay_83_battle_castle_yes_no_prompt.h"
#include "g2d_renderer.h"

// BattleCastleOpponentApp: merged by tools/hg/promote.py from 11 m2c local copies (6 variants, base
// src/overlay_83_02244DF4_02244F60.c) and 470 accesses; fields at every accessed offset, pads elsewhere.

struct ApplicationManager;
struct GridInputHandler;
struct ListMenuItem;
struct TouchscreenListMenu;
struct TouchscreenListMenuSpawner;
struct BattleCastleAppSprite;
struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct BoxPokemon;
struct FrontierSave;
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
struct MessageFormat;
struct StringTemplate;

typedef struct BattleCastleOpponentApp {
    struct ApplicationManager *appMan;
    struct FrontierSave *frontier;
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
    struct MessageFormat *strTemplate;
    struct String *displayStr;
    struct String *fmtStr;
    struct String *simpleMenuStr[3];
    u16 unused3[8];
    struct BgConfig *bgConfig;
    Window windows[36];
    u8 pad_290[0x20];
    struct PaletteData *plttData;
    struct MessagePrinter *msgPrinter;
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
    u8 level;
    u8 ability;
    u8 nature;
    u16 heldItem;
    u32 personality;
    u16 stats[7];
    s8 unk_5DE;
    u16 moves[4];
    u8 movePP[4];
    u8 moveMaxPP[4];
    struct GridInputHandler *partyGridInput;
    struct TouchscreenListMenuSpawner *touchMenuSpawner;
    struct TouchscreenListMenu *touchMenu;
    struct ListMenuItem *listMenuItems;
    s32 listSelection;
    BattleCastleYesNoPrompt yesNoPrompt;
    u8 pad_610[0x4];
} BattleCastleOpponentApp;

// sizeof(BattleCastleOpponentApp) == 0x614

#endif // POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_OPPONENT_APP_H
