#ifndef POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_SELF_APP_H
#define POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_SELF_APP_H

#include "battle_castle_app_sprite_manager.h"
#include "bg_window.h"
#include "g2d_renderer.h"
#include "overlay_83_battle_castle_yes_no_prompt.h"

// BattleCastleSelfApp: merged by tools/hg/promote.py from 10 m2c local copies (3 variants, base
// src/overlay_83_0223DE60.c) and 297 accesses; fields at every accessed offset, pads elsewhere.

struct OverlayManager;
struct BattleCastleAppSprite;
struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct FrontierSave;
struct BgConfig;
struct BoxPokemon;
struct GF_2DGfxResMan;
struct GridInputHandler;
struct ListMenuItem;
struct MessagePrinter;
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
struct TouchscreenListMenu;
struct TouchscreenListMenuSpawner;
struct Window;
struct YesNoPrompt;

enum BattleCastleSelfAppWindow {
    SELF_APP_WINDOW_HEADER = 0,
    SELF_APP_WINDOW_EXIT,
    SELF_APP_WINDOW_LEVELS,
    SELF_APP_WINDOW_HP_BARS,
    SELF_APP_WINDOW_SUMMARY_SCREEN,
    SELF_APP_WINDOW_ITEM_SELECT_MENU,
    SELF_APP_WINDOW_MSG_BOX,
    SELF_APP_WINDOW_MON_SELECTION_MSG_BOX,
    SELF_APP_WINDOW_MON_OPTIONS_MSG_BOX,
    SELF_APP_WINDOW_MON_OPTIONS_MENU,
    SELF_APP_WINDOW_HEAL_MENU,
    SELF_APP_WINDOW_RENTAL_MENU,
    SELF_APP_WINDOW_SELECTED_ITEM_NAME,
    SELF_APP_WINDOW_YES_NO_MENU,
    SELF_APP_WINDOW_UNUSED,
    SELF_APP_WINDOW_ITEM_SELECT_PLAYER_INFO,
    SELF_APP_WINDOW_ITEM_SELECT_MON_INFO,
    SELF_APP_WINDOW_ITEM_SELECT_MON_HP,
    NUM_SELF_APP_WINDOWS,
};

#define SELF_APP_WINDOW_TEMPLATE_COUNT 70

// One row (0xC bytes) of the mon/item option menus built in ov83_02242BF0 / ov83_02242CAC.
typedef struct BattleCastleMenuEntry {
    u32 minRank;
    s32 msgID;
    s32 action;
} BattleCastleMenuEntry;

typedef struct BattleCastleSelfApp {
    struct OverlayManager *appMan;
    struct FrontierSave *frontier;
    u8 subState;
    u8 challengeType;
    u8 printerID;
    u8 syncDelay;
    u8 previousSlot;
    u8 selectedMonSlot;
    u8 listMenuIsOpen : 1;
    u8 syncPurchaseAction : 1;
    u8 monSparklesVisible : 1;
    u8 itemSelectIsOpen : 1;
    u8 yesNoMenuIsOpen : 1;
    u8 syncIncreaseRank : 2;
    u8 unused : 1;
    u8 msgsReceived;
    u16 selectedRentalItem;
    u8 slotID;
    u8 selectedMenuEntry;
    u8 numSlots;
    u8 exitSlot;
    u16 menuPos;
    u16 monMenuListPos;
    u16 monMenuCursorPos;
    struct MsgData *itemMsgLoader;
    struct MsgData *msgLoader;
    struct MessageFormat *strTemplate;
    struct String *displayStr;
    struct String *fmtStr;
    struct String *yesNoStrs[3];
    u16 unused2[8];
    struct BgConfig *bgConfig;
    Window windows[SELF_APP_WINDOW_TEMPLATE_COUNT]; // 0x50: 70 templates in ov83_0224819C[0]; the enum names the first 18
    struct { struct StringList *choices; struct Window *window; u8 fontID; u8 xSize; u8 ySize; u8 lineSpacing : 4; u8 suppressCursor : 2; u8 loopAround : 2; } yesNoMenuTemplate;
    struct Menu *yesNoMenu;
    struct { void *entry; u8 unk_4_anon[0x4]; } yesNoStrList[3];
    struct ListMenu *listMenu;
    struct ListMenuItem *listItems;
    struct { struct StringList *choices; void *cursorCallback; void *printCallback; struct Window *window; u16 count; u16 maxDisplay; u8 headerXOffset; u8 textXOffset; u8 cursorXOffset; u8 yOffset : 4; u8 textColorFg : 4; u8 textColorBg : 4; u8 textColorShadow : 4; u16 letterSpacing : 3; u16 lineSpacing : 4; u16 pagerMode : 2; u16 fontID : 6; u16 cursorType : 1; void *parent; } listTemplate;
    struct PaletteData *plttData;
    struct MessagePrinter *msgPrinter;
    struct Options *options;
    struct SaveData *saveData;
    struct BattleCastleSave *castleSave;
    struct BattleCastlePersistentSave *persistentSave;
    BattleCastleAppSpriteManager spriteMan;
    struct BattleCastleAppSprite *upArrowSprite;
    struct BattleCastleAppSprite *downArrowSprite;
    struct BattleCastleAppSprite *monIconSprites[4];
    struct BattleCastleAppSprite *itemIconSprites[4];
    struct BattleCastleAppSprite *monSparklesSprite;
    struct BattleCastleAppSprite *cursorSprite;
    struct BattleCastleAppSprite *partnerCursorSprite;
    struct BattleCastleAppSprite *barSprites[4];
    struct BattleCastleAppSprite *bigSparkles;
    struct BattleCastleAppSprite *itemSelectCursorSprite;
    struct BattleCastleAppSprite *selectedMonSprite;
    struct BattleCastleAppSprite *listItemSprites[6];
    struct BattleCastleAppSprite *subScreenSprite;
    u16 *selectedMonSlotOut; // BattleCastleAppArgs.selectedMonSlot (args + 0x20)
    struct Party *party;
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
    s8 unk_810;
    u8 nature;
    u16 heldItem;
    u32 personality;
    u16 stats[7];
    s8 unk_826;
    u16 moves[4];
    s8 movePP[4];
    u8 moveMaxPP[4];
    struct GridInputHandler *partyGridInput;
    struct TouchscreenListMenuSpawner *touchMenuSpawner;
    struct TouchscreenListMenu *touchMenu;
    struct ListMenuItem *listMenuItems;
    s32 listSelection;
    BattleCastleYesNoPrompt yesNoPrompt;
    u8 pad_858[0x4];
    struct GridInputHandler *gridInput;
    s8 unk_860;
    u8 numEntries;
    s16 page;
    u16 highlightIdx;
    u8 highlightState;
    u8 highlightTimer;
    s32 unk_868;
} BattleCastleSelfApp;

// sizeof(BattleCastleSelfApp) == 0x86C (the OverlayManager_CreateAndGetData size in ov83_0223DD60)

#endif // POKEHEARTGOLD_OVERLAY_83_BATTLE_CASTLE_SELF_APP_H
