#ifndef POKEHEARTGOLD_OVERLAY_81_BATTLE_FACTORY_APP_H
#define POKEHEARTGOLD_OVERLAY_81_BATTLE_FACTORY_APP_H

#include "bg_window.h"
#include "g2d_renderer.h"
#include "overlay_81_battle_factory_app_args.h"

// BattleFactoryApp: merged by tools/hg/promote.py from 11 m2c local copies (6 variants, base
// src/overlay_81_02240F48_02241020_0224102C.c) and 889 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
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
struct Sprite;
struct SpriteList;
struct GF_2DGfxResMan;
struct SpriteResource;
struct String;
struct ListMenuItem;
struct GridInputHandler;
struct YesNoPrompt;
struct UnkStruct_0201956C;
struct PokemonSummaryArgs;
struct GF3DVramMan;
struct Pokepic;
struct PokepicManager;
struct ListMenu2D;
struct OverlayManager;
struct MsgData;
struct MessageFormat;

// Sprite wrappers of ov81_02242CBC/ov81_02242D18 (0xC) and ov81_02242E50 (0x10).
typedef struct BattleFactoryAppPanelSprite {
    int x;
    int y;
    struct Sprite *sprite;
} BattleFactoryAppPanelSprite;

typedef struct BattleFactoryAppPokeballSprite {
    u16 isSelected;
    u16 unused;
    int restingX;
    int restingY;
    struct Sprite *sprite;
} BattleFactoryAppPokeballSprite;

// resources[set][char, pltt, cell, anim]: ov81_0224276C loads set 1 at +0x14C
typedef struct BattleFactoryAppSpriteManager {
    struct SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    struct GF_2DGfxResMan *resourceCollection[4];
    struct SpriteResource *resources[6][4];
} BattleFactoryAppSpriteManager;

// Palette blink of a tilemap rectangle (ov81_0224258C / ov81_022425EC)
typedef struct BattleFactoryAppBlink {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 count;
    u8 timer;
    u8 palSwap;
    u8 nextSubState;
} BattleFactoryAppBlink;

typedef struct BattleFactoryApp {
    struct OverlayManager *appMan;
    struct OverlayManager *monSummaryAppMan;
    u8 subState;
    u8 challengeType;
    u8 isOpenLevel;
    u8 isExchangeMode;
    int conveyorXOffset;
    u8 printerID;
    u8 numMonsSelected;
    u8 numPokeballs;
    u8 menuWasCancelled : 1;
    u8 summaryScreenOpened : 1;
    u8 exchangeMonSelected : 1;
    u8 tradedMon : 1;
    u8 firstTimeSetupDone : 1;
    u8 clearPartnerInfo : 1;
    u8 summaryScreenOpen : 1;
    u8 unused : 1;
    u32 wheelPaletteCounter;
    u8 partnersNumSelectedMons;
    u8 animationTimer;
    u8 exchangeSelectCursorPositions;
    u8 numReceiveSelectCursorPositions;
    struct MsgData *msgLoader;
    struct MessageFormat *strTemplate;
    struct String *displayStr;
    struct String *fmtStr;
    struct String *menuStr[4];
    u16 unused2[8];
    struct BgConfig *bgConfig;
    Window windows[18]; // 0x50, added by ov81_02242FDC
    u8 unk_170[4];
    Window *unk_174;
    u8 unk_178[4];
    struct ListMenu2D *menu;
    u8 unk_180[0x20];
    struct PaletteData *plttData;
    struct GF3DVramMan *g3dPipeline;
    struct PokepicManager *monSpriteMan;
    struct Pokepic *monSprites[3];
    struct Options *options;
    struct SaveData *saveData;
    struct PokemonSummaryArgs *monSummary;
    BattleFactoryAppSpriteManager spriteMan; // 0x1c4
    BattleFactoryAppPokeballSprite *ballSprites[6];
    u8 unk_378[4];
    BattleFactoryAppPokeballSprite *unk_37C;
    BattleFactoryAppPanelSprite *unk_380;
    BattleFactoryAppPanelSprite *unk_384;
    BattleFactoryAppPanelSprite *unk_388;
    BattleFactoryAppPanelSprite *unk_38C;
    BattleFactoryAppPanelSprite *unk_390;
    BattleFactoryAppPanelSprite *monIconSprites[2]; // 0x394, indexed by unk_480
    BattleFactoryAppPanelSprite *panelSprites[3]; // 0x39c, numMonsToFade of them
    BattleFactoryAppPanelSprite *unk_3A8[6];
    struct Party *personalParty;
    struct Party *receivableParty;
    u16 selectedIndices[6];
    u16 *selectedIndicesFinal;
    int startingSlot;
    struct NARC *narc;
    u16 commPayload[60];
    u16 partnerTradedMon;
    u16 partnerMonSpecies[2];
    u16 partnenMonGenders[2];
    u8 numTradeResultMsgReceived;
    u8 unk_463;
    struct GridInputHandler *inputHandler;
    s32 unk_468;
    struct YesNoPrompt *yesNoPrompt;
    u8 pad_470[0x4];
    struct UnkStruct_0201956C *unk_474;
    u32 unk_478;
    u32 numMonsToFade;
    s32 unk_480;
    BattleFactoryAppBlink blink; // 0x484, ov81_0224258C sets it up, ov81_022425EC runs it
} BattleFactoryApp;

// sizeof(BattleFactoryApp) == 0x48C (OverlayManager_CreateAndGetData in ov81_0223DD60)

#endif // POKEHEARTGOLD_OVERLAY_81_BATTLE_FACTORY_APP_H
