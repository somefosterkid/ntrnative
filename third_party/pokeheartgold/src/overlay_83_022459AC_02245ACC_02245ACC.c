#include "global.h"

#include "error_handling.h"
#include "options.h"
#include "overlay_80_02237A70.h"
#include "player_avatar.h"
#include "unk_02030A98.h"
#include "unk_02035900.h"
#include "unk_0205BFF0.h"
#include "battle_castle_app_sprite_manager.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "battle_castle_save.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BattleCastleAppSprite;

struct BattleCastleOpponentApp;

struct BattleCastlePersistentSave;

struct BattleCastleSave;

struct BattleFrontierSave;

struct BgConfig;

struct ColoredArrow;

struct FontSpecialCharsContext;

struct G2dRenderer;

struct ListMenu;

struct ListMenuTemplate;

struct Menu;

struct MenuTemplate;

struct MessageLoader;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct StringList;

struct StringTemplate;

struct Window;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

enum BattleFrontierStatsIndex {
    STAT_TOWER_RECORD_STREAK_SINGLE = 0,
    STAT_TOWER_LATEST_STREAK_SINGLE,
    STAT_TOWER_RECORD_STREAK_DOUBLE,
    STAT_TOWER_LATEST_STREAK_DOUBLE,
    STAT_TOWER_RECORD_STREAK_MULTI,
    STAT_TOWER_LATEST_STREAK_MULTI,
    STAT_TOWER_RECORD_STREAK_LINK_MULTI,
    STAT_TOWER_LATEST_STREAK_LINK_MULTI,
    STAT_TOWER_RECORD_STREAK_WIFI,
    STAT_TOWER_LATEST_STREAK_WIFI,
    STAT_FACTORY_RECORD_STREAK_50_SINGLE,
    STAT_FACTORY_LATEST_STREAK_50_SINGLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_SINGLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_SINGLE,
    STAT_FACTORY_RECORD_STREAK_OPEN_SINGLE,
    STAT_FACTORY_LATEST_STREAK_OPEN_SINGLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_SINGLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_SINGLE,
    STAT_FACTORY_RECORD_STREAK_50_DOUBLE,
    STAT_FACTORY_LATEST_STREAK_50_DOUBLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_DOUBLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_DOUBLE,
    STAT_FACTORY_RECORD_STREAK_OPEN_DOUBLE,
    STAT_FACTORY_LATEST_STREAK_OPEN_DOUBLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_DOUBLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_DOUBLE,
    STAT_FACTORY_RECORD_STREAK_50_MULTI,
    STAT_FACTORY_LATEST_STREAK_50_MULTI,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_MULTI,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_MULTI,
    STAT_FACTORY_RECORD_STREAK_OPEN_MULTI,
    STAT_FACTORY_LATEST_STREAK_OPEN_MULTI,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_MULTI,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_MULTI,
    STAT_HALL_CURRENT_STREAK_SINGLE,
    STAT_HALL_LATEST_STREAK_SINGLE,
    STAT_HALL_LATEST_SPECIES_SINGLE,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_SINGLE,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_SINGLE,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_SINGLE,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_SINGLE,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_SINGLE,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_SINGLE,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_SINGLE,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_SINGLE,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_SINGLE,
    STAT_HALL_CURRENT_STREAK_DOUBLE,
    STAT_HALL_LATEST_STREAK_DOUBLE,
    STAT_HALL_LATEST_SPECIES_DOUBLE,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_DOUBLE,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_DOUBLE,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_DOUBLE,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_DOUBLE,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_DOUBLE,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_DOUBLE,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_DOUBLE,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_DOUBLE,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_DOUBLE,
    STAT_HALL_CURRENT_STREAK_MULTI,
    STAT_HALL_LATEST_STREAK_MULTI,
    STAT_HALL_LATEST_SPECIES_MULTI,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_MULTI,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_MULTI,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_MULTI,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_MULTI,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_MULTI,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_MULTI,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_MULTI,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_MULTI,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_MULTI,
    STAT_CASTLE_RECORD_STREAK_SINGLE,
    STAT_CASTLE_LATEST_STREAK_SINGLE,
    STAT_CASTLE_LATEST_CP_SINGLE,
    STAT_CASTLE_SPENT_CP_SINGLE,
    STAT_CASTLE_RECORD_CP_SINGLE,
    STAT_CASTLE_RANK_HEALING_SINGLE,
    STAT_CASTLE_RANK_RENTALS_SINGLE,
    STAT_CASTLE_RANK_OPP_SUMMARY_SINGLE,
    STAT_CASTLE_RECORD_STREAK_DOUBLE,
    STAT_CASTLE_LATEST_STREAK_DOUBLE,
    STAT_CASTLE_LATEST_CP_DOUBLE,
    STAT_CASTLE_SPENT_CP_DOUBLE,
    STAT_CASTLE_RECORD_CP_DOUBLE,
    STAT_CASTLE_RANK_HEALING_DOUBLE,
    STAT_CASTLE_RANK_RENTALS_DOUBLE,
    STAT_CASTLE_RANK_OPP_SUMMARY_DOUBLE,
    STAT_CASTLE_RECORD_STREAK_MULTI,
    STAT_CASTLE_LATEST_STREAK_MULTI,
    STAT_CASTLE_LATEST_CP_MULTI,
    STAT_CASTLE_SPENT_CP_MULTI,
    STAT_CASTLE_RECORD_CP_MULTI,
    STAT_CASTLE_RANK_HEALING_MULTI,
    STAT_CASTLE_RANK_RENTALS_MULTI,
    STAT_CASTLE_RANK_OPP_SUMMARY_MULTI,
    STAT_ARCADE_RECORD_STREAK_SINGLE,
    STAT_ARCADE_LATEST_STREAK_SINGLE,
    STAT_ARCADE_RECORD_STREAK_DOUBLE,
    STAT_ARCADE_LATEST_STREAK_DOUBLE,
    STAT_ARCADE_RECORD_STREAK_MULTI,
    STAT_ARCADE_LATEST_STREAK_MULTI,

    STATS_NUM_SOLO_STATS,

    STAT_TOWER_WFC_STREAK_ACTIVE = STATS_NUM_SOLO_STATS,
    STAT_TOWER_WFC_STREAK_ACTIVE_2,
    STAT_FACTORY_50_WFC_STREAK_ACTIVE,
    STAT_FACTORY_50_WFC_STREAK_ACTIVE_2,
    STAT_FACTORY_OPEN_WFC_STREAK_ACTIVE,
    STAT_FACTORY_OPEN_WFC_STREAK_ACTIVE_2,
    STAT_HALL_WFC_STREAK_ACTIVE,
    STAT_HALL_WFC_STREAK_ACTIVE_2,
    STAT_CASTLE_WFC_STREAK_ACTIVE,
    STAT_CASTLE_WFC_STREAK_ACTIVE_2,
    STAT_ARCADE_WFC_STREAK_ACTIVE,
    STAT_ARCADE_WFC_STREAK_ACTIVE_2,

    STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS,

    STAT_TOWER_RECORD_STREAK_MODE_6 = STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS,
    STAT_TOWER_LATEST_STREAK_MODE_6,
    STAT_FACTORY_RECORD_STREAK_50_MULTI_WFC,
    STAT_FACTORY_LATEST_STREAK_50_MULTI_WFC,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_MULTI_WFC,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_MULTI_WFC,
    STAT_FACTORY_RECORD_STREAK_OPEN_MULTI_WFC,
    STAT_FACTORY_LATEST_STREAK_OPEN_MULTI_WFC,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_MULTI_WFC,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_MULTI_WFC,
    STAT_HALL_CURRENT_STREAK_MULTI_WFC,
    STAT_HALL_LATEST_STREAK_MULTI_WFC,
    STAT_HALL_LATEST_SPECIES_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_MULTI_WFC,
    STAT_CASTLE_RECORD_STREAK_MULTI_WFC,
    STAT_CASTLE_LATEST_STREAK_MULTI_WFC,
    STAT_CASTLE_LATEST_CP_MULTI_WFC,
    STAT_CASTLE_SPENT_CP_MULTI_WFC,
    STAT_CASTLE_RECORD_CP_MULTI_WFC,
    STAT_CASTLE_RANK_HEALING_MULTI_WFC,
    STAT_CASTLE_RANK_RENTALS_MULTI_WFC,
    STAT_CASTLE_RANK_OPP_SUMMARY_MULTI_WFC,
    STAT_ARCADE_RECORD_STREAK_MULTI_WFC,
    STAT_ARCADE_LATEST_STREAK_MULTI_WFC,

    STATS_NUM_STATS_TOTAL,
};

enum PaddingMode {
    PADDING_MODE_NONE = 0,
    PADDING_MODE_SPACES,
    PADDING_MODE_ZEROES,
};

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

typedef struct StringTemplate StringTemplate;



typedef struct BattleFrontierSave BattleFrontierSave;

u8 ov84_0223EC88(u8 exitSlot, u8 slotID);

u8 ov83_0224777C(SaveData *saveData, u8 challengeType, u8 rankType);

void BattleCastleApp_SetPartnerName(StringTemplate *template, u32 idx);

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

enum BattleCastleOpponentAppWindow {
    OPPONENT_APP_WINDOW_HEADER = 0,
    OPPONENT_APP_WINDOW_EXIT,
    OPPONENT_APP_WINDOW_LEVELS,
    OPPONENT_APP_WINDOW_HP_BARS,
    OPPONENT_APP_WINDOW_SUMMARY_SCREEN,
    OPPONENT_APP_WINDOW_MON_OPTIONS_MENU,
    OPPONENT_APP_WINDOW_SUMMARY_MENU,
    OPPONENT_APP_WINDOW_MSG_BOX,
    OPPONENT_APP_WINDOW_MON_SELECTION_MSG_BOX,
    OPPONENT_APP_WINDOW_MON_OPTION_MSG_BOX,
    OPPONENT_APP_WINDOW_YES_NO_MENU,
    OPPONENT_APP_WINDOW_LEVEL_MENU,
    NUM_OPPONENT_APP_WINDOWS,
};

void ov83_02247944(Window *window, int frame);

enum Font {
    FONT_SYSTEM = 0,
    FONT_MESSAGE,
    FONT_SUBSCREEN,
    FONT_UNOWN,

    FONT_MAX,
};

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

typedef struct ColoredArrow ColoredArrow;

typedef struct MessageLoader MessageLoader;

typedef struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} StringList;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct MenuTemplate {
    StringList *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} MenuTemplate;

typedef struct Menu Menu;

void ov80_02237FA4(BattleFrontierSave *frontier, u8 challengeType, int castlePoints);

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

typedef struct BattleCastleOpponentApp {
    ApplicationManager *appMan;
    BattleFrontierSave *frontier;
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
    MessageLoader *msgLoader;
    StringTemplate *strTemplate;
    String *displayStr;
    String *fmtStr;
    String *simpleMenuStr[3];
    u16 unused3[8];
    BgConfig *bgConfig;
    Window windows[NUM_OPPONENT_APP_WINDOWS];
    MenuTemplate simpleMenuTemplate;
    Menu *simpleMenu;
    struct { void *entry; u8 unk_4_anon[4]; } simpleStrList[3];
    ListMenu *listMenu;
    StringList *listMenuStrList;
    PaletteData *plttData;
    FontSpecialCharsContext *unk_144_;
    struct Options *unk_148;
    struct SaveData *unk_14C;
    struct BattleCastleSave *unk_150;
    struct BattleCastlePersistentSave *unk_154;
    u8 pad_158[344];
    u32 unk_2B0;
    struct FontSpecialCharsContext *specialChars;
    Options *options;
    SaveData *saveData;
    BattleCastleSave *castleSave;
    BattleCastlePersistentSave *persistentSave;
    BattleCastleAppSpriteManager spriteMan;
    BattleCastleAppSprite *monSprites[4];
    BattleCastleAppSprite *ballSprites[4];
    BattleCastleAppSprite *arrowSprite;
    BattleCastleAppSprite *cursorSprite;
    BattleCastleAppSprite *partnerCursorSprite;
    struct BattleCastleAppSprite *barSprites[4];
    struct BattleCastleAppSprite *flagSprites[2][4];
    struct BattleCastleAppSprite *sparklesSprite;
    BattleCastleAppSprite *selectedMonSprite;
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
    u32 unused4;
    u32 unk_5C0;
    u16 unk_5C4;
    u8 pad_5C6[6];
    u32 unk_5CC;
    u8 pad_5D0[32];
    u32 unk_5F0;
    u32 unk_5F4;
    u32 unk_5F8;
    u32 unk_5FC;
    u32 unk_600;
    u32 unk_604;
} BattleCastleOpponentApp;

u8 PrintMessageAndCopyToVRAM(BattleCastleOpponentApp *app, int entryID, u8 font);

void PrintPlayersAndPartnersCastlePoints(BattleCastleOpponentApp *app, Window *window);

void SetStringTemplatePlayerName(BattleCastleOpponentApp *app, u32 idx);

void FreeSimpleMenu(BattleCastleOpponentApp *app);

void LoadMainBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

void LoadSummaryBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

u8 ov81_0224080C(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void ov82_0223EFB4(BattleCastleOpponentApp *app, u32 idx, s32 num, u32 maxDigits, enum PaddingMode paddingMode);

void ChangeState(BattleCastleOpponentApp *app, int *state, enum BattleCastleAppState newState);

extern const u16 ov83_02248054[3][3];

void ov83_02245ACC(BattleCastleOpponentApp *app, u8 slot, u8 menuOption);

u32 ov83_0224776C(u32 exitSlot, u32 slotID);

void ov83_02244ABC(BattleCastleOpponentApp *app, u32 idx);

void ov83_022477C4(StringTemplate *template, u32 idx);

void ov83_02245390(BattleCastleOpponentApp *app);

void ov83_022453DC(BattleCastleOpponentApp *app, Window *window);

u8 ov83_022448AC(BattleCastleOpponentApp *app, int entryID, u8 font);

void ov83_02245ACC(BattleCastleOpponentApp *app, u8 slot, u8 menuOption) {
    u8 unused = ov80_02237B24(app->challengeType, 0);

    u8 rankType;
    if (menuOption == 5) {
        rankType = 2;
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    u8 exitSlot = app->exitSlot;
    u8 unused2 = ov83_0224776C(exitSlot, slot);

    u32 rank;
    if (sub_0203769C() == 0) {
        if (slot < exitSlot) {
            ov83_02244ABC(app, 5);
            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);

            ov80_02237FA4(app->frontier, app->challengeType, 50);
            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);

            sub_02031108(Save_Frontier_GetStatic(app->saveData), sub_0205C174(app->challengeType, rankType), sub_0205C268(sub_0205C174(app->challengeType, rankType)), rank + 1);

            if (ov80_02237D8C(app->challengeType) == 1) {
                app->syncPurchaseState = 2;
            }
        } else {
            ov83_022477C4(app->strTemplate, 5);
            rank = app->partnersRanks[rankType];
            app->partnersCP -= 50;
            app->partnersRanks[rankType]++;
        }
    } else {
        if (slot < exitSlot) {
            ov83_022477C4(app->strTemplate, 5);
            rank = app->partnersRanks[rankType];
            app->partnersCP -= 50;
            app->partnersRanks[rankType]++;
        } else {
            ov83_02244ABC(app, 5);
            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);

            ov80_02237FA4(app->frontier, app->challengeType, 50);
            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);

            sub_02031108(Save_Frontier_GetStatic(app->saveData), sub_0205C174(app->challengeType, rankType), sub_0205C268(sub_0205C174(app->challengeType, rankType)), rank + 1);

            if (ov80_02237D8C(app->challengeType) == 1) {
                app->syncPurchaseState = 2;
            }
        }
    }

    ov83_02245390(app);
    ov83_022453DC(app, &app->windows[OPPONENT_APP_WINDOW_HEADER]);
    ov83_02247944(&app->windows[OPPONENT_APP_WINDOW_MSG_BOX], Options_GetFrame(app->options));

    app->printerID = ov83_022448AC(app, ov83_02248054[rankType][rank], FONT_MESSAGE);
}
