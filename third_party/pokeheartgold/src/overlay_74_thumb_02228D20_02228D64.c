#include "global.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BallCapsule;

struct BallSeal;

struct BattleRegulation;

struct BgConfig;

struct BoxPokemon;

struct EasyChatSentence;

struct GXBanks;

struct GraphicsModes;

struct Mail;

struct MainMenuWindow;

struct MysteryGift;

struct MysteryGiftCosmeticsData;

struct MysteryGiftEventHeader;

struct MysteryGiftItemData;

struct MysteryGiftPokemonData;

struct Options;

struct PGT;

struct PartyPokemon;

struct Pokedex;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct RangerLinkManager;

struct SaveData;

struct Sprite;

struct StringTemplate;

struct TrainerInfo;

struct WaitDial;

struct Window;

struct WonderCard;

enum HeapID {
    HEAP_ID_SYSTEM = 0,
    HEAP_ID_SAVE,
    HEAP_ID_DEBUG,
    HEAP_ID_APPLICATION,
    HEAP_ID_FIELD1,
    HEAP_ID_BATTLE,
    HEAP_ID_BAG,
    HEAP_ID_POKETCH_MAIN,
    HEAP_ID_POKETCH_APP,
    HEAP_ID_BOX_DATA,
    HEAP_ID_BOX_GRAPHICS,
    HEAP_ID_FIELD2,
    HEAP_ID_PARTY_MENU,
    HEAP_ID_13,
    HEAP_ID_14,
    HEAP_ID_COMMUNICATION,
    HEAP_ID_16,
    HEAP_ID_TOWN_MAP,
    HEAP_ID_NAMING_SCREEN_APP,
    HEAP_ID_POKEMON_SUMMARY_SCREEN,
    HEAP_ID_20,
    HEAP_ID_21,
    HEAP_ID_22,
    HEAP_ID_23,
    HEAP_ID_24,
    HEAP_ID_TRAINER_CASE,
    HEAP_ID_TRADE_ROOM,
    HEAP_ID_27,
    HEAP_ID_28,
    HEAP_ID_MINING,
    HEAP_ID_DISTORTION_WORLD_WARP,
    HEAP_ID_31,
    HEAP_ID_FIELD3,
    HEAP_ID_UNDERGROUND,
    HEAP_ID_34,
    HEAP_ID_35,
    HEAP_ID_BERRY_TAG,
    HEAP_ID_POKEDEX,
    HEAP_ID_OPTIONS_MENU,
    HEAP_ID_DRAWING,
    HEAP_ID_MAIL,
    HEAP_ID_MAIL_VIEWER,
    HEAP_ID_JOURNAL,
    HEAP_ID_43,
    HEAP_ID_POFFIN_CASE,
    HEAP_ID_POFFIN_MANAGER,
    HEAP_ID_POFFIN_CUTSCENE,
    HEAP_ID_CHOOSE_STARTER_APP,
    HEAP_ID_48,
    HEAP_ID_NINTENDO_WFC,
    HEAP_ID_50,
    HEAP_ID_51,
    HEAP_ID_52,
    HEAP_ID_53,
    HEAP_ID_54,
    HEAP_ID_POFFIN_BERRY_SELECTION,
    HEAP_ID_56,
    HEAP_ID_TRADE_SEQUENCE,
    HEAP_ID_TRADE_SEQUENCE_PHASE,
    HEAP_ID_59,
    HEAP_ID_60,
    HEAP_ID_PC_HALL_OF_FAME,
    HEAP_ID_62,
    HEAP_ID_HALL_OF_FAME,
    HEAP_ID_64,
    HEAP_ID_65,
    HEAP_ID_66,
    HEAP_ID_MOVE_REMINDER,
    HEAP_ID_68,
    HEAP_ID_69,
    HEAP_ID_70,
    HEAP_ID_EGG_HATCH,
    HEAP_ID_BOAT_CUTSCENE,
    HEAP_ID_EVOLUTION,
    HEAP_ID_BATTLE_TOWER_RECORDS_APP,
    HEAP_ID_END_CREDITS,
    HEAP_ID_76,
    HEAP_ID_GAME_START,
    HEAP_ID_MIGRATE_FROM_GBA,
    HEAP_ID_79,
    HEAP_ID_RANKINGS_MACHINE,
    HEAP_ID_MAIN_MENU,
    HEAP_ID_ROWAN_INTRO,
    HEAP_ID_ROWAN_INTRO_TV,
    HEAP_ID_DIPLOMA,
    HEAP_ID_RANGER_LINK,
    HEAP_ID_MYSTERY_GIFT_APP,
    HEAP_ID_WONDER_CARDS_APP,
    HEAP_ID_88,
    HEAP_ID_89,
    HEAP_ID_90,
    HEAP_ID_NETWORK_ICON,
    HEAP_ID_92,
    HEAP_ID_BATTLE_FACTORY_APP,
    HEAP_ID_94,
    HEAP_ID_95,
    HEAP_ID_96,
    HEAP_ID_97,
    HEAP_ID_BATTLE_HALL_APP,
    HEAP_ID_99,
    HEAP_ID_BATTLE_CASTLE_APP,
    HEAP_ID_101,
    HEAP_ID_102,
    HEAP_ID_BATTLE_ARCADE_APP,
    HEAP_ID_104,
    HEAP_ID_105,
    HEAP_ID_106,
    HEAP_ID_107,
    HEAP_ID_108,
    HEAP_ID_109,
    HEAP_ID_110,
    HEAP_ID_SPEAR_PILLAR_CUTSCENE,
    HEAP_ID_112,
    HEAP_ID_113,
    HEAP_ID_FRONTIER_RECORD_APP,
    HEAP_ID_SCRATCH_OFF_CARD_APP,
    HEAP_ID_116,
    HEAP_ID_117,
    HEAP_ID_118,
    HEAP_ID_119,
    HEAP_ID_LIBRARY_TV,
    HEAP_ID_PARTY_MENU1,
    HEAP_ID_PARTY_MENU2,
    HEAP_ID_MAX,
};

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

enum BgScreenSize {
    BG_SCREEN_SIZE_128x128 = 0,
    BG_SCREEN_SIZE_256x256,
    BG_SCREEN_SIZE_256x512,
    BG_SCREEN_SIZE_512x256,
    BG_SCREEN_SIZE_512x512,
    BG_SCREEN_SIZE_1024x1024,
};

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct GraphicsModes {
    GXDispMode displayMode;
    GXBGMode mainBgMode;
    GXBGMode subBgMode;
    GXBG0As bg0As2DOr3D;
} GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void BgCommitTilemapBufferToVram(BgConfig *bgConfig, u8 bgLayer);

void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette);

typedef struct SaveData SaveData;

typedef struct Options Options;

int Options_GetFrame(const Options *options);

typedef u16 charcode_t;

enum NarcID {
    NARC_INDEX_BATTLE__SKILL__WAZA_SEQ = 0,
    NARC_INDEX_BATTLE__SKILL__SUB_SEQ,
    NARC_INDEX_POKETOOL__PERSONAL__PL_PERSONAL,
    NARC_INDEX_POKETOOL__PERSONAL__PL_GROWTBL,
    NARC_INDEX_POKETOOL__POKEGRA__PL_POKEGRA,
    NARC_INDEX_POKETOOL__POKEGRA__HEIGHT,
    NARC_INDEX_POKETOOL__TRGRA__TRBGRA,
    NARC_INDEX_BATTLE__GRAPHIC__PL_BATT_BG,
    NARC_INDEX_WAZAEFFECT__WE,
    NARC_INDEX_POKETOOL__WAZA__PL_WAZA_TBL,
    NARC_INDEX_FIELDDATA__SCRIPT__SCR_SEQ,
    NARC_INDEX_MSGDATA__SCENARIO__SCR_MSG,
    NARC_INDEX_GRAPHIC__POKETCH,
    NARC_INDEX_GRAPHIC__PL_BAG_GRA,
    NARC_INDEX_GRAPHIC__PL_FONT,
    NARC_INDEX_ITEMTOOL__ITEMDATA__PL_ITEM_DATA,
    NARC_INDEX_ITEMTOOL__ITEMDATA__ITEM_ICON,
    NARC_INDEX_GRAPHIC__TMAP_GRA,
    NARC_INDEX_GRAPHIC__BOX,
    NARC_INDEX_POKETOOL__ICONGRA__PL_POKE_ICON,
    NARC_INDEX_GRAPHIC__PL_PLIST_GRA,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WECHAR,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WEPLTT,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WECELL,
    NARC_INDEX_WAZAEFFECT__EFFECTCLACT__WECELLANM,
    NARC_INDEX_GRAPHIC__IMAGECLIP,
    NARC_INDEX_MSGDATA__PL_MSG,
    NARC_INDEX_BATTLE__GRAPHIC__PL_BATT_OBJ,
    NARC_INDEX_DATA__UNDERG_RADAR,
    NARC_INDEX_WAZAEFFECT__EFFECTDATA__WAZA_PARTICLE,
    NARC_INDEX_BATTLE__SKILL__BE_SEQ,
    NARC_INDEX_DATA__NAMEIN,
    NARC_INDEX_FIELDDATA__EVENTDATA__ZONE_EVENT,
    NARC_INDEX_POKETOOL__PERSONAL__WOTBL,
    NARC_INDEX_POKETOOL__PERSONAL__EVO,
    NARC_INDEX_GRAPHIC__FONTOAM,
    NARC_INDEX_GRAPHIC__FIELD_BOARD,
    NARC_INDEX_FIELDDATA__ENCOUNTDATA__D_ENC_DATA,
    NARC_INDEX_GRAPHIC__PL_WINFRAME,
    NARC_INDEX_GRAPHIC__PL_PST_GRA,
    NARC_INDEX_FIELDDATA__BUILD_MODEL__BUILD_MODEL,
    NARC_INDEX_FIELDDATA__MAPMATRIX__MAP_MATRIX,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_DATA,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_BUILD_MODEL__AREA_BUILD,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_MAP_TEX__MAP_TEX_SET,
    NARC_INDEX_CONTEST__GRAPHIC__CONTEST_BG,
    NARC_INDEX_CONTEST__GRAPHIC__CONTEST_OBJ,
    NARC_INDEX_DATA__UGEFFECT_OBJ_GRAPHIC,
    NARC_INDEX_DEMO__TITLE__TITLEDEMO,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_MOVE_MODEL__MOVE_MODEL_LIST,
    NARC_INDEX_DATA__UG_TRAP,
    NARC_INDEX_GRAPHIC__TRAINER_CASE,
    NARC_INDEX_DATA__UG_FOSSIL,
    NARC_INDEX_GRAPHIC__NTAG_GRA,
    NARC_INDEX_DATA__TRADELIST,
    NARC_INDEX_DATA__UG_PARTS,
    NARC_INDEX_GRAPHIC__OPENING,
    NARC_INDEX_POKETOOL__TRAINER__TRDATA,
    NARC_INDEX_POKETOOL__TRAINER__TRPOKE,
    NARC_INDEX_POKETOOL__TRMSG__TRTBL,
    NARC_INDEX_POKETOOL__TRGRA__TRFGRA,
    NARC_INDEX_PARTICLEDATA__PARTICLEDATA,
    NARC_INDEX_GRAPHIC__SHOP_GRA,
    NARC_INDEX_WAZAEFFECT__WE_SUB,
    NARC_INDEX_DATA__UG_ANIM,
    NARC_INDEX_DATA__WEATHER_SYS,
    NARC_INDEX_GRAPHIC__MENU_GRA,
    NARC_INDEX_FIELDDATA__LAND_DATA__LAND_DATA,
    NARC_INDEX_ITEMTOOL__ITEMDATA__NUTS_DATA,
    NARC_INDEX_RESOURCE__ENG__ZUKAN__ZUKAN,
    NARC_INDEX_POKETOOL__POKEFOOT__POKEFOOT,
    NARC_INDEX_FIELDDATA__AREADATA__AREA_BUILD_MODEL__AREABM_TEXSET,
    NARC_INDEX_BATTLE__GRAPHIC__PL_B_PLIST_GRA,
    NARC_INDEX_GRAPHIC__CONFIG_GRA,
    NARC_INDEX_GRAPHIC__UNIONROOM,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_DATA,
    NARC_INDEX_GRAPHIC__PMSI,
    NARC_INDEX_BATTLE__GRAPHIC__B_BAG_GRA,
    NARC_INDEX_GRAPHIC__OEKAKI,
    NARC_INDEX_GRAPHIC__MAIL_GRA,
    NARC_INDEX_GRAPHIC__F_NOTE_GRA,
    NARC_INDEX_DATA__MMODEL__MMODEL,
    NARC_INDEX_GRAPHIC__EV_POKESELECT,
    NARC_INDEX_GRAPHIC__PORU_GRA,
    NARC_INDEX_GRAPHIC__TOUCH_SUBWINDOW,
    NARC_INDEX_GRAPHIC__FLD_COMACT,
    NARC_INDEX_GRAPHIC__RECORD,
    NARC_INDEX_GRAPHIC__PORUDEMO,
    NARC_INDEX_GRAPHIC__PORUACT,
    NARC_INDEX_GRAPHIC__PL_WIFINOTE,
    NARC_INDEX_CONTEST__DATA__CONTEST_DATA,
    NARC_INDEX_APPLICATION__CUSTOM_BALL__DATA__CB_DATA,
    NARC_INDEX_DATA__WIFIP2PMATCH,
    NARC_INDEX_GRAPHIC__DEMO_TRADE,
    NARC_INDEX_POKEANIME__PL_POKE_ANM,
    NARC_INDEX_GRAPHIC__MYSIGN,
    NARC_INDEX_FIELDDATA__MM_LIST__MOVE_MODEL_LIST,
    NARC_INDEX_DATA__FIELD_CUTIN,
    NARC_INDEX_DATA__DEMO_CLIMAX,
    NARC_INDEX_WAZAEFFECT__EFFECTDATA__BALL_PARTICLE,
    NARC_INDEX_WAZAEFFECT__PT_DEBUG__DEBUG_PARTICLE,
    NARC_INDEX_GRAPHIC__DENDOU_PC,
    NARC_INDEX_APPLICATION__WIFI_EARTH__WIFI_EARTH_PLACE,
    NARC_INDEX_BATTLE__TR_AI__TR_AI_SEQ,
    NARC_INDEX_GRAPHIC__WORLDTRADE,
    NARC_INDEX_GRAPHIC__DENDOU_DEMO,
    NARC_INDEX_ARC__ENCDATA_EX,
    NARC_INDEX_DATA__MMODEL__FLDEFF,
    NARC_INDEX_BATTLE__GRAPHIC__VS_DEMO_GRA,
    NARC_INDEX_POKETOOL__PL_POKEZUKAN,
    NARC_INDEX_ARC__BM_ANIME,
    NARC_INDEX_ARC__BM_ANIME_LIST,
    NARC_INDEX_GRAPHIC__FIELD_ENCOUNTEFFECT,
    NARC_INDEX_GRAPHIC__WAZA_OSHIE_GRA,
    NARC_INDEX_POKETOOL__POKEANM__PL_POKEANM,
    NARC_INDEX_FIELDDATA__POKEMON_TRADE__FLD_TRADE,
    NARC_INDEX_GRAPHIC__MYSTERY,
    NARC_INDEX_POKETOOL__POKEGRA__PL_OTHERPOKE,
    NARC_INDEX_DEMO__EGG__DATA__EGG_DATA,
    NARC_INDEX_DEMO__EGG__DATA__PARTICLE__EGG_DEMO_PARTICLE,
    NARC_INDEX_ARC__SHIP_DEMO,
    NARC_INDEX_POKETOOL__POKEGRA__HEIGHT_O,
    NARC_INDEX_GRAPHIC__NUTMIXER,
    NARC_INDEX_GRAPHIC__BTOWER,
    NARC_INDEX_DEMO__SHINKA__DATA__PARTICLE__SHINKA_DEMO_PARTICLE,
    NARC_INDEX_DEMO__INTRO__INTRO,
    NARC_INDEX_ARC__TV,
    NARC_INDEX_GRAPHIC__ENDING,
    NARC_INDEX_DEMO__TITLE__OP_DEMO,
    NARC_INDEX_APPLICATION__WIFI_EARTH__WIFI_EARTH,
    NARC_INDEX_GRAPHIC__RANKING,
    NARC_INDEX_ARC__PPARK,
    NARC_INDEX_DEMO__SYOUJYOU__SYOUJYOU,
    NARC_INDEX_DATA__SLOT,
    NARC_INDEX_BATTLE__B_TOWER__BTDTR,
    NARC_INDEX_BATTLE__B_TOWER__BTDPM,
    NARC_INDEX_DEMO__INTRO__INTRO_TV,
    NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS,
    NARC_INDEX_POKETOOL__POKEGRA__POKE_YOFS,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_ENC_DIAMOND,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_ENC_PEARL,
    NARC_INDEX_GRAPHIC__HIDEN_EFFECT,
    NARC_INDEX_POKETOOL__POKEGRA__POKE_SHADOW,
    NARC_INDEX_FIELDDATA__ENCOUNTDATA__P_ENC_DATA,
    NARC_INDEX_POKETOOL__POKEGRA__POKE_SHADOW_OFX,
    NARC_INDEX_POKETOOL__SHINZUKAN,
    NARC_INDEX_GRAPHIC__WIFI2DCHAR,
    NARC_INDEX_POKETOOL__POKE_EDIT__PL_POKE_DATA,
    NARC_INDEX_DEBUG__CB_EDIT__D_TEST,
    NARC_INDEX_FRONTIER__SCRIPT__FR_SCRIPT,
    NARC_INDEX_FRONTIER_BACKGROUNDS,
    NARC_INDEX_RESOURCE__ENG__FRONTIER_GRAPHIC__FRONTIER_OBJ,
    NARC_INDEX_APPLICATION__CUSTOM_BALL__EDIT__PL_CB_DATA,
    NARC_INDEX_GRAPHIC__WIFI_UNIONOBJ,
    NARC_INDEX_DATA__GURU2,
    NARC_INDEX_DATA__FLDTANIME,
    NARC_INDEX_ARC__SHIP_DEMO_PL,
    NARC_INDEX_PARTICLEDATA__PL_FRONTIER__FRONTIER_PARTICLE,
    NARC_INDEX_GRAPHIC__BUCKET,
    NARC_INDEX_ARC__CODEIN_GRA,
    NARC_INDEX_ARC__AREA_WIN_GRA,
    NARC_INDEX_FIELDDATA__ENCOUNTDATA__PL_ENC_DATA,
    NARC_INDEX_RESOURCE__ENG__BATT_REC__BATT_REC_GRA,
    NARC_INDEX_GRAPHIC__WORLDTIMER,
    NARC_INDEX_ARC__MANENE,
    NARC_INDEX_POKETOOL__POKEGRA__POKEGRA,
    NARC_INDEX_POKETOOL__POKEGRA__OTHERPOKE,
    NARC_INDEX_POKETOOL__POKEGRA__DP_HEIGHT,
    NARC_INDEX_POKETOOL__POKEGRA__DP_HEIGHT_O,
    NARC_INDEX_GRAPHIC__LOBBY_NEWS,
    NARC_INDEX_ARC__EMAIL_GRA,
    NARC_INDEX_RESOURCE__ENG__WIFI_LOBBY_MINIGAME__WLMNGM_TOOL,
    NARC_INDEX_ARC__DEMO_TENGAN_GRA,
    NARC_INDEX_APPLICATION__BALLOON__GRAPHIC__BALLOON_GRA,
    NARC_INDEX_APPLICATION__WIFI_LOBBY__MAP_CONV__WFLBY_MAP,
    NARC_INDEX_GRAPHIC__WIFI_LOBBY,
    NARC_INDEX_DATA__AREALIGHT,
    NARC_INDEX_DATA__UGROUNDEFFECT,
    NARC_INDEX_BATTLE__B_PL_TOWER__PL_BTDTR,
    NARC_INDEX_BATTLE__B_PL_TOWER__PL_BTDPM,
    NARC_INDEX_BATTLE__B_PL_STAGE__PL_BSDPM,
    NARC_INDEX_FIELDDATA__TORNWORLD__TW_ARC,
    NARC_INDEX_FIELDDATA__TORNWORLD__TW_ARC_ATTR,
    NARC_INDEX_DATA__TW_ARC_ETC,
    NARC_INDEX_SCRATCH_OFF_CARDS,
    NARC_INDEX_PARTICLEDATA__PL_POKELIST__POKELIST_PARTICLE,
    NARC_INDEX_GRAPHIC__WIFI_LOBBY_OTHER,
    NARC_INDEX_GRAPHIC__FOOTPRINT_BOARD,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_ENC_PLATINUM,
    NARC_INDEX_APPLICATION__BUCKET__BALLSLOW_DATA,
    NARC_INDEX_PARTICLEDATA__PL_ETC__PL_ETC_PARTICLE,
    NARC_INDEX_RESOURCE__ENG__PMS_AIKOTOBA__PMS_AIKOTOBA,
    NARC_INDEX_GRAPHIC__UNIONOBJ2D_ONLYFRONT,
    NARC_INDEX_GRAPHIC__LIBRARY_TV,
    NARC_INDEX_APPLICATION__ZUKANLIST__ZKN_DATA__ZUKAN_DATA_GIRA,
};

enum PaletteLoadLocation {
    PAL_LOAD_MAIN_BG = 0,
    PAL_LOAD_MAIN_OBJ,
    PAL_LOAD_MAIN_BGEXT,
    PAL_LOAD_MAIN_OBJEXT,
    PAL_LOAD_SUB_BG,
    PAL_LOAD_SUB_OBJ,
    PAL_LOAD_SUB_BGEXT,
    PAL_LOAD_SUB_OBJEXT,
};

u32 GfGfxLoader_LoadCharData(enum NarcID narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnData(enum NarcID narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

void GfGfxLoader_GXLoadPal(enum NarcID narcID, u32 narcMemberIdx, enum PaletteLoadLocation loadLocation, u32 palOffset, u32 size, enum HeapID heapID);

enum Font {
    FONT_SYSTEM = 0,
    FONT_MESSAGE,
    FONT_SUBSCREEN,
    FONT_UNOWN,

    FONT_MAX,
};

void LoadFontPal0(enum PaletteLoadLocation palLocation, u32 palSlotOffset, enum HeapID heapID);

typedef u32 TextColor;

void ResetAllTextPrinters(void);

enum Item {
    ITEM_NONE = 0,
    ITEM_MASTER_BALL = 1,
    ITEM_ULTRA_BALL = 2,
    ITEM_GREAT_BALL = 3,
    ITEM_POKE_BALL = 4,
    ITEM_SAFARI_BALL = 5,
    ITEM_NET_BALL = 6,
    ITEM_DIVE_BALL = 7,
    ITEM_NEST_BALL = 8,
    ITEM_REPEAT_BALL = 9,
    ITEM_TIMER_BALL = 10,
    ITEM_LUXURY_BALL = 11,
    ITEM_PREMIER_BALL = 12,
    ITEM_DUSK_BALL = 13,
    ITEM_HEAL_BALL = 14,
    ITEM_QUICK_BALL = 15,
    ITEM_CHERISH_BALL = 16,
    ITEM_POTION = 17,
    ITEM_ANTIDOTE = 18,
    ITEM_BURN_HEAL = 19,
    ITEM_ICE_HEAL = 20,
    ITEM_AWAKENING = 21,
    ITEM_PARLYZ_HEAL = 22,
    ITEM_FULL_RESTORE = 23,
    ITEM_MAX_POTION = 24,
    ITEM_HYPER_POTION = 25,
    ITEM_SUPER_POTION = 26,
    ITEM_FULL_HEAL = 27,
    ITEM_REVIVE = 28,
    ITEM_MAX_REVIVE = 29,
    ITEM_FRESH_WATER = 30,
    ITEM_SODA_POP = 31,
    ITEM_LEMONADE = 32,
    ITEM_MOOMOO_MILK = 33,
    ITEM_ENERGYPOWDER = 34,
    ITEM_ENERGY_ROOT = 35,
    ITEM_HEAL_POWDER = 36,
    ITEM_REVIVAL_HERB = 37,
    ITEM_ETHER = 38,
    ITEM_MAX_ETHER = 39,
    ITEM_ELIXIR = 40,
    ITEM_MAX_ELIXIR = 41,
    ITEM_LAVA_COOKIE = 42,
    ITEM_BERRY_JUICE = 43,
    ITEM_SACRED_ASH = 44,
    ITEM_HP_UP = 45,
    ITEM_PROTEIN = 46,
    ITEM_IRON = 47,
    ITEM_CARBOS = 48,
    ITEM_CALCIUM = 49,
    ITEM_RARE_CANDY = 50,
    ITEM_PP_UP = 51,
    ITEM_ZINC = 52,
    ITEM_PP_MAX = 53,
    ITEM_OLD_GATEAU = 54,
    ITEM_GUARD_SPEC = 55,
    ITEM_DIRE_HIT = 56,
    ITEM_X_ATTACK = 57,
    ITEM_X_DEFENSE = 58,
    ITEM_X_SPEED = 59,
    ITEM_X_ACCURACY = 60,
    ITEM_X_SPECIAL = 61,
    ITEM_X_SP_DEF = 62,
    ITEM_POKE_DOLL = 63,
    ITEM_FLUFFY_TAIL = 64,
    ITEM_BLUE_FLUTE = 65,
    ITEM_YELLOW_FLUTE = 66,
    ITEM_RED_FLUTE = 67,
    ITEM_BLACK_FLUTE = 68,
    ITEM_WHITE_FLUTE = 69,
    ITEM_SHOAL_SALT = 70,
    ITEM_SHOAL_SHELL = 71,
    ITEM_RED_SHARD = 72,
    ITEM_BLUE_SHARD = 73,
    ITEM_YELLOW_SHARD = 74,
    ITEM_GREEN_SHARD = 75,
    ITEM_SUPER_REPEL = 76,
    ITEM_MAX_REPEL = 77,
    ITEM_ESCAPE_ROPE = 78,
    ITEM_REPEL = 79,
    ITEM_SUN_STONE = 80,
    ITEM_MOON_STONE = 81,
    ITEM_FIRE_STONE = 82,
    ITEM_THUNDERSTONE = 83,
    ITEM_WATER_STONE = 84,
    ITEM_LEAF_STONE = 85,
    ITEM_TINYMUSHROOM = 86,
    ITEM_BIG_MUSHROOM = 87,
    ITEM_PEARL = 88,
    ITEM_BIG_PEARL = 89,
    ITEM_STARDUST = 90,
    ITEM_STAR_PIECE = 91,
    ITEM_NUGGET = 92,
    ITEM_HEART_SCALE = 93,
    ITEM_HONEY = 94,
    ITEM_GROWTH_MULCH = 95,
    ITEM_DAMP_MULCH = 96,
    ITEM_STABLE_MULCH = 97,
    ITEM_GOOEY_MULCH = 98,
    ITEM_ROOT_FOSSIL = 99,
    ITEM_CLAW_FOSSIL = 100,
    ITEM_HELIX_FOSSIL = 101,
    ITEM_DOME_FOSSIL = 102,
    ITEM_OLD_AMBER = 103,
    ITEM_ARMOR_FOSSIL = 104,
    ITEM_SKULL_FOSSIL = 105,
    ITEM_RARE_BONE = 106,
    ITEM_SHINY_STONE = 107,
    ITEM_DUSK_STONE = 108,
    ITEM_DAWN_STONE = 109,
    ITEM_OVAL_STONE = 110,
    ITEM_ODD_KEYSTONE = 111,
    ITEM_GRISEOUS_ORB = 112,
    ITEM_UNUSED_113 = 113,
    ITEM_UNUSED_114 = 114,
    ITEM_UNUSED_115 = 115,
    ITEM_UNUSED_116 = 116,
    ITEM_UNUSED_117 = 117,
    ITEM_UNUSED_118 = 118,
    ITEM_UNUSED_119 = 119,
    ITEM_UNUSED_120 = 120,
    ITEM_UNUSED_121 = 121,
    ITEM_UNUSED_122 = 122,
    ITEM_UNUSED_123 = 123,
    ITEM_UNUSED_124 = 124,
    ITEM_UNUSED_125 = 125,
    ITEM_UNUSED_126 = 126,
    ITEM_UNUSED_127 = 127,
    ITEM_UNUSED_128 = 128,
    ITEM_UNUSED_129 = 129,
    ITEM_UNUSED_130 = 130,
    ITEM_UNUSED_131 = 131,
    ITEM_UNUSED_132 = 132,
    ITEM_UNUSED_133 = 133,
    ITEM_UNUSED_134 = 134,
    ITEM_ADAMANT_ORB = 135,
    ITEM_LUSTROUS_ORB = 136,
    ITEM_GRASS_MAIL = 137,
    ITEM_FLAME_MAIL = 138,
    ITEM_BUBBLE_MAIL = 139,
    ITEM_BLOOM_MAIL = 140,
    ITEM_TUNNEL_MAIL = 141,
    ITEM_STEEL_MAIL = 142,
    ITEM_HEART_MAIL = 143,
    ITEM_SNOW_MAIL = 144,
    ITEM_SPACE_MAIL = 145,
    ITEM_AIR_MAIL = 146,
    ITEM_MOSAIC_MAIL = 147,
    ITEM_BRICK_MAIL = 148,
    ITEM_CHERI_BERRY = 149,
    ITEM_CHESTO_BERRY = 150,
    ITEM_PECHA_BERRY = 151,
    ITEM_RAWST_BERRY = 152,
    ITEM_ASPEAR_BERRY = 153,
    ITEM_LEPPA_BERRY = 154,
    ITEM_ORAN_BERRY = 155,
    ITEM_PERSIM_BERRY = 156,
    ITEM_LUM_BERRY = 157,
    ITEM_SITRUS_BERRY = 158,
    ITEM_FIGY_BERRY = 159,
    ITEM_WIKI_BERRY = 160,
    ITEM_MAGO_BERRY = 161,
    ITEM_AGUAV_BERRY = 162,
    ITEM_IAPAPA_BERRY = 163,
    ITEM_RAZZ_BERRY = 164,
    ITEM_BLUK_BERRY = 165,
    ITEM_NANAB_BERRY = 166,
    ITEM_WEPEAR_BERRY = 167,
    ITEM_PINAP_BERRY = 168,
    ITEM_POMEG_BERRY = 169,
    ITEM_KELPSY_BERRY = 170,
    ITEM_QUALOT_BERRY = 171,
    ITEM_HONDEW_BERRY = 172,
    ITEM_GREPA_BERRY = 173,
    ITEM_TAMATO_BERRY = 174,
    ITEM_CORNN_BERRY = 175,
    ITEM_MAGOST_BERRY = 176,
    ITEM_RABUTA_BERRY = 177,
    ITEM_NOMEL_BERRY = 178,
    ITEM_SPELON_BERRY = 179,
    ITEM_PAMTRE_BERRY = 180,
    ITEM_WATMEL_BERRY = 181,
    ITEM_DURIN_BERRY = 182,
    ITEM_BELUE_BERRY = 183,
    ITEM_OCCA_BERRY = 184,
    ITEM_PASSHO_BERRY = 185,
    ITEM_WACAN_BERRY = 186,
    ITEM_RINDO_BERRY = 187,
    ITEM_YACHE_BERRY = 188,
    ITEM_CHOPLE_BERRY = 189,
    ITEM_KEBIA_BERRY = 190,
    ITEM_SHUCA_BERRY = 191,
    ITEM_COBA_BERRY = 192,
    ITEM_PAYAPA_BERRY = 193,
    ITEM_TANGA_BERRY = 194,
    ITEM_CHARTI_BERRY = 195,
    ITEM_KASIB_BERRY = 196,
    ITEM_HABAN_BERRY = 197,
    ITEM_COLBUR_BERRY = 198,
    ITEM_BABIRI_BERRY = 199,
    ITEM_CHILAN_BERRY = 200,
    ITEM_LIECHI_BERRY = 201,
    ITEM_GANLON_BERRY = 202,
    ITEM_SALAC_BERRY = 203,
    ITEM_PETAYA_BERRY = 204,
    ITEM_APICOT_BERRY = 205,
    ITEM_LANSAT_BERRY = 206,
    ITEM_STARF_BERRY = 207,
    ITEM_ENIGMA_BERRY = 208,
    ITEM_MICLE_BERRY = 209,
    ITEM_CUSTAP_BERRY = 210,
    ITEM_JABOCA_BERRY = 211,
    ITEM_ROWAP_BERRY = 212,
    ITEM_BRIGHTPOWDER = 213,
    ITEM_WHITE_HERB = 214,
    ITEM_MACHO_BRACE = 215,
    ITEM_EXP_SHARE = 216,
    ITEM_QUICK_CLAW = 217,
    ITEM_SOOTHE_BELL = 218,
    ITEM_MENTAL_HERB = 219,
    ITEM_CHOICE_BAND = 220,
    ITEM_KINGS_ROCK = 221,
    ITEM_SILVERPOWDER = 222,
    ITEM_AMULET_COIN = 223,
    ITEM_CLEANSE_TAG = 224,
    ITEM_SOUL_DEW = 225,
    ITEM_DEEPSEATOOTH = 226,
    ITEM_DEEPSEASCALE = 227,
    ITEM_SMOKE_BALL = 228,
    ITEM_EVERSTONE = 229,
    ITEM_FOCUS_BAND = 230,
    ITEM_LUCKY_EGG = 231,
    ITEM_SCOPE_LENS = 232,
    ITEM_METAL_COAT = 233,
    ITEM_LEFTOVERS = 234,
    ITEM_DRAGON_SCALE = 235,
    ITEM_LIGHT_BALL = 236,
    ITEM_SOFT_SAND = 237,
    ITEM_HARD_STONE = 238,
    ITEM_MIRACLE_SEED = 239,
    ITEM_BLACKGLASSES = 240,
    ITEM_BLACK_BELT = 241,
    ITEM_MAGNET = 242,
    ITEM_MYSTIC_WATER = 243,
    ITEM_SHARP_BEAK = 244,
    ITEM_POISON_BARB = 245,
    ITEM_NEVERMELTICE = 246,
    ITEM_SPELL_TAG = 247,
    ITEM_TWISTEDSPOON = 248,
    ITEM_CHARCOAL = 249,
    ITEM_DRAGON_FANG = 250,
    ITEM_SILK_SCARF = 251,
    ITEM_UPGRADE = 252,
    ITEM_SHELL_BELL = 253,
    ITEM_SEA_INCENSE = 254,
    ITEM_LAX_INCENSE = 255,
    ITEM_LUCKY_PUNCH = 256,
    ITEM_METAL_POWDER = 257,
    ITEM_THICK_CLUB = 258,
    ITEM_STICK = 259,
    ITEM_RED_SCARF = 260,
    ITEM_BLUE_SCARF = 261,
    ITEM_PINK_SCARF = 262,
    ITEM_GREEN_SCARF = 263,
    ITEM_YELLOW_SCARF = 264,
    ITEM_WIDE_LENS = 265,
    ITEM_MUSCLE_BAND = 266,
    ITEM_WISE_GLASSES = 267,
    ITEM_EXPERT_BELT = 268,
    ITEM_LIGHT_CLAY = 269,
    ITEM_LIFE_ORB = 270,
    ITEM_POWER_HERB = 271,
    ITEM_TOXIC_ORB = 272,
    ITEM_FLAME_ORB = 273,
    ITEM_QUICK_POWDER = 274,
    ITEM_FOCUS_SASH = 275,
    ITEM_ZOOM_LENS = 276,
    ITEM_METRONOME = 277,
    ITEM_IRON_BALL = 278,
    ITEM_LAGGING_TAIL = 279,
    ITEM_DESTINY_KNOT = 280,
    ITEM_BLACK_SLUDGE = 281,
    ITEM_ICY_ROCK = 282,
    ITEM_SMOOTH_ROCK = 283,
    ITEM_HEAT_ROCK = 284,
    ITEM_DAMP_ROCK = 285,
    ITEM_GRIP_CLAW = 286,
    ITEM_CHOICE_SCARF = 287,
    ITEM_STICKY_BARB = 288,
    ITEM_POWER_BRACER = 289,
    ITEM_POWER_BELT = 290,
    ITEM_POWER_LENS = 291,
    ITEM_POWER_BAND = 292,
    ITEM_POWER_ANKLET = 293,
    ITEM_POWER_WEIGHT = 294,
    ITEM_SHED_SHELL = 295,
    ITEM_BIG_ROOT = 296,
    ITEM_CHOICE_SPECS = 297,
    ITEM_FLAME_PLATE = 298,
    ITEM_SPLASH_PLATE = 299,
    ITEM_ZAP_PLATE = 300,
    ITEM_MEADOW_PLATE = 301,
    ITEM_ICICLE_PLATE = 302,
    ITEM_FIST_PLATE = 303,
    ITEM_TOXIC_PLATE = 304,
    ITEM_EARTH_PLATE = 305,
    ITEM_SKY_PLATE = 306,
    ITEM_MIND_PLATE = 307,
    ITEM_INSECT_PLATE = 308,
    ITEM_STONE_PLATE = 309,
    ITEM_SPOOKY_PLATE = 310,
    ITEM_DRACO_PLATE = 311,
    ITEM_DREAD_PLATE = 312,
    ITEM_IRON_PLATE = 313,
    ITEM_ODD_INCENSE = 314,
    ITEM_ROCK_INCENSE = 315,
    ITEM_FULL_INCENSE = 316,
    ITEM_WAVE_INCENSE = 317,
    ITEM_ROSE_INCENSE = 318,
    ITEM_LUCK_INCENSE = 319,
    ITEM_PURE_INCENSE = 320,
    ITEM_PROTECTOR = 321,
    ITEM_ELECTIRIZER = 322,
    ITEM_MAGMARIZER = 323,
    ITEM_DUBIOUS_DISC = 324,
    ITEM_REAPER_CLOTH = 325,
    ITEM_RAZOR_CLAW = 326,
    ITEM_RAZOR_FANG = 327,
    ITEM_TM01 = 328,
    ITEM_TM02 = 329,
    ITEM_TM03 = 330,
    ITEM_TM04 = 331,
    ITEM_TM05 = 332,
    ITEM_TM06 = 333,
    ITEM_TM07 = 334,
    ITEM_TM08 = 335,
    ITEM_TM09 = 336,
    ITEM_TM10 = 337,
    ITEM_TM11 = 338,
    ITEM_TM12 = 339,
    ITEM_TM13 = 340,
    ITEM_TM14 = 341,
    ITEM_TM15 = 342,
    ITEM_TM16 = 343,
    ITEM_TM17 = 344,
    ITEM_TM18 = 345,
    ITEM_TM19 = 346,
    ITEM_TM20 = 347,
    ITEM_TM21 = 348,
    ITEM_TM22 = 349,
    ITEM_TM23 = 350,
    ITEM_TM24 = 351,
    ITEM_TM25 = 352,
    ITEM_TM26 = 353,
    ITEM_TM27 = 354,
    ITEM_TM28 = 355,
    ITEM_TM29 = 356,
    ITEM_TM30 = 357,
    ITEM_TM31 = 358,
    ITEM_TM32 = 359,
    ITEM_TM33 = 360,
    ITEM_TM34 = 361,
    ITEM_TM35 = 362,
    ITEM_TM36 = 363,
    ITEM_TM37 = 364,
    ITEM_TM38 = 365,
    ITEM_TM39 = 366,
    ITEM_TM40 = 367,
    ITEM_TM41 = 368,
    ITEM_TM42 = 369,
    ITEM_TM43 = 370,
    ITEM_TM44 = 371,
    ITEM_TM45 = 372,
    ITEM_TM46 = 373,
    ITEM_TM47 = 374,
    ITEM_TM48 = 375,
    ITEM_TM49 = 376,
    ITEM_TM50 = 377,
    ITEM_TM51 = 378,
    ITEM_TM52 = 379,
    ITEM_TM53 = 380,
    ITEM_TM54 = 381,
    ITEM_TM55 = 382,
    ITEM_TM56 = 383,
    ITEM_TM57 = 384,
    ITEM_TM58 = 385,
    ITEM_TM59 = 386,
    ITEM_TM60 = 387,
    ITEM_TM61 = 388,
    ITEM_TM62 = 389,
    ITEM_TM63 = 390,
    ITEM_TM64 = 391,
    ITEM_TM65 = 392,
    ITEM_TM66 = 393,
    ITEM_TM67 = 394,
    ITEM_TM68 = 395,
    ITEM_TM69 = 396,
    ITEM_TM70 = 397,
    ITEM_TM71 = 398,
    ITEM_TM72 = 399,
    ITEM_TM73 = 400,
    ITEM_TM74 = 401,
    ITEM_TM75 = 402,
    ITEM_TM76 = 403,
    ITEM_TM77 = 404,
    ITEM_TM78 = 405,
    ITEM_TM79 = 406,
    ITEM_TM80 = 407,
    ITEM_TM81 = 408,
    ITEM_TM82 = 409,
    ITEM_TM83 = 410,
    ITEM_TM84 = 411,
    ITEM_TM85 = 412,
    ITEM_TM86 = 413,
    ITEM_TM87 = 414,
    ITEM_TM88 = 415,
    ITEM_TM89 = 416,
    ITEM_TM90 = 417,
    ITEM_TM91 = 418,
    ITEM_TM92 = 419,
    ITEM_HM01 = 420,
    ITEM_HM02 = 421,
    ITEM_HM03 = 422,
    ITEM_HM04 = 423,
    ITEM_HM05 = 424,
    ITEM_HM06 = 425,
    ITEM_HM07 = 426,
    ITEM_HM08 = 427,
    ITEM_EXPLORER_KIT = 428,
    ITEM_LOOT_SACK = 429,
    ITEM_RULE_BOOK = 430,
    ITEM_POKE_RADAR = 431,
    ITEM_POINT_CARD = 432,
    ITEM_JOURNAL = 433,
    ITEM_SEAL_CASE = 434,
    ITEM_FASHION_CASE = 435,
    ITEM_SEAL_BAG = 436,
    ITEM_PAL_PAD = 437,
    ITEM_WORKS_KEY = 438,
    ITEM_OLD_CHARM = 439,
    ITEM_GALACTIC_KEY = 440,
    ITEM_RED_CHAIN = 441,
    ITEM_TOWN_MAP = 442,
    ITEM_VS_SEEKER = 443,
    ITEM_COIN_CASE = 444,
    ITEM_OLD_ROD = 445,
    ITEM_GOOD_ROD = 446,
    ITEM_SUPER_ROD = 447,
    ITEM_SPRAYDUCK = 448,
    ITEM_POFFIN_CASE = 449,
    ITEM_BICYCLE = 450,
    ITEM_SUITE_KEY = 451,
    ITEM_OAKS_LETTER = 452,
    ITEM_LUNAR_WING = 453,
    ITEM_MEMBER_CARD = 454,
    ITEM_AZURE_FLUTE = 455,
    ITEM_S_S_TICKET = 456,
    ITEM_CONTEST_PASS = 457,
    ITEM_MAGMA_STONE = 458,
    ITEM_PARCEL = 459,
    ITEM_COUPON_1 = 460,
    ITEM_COUPON_2 = 461,
    ITEM_COUPON_3 = 462,
    ITEM_STORAGE_KEY = 463,
    ITEM_SECRETPOTION = 464,
    ITEM_VS_RECORDER = 465,
    ITEM_GRACIDEA = 466,
    ITEM_SECRET_KEY = 467,
    MAX_ITEMS = 468,
};

enum PoketchAppID {
    POKETCH_APPID_DIGITALWATCH = 0,
    POKETCH_APPID_CALCULATOR = 1,
    POKETCH_APPID_MEMOPAD = 2,
    POKETCH_APPID_PEDOMETER = 3,
    POKETCH_APPID_PARTYSTATUS = 4,
    POKETCH_APPID_FRIENDSHIPCHECKER = 5,
    POKETCH_APPID_DOWSINGMACHINE = 6,
    POKETCH_APPID_BERRYSEARCHER = 7,
    POKETCH_APPID_DAYCARECHECKER = 8,
    POKETCH_APPID_POKEMONHISTORY = 9,
    POKETCH_APPID_COUNTER = 10,
    POKETCH_APPID_ANALOGWATCH = 11,
    POKETCH_APPID_MARKINGMAP = 12,
    POKETCH_APPID_LINKSEARCHER = 13,
    POKETCH_APPID_COINTOSS = 14,
    POKETCH_APPID_MOVETESTER = 15,
    POKETCH_APPID_CALENDAR = 16,
    POKETCH_APPID_DOTART = 17,
    POKETCH_APPID_ROULETTE = 18,
    POKETCH_APPID_TRAINERCOUNTER = 19,
    POKETCH_APPID_KITCHENTIMER = 20,
    POKETCH_APPID_COLORCHANGER = 21,
    POKETCH_APPID_MATCHUPCHECKER = 22,
    POKETCH_APPID_STOPWATCH = 23,
    POKETCH_APPID_ALARMCLOCK = 24,
    POKETCH_APPID_MAX = 25,
    POKETCH_APPID_NONE = -1,
};

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct BallCapsule {
    BallSeal seals[8];
} BallCapsule;

typedef struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2];
} EasyChatSentence;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

typedef struct Mail {
    u32 trainerID;
    u8 trainerGender;
    u8 language;
    u8 gameVersion;
    u8 mailType;
    charcode_t trainerName[7 + 1];
    MailIconData iconData[3];
    u16 platExclusiveFormIcons;
    EasyChatSentence sentences[3];
} Mail;

typedef struct PokemonDataBlockA {
    u16 species;
    u16 heldItem;

    u32 otID;
    u32 exp;

    u8 friendship;
    u8 ability;
    u8 markings;
    u8 originLanguage;

    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speedEV;
    u8 spAtkEV;
    u8 spDefEV;

    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;

    u32 ribbonsDS1;
} PokemonDataBlockA;

typedef struct PokemonDataBlockB {
    u16 moves[4];
    u8 moveCurrentPPs[4];
    u8 movePPUps[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 hasNickname : 1;
    u32 ribbonsGBA;
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u8 HGSS_shinyLeaves : 6;
    u8 unused1 : 2;
    u16 unused2;
    u16 EggLocation_PtHGSS;
    u16 MetLocation_PtHGSS;
} PokemonDataBlockB;

typedef struct PokemonDataBlockC {
    charcode_t nickname[10 + 1];

    u8 unused;
    u8 originGame;

    u64 ribbonsDS2;
} PokemonDataBlockC;

typedef struct PokemonDataBlockD {
    u16 otName[7 + 1];
    u8 eggYear;
    u8 eggMonth;
    u8 eggDay;
    u8 metYear;
    u8 metMonth;
    u8 metDay;
    u16 EggLocation_DP;
    u16 MetLocation_DP;
    u8 pokerus;
    u8 pokeball;
    u8 metLevel : 7;
    u8 otGender : 1;
    u8 metTerrain;
    u8 HGSS_Pokeball;
    s8 mood;
} PokemonDataBlockD;

typedef union {
    PokemonDataBlockA blockA;
    PokemonDataBlockB blockB;
    PokemonDataBlockC blockC;
    PokemonDataBlockD blockD;
} PokemonDataBlock;

typedef struct BoxPokemon {
    u32 personality;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 checksumFailed : 1;
    u16 unused : 13;
    u16 checksum;
    union { PokemonDataBlockA blockA; PokemonDataBlockB blockB; PokemonDataBlockC blockC; PokemonDataBlockD blockD; } dataBlocks[4];
} BoxPokemon;

typedef struct PartyPokemon {
    u32 status;
    u8 level;
    u8 ballCapsuleID;
    u16 hp;
    u16 maxHP;
    u16 attack;
    u16 defense;
    u16 speed;
    u16 spAtk;
    u16 spDef;
    Mail mail;
    BallCapsule ballCapsule;
} PartyPokemon;

typedef struct Pokemon {
    BoxPokemon box;
    PartyPokemon party;
} Pokemon;

typedef struct BattleRegulation {
    u16 name[12];
    u16 maxTotalLevel;
    u8 teamSize;
    u8 maxLevel;
    s8 heightRestriction;
    s8 weightRestriction;
    u8 evolutionRestriction : 1;
    u8 specialSpeciesRestriction : 1;
    u8 speciesUniqueness : 1;
    u8 itemUniqueness : 1;
    u8 moveRestrictions : 1;
} BattleRegulation;

typedef struct MysteryGiftPokemonData {
    BOOL hasCustomOT;
    Pokemon pokemon;
    u8 specialRibbonsDescIDs[10];
    u8 padding_FA[6];
} MysteryGiftPokemonData;

typedef struct MysteryGiftItemData {
    enum Item item;
    BOOL shouldPlayAnimation;
} MysteryGiftItemData;

enum MysteryGiftCosmeticsType {
    MG_COSMETICS_SEAL = 1,
    MG_COSMETICS_ACCESSORY,
    MG_COSMETICS_BACKDROP,
};

typedef struct MysteryGiftCosmeticsData {
    enum MysteryGiftCosmeticsType type;
    int id;
} MysteryGiftCosmeticsData;

typedef union GiftData {
    MysteryGiftPokemonData pokemonGiftData;
    MysteryGiftItemData itemGiftData;
    int decorationGoodID;
    MysteryGiftCosmeticsData cosmeticGiftData;
    enum PoketchAppID poketchAppID;
    BattleRegulation battleReg;
} GiftData;

typedef struct PGT {
    u16 type;
    u16 wondercardSlot : 2;
    u16 : 14;
    GiftData data;
} PGT;

typedef struct MysteryGiftEventHeader {
    u16 title[36];
    u32 validGames;
    u16 id;
    u8 unique : 1;
    u8 unk_4E_1 : 1;
    u8 hasWonderCard : 1;
    u8 savePgt : 1;
    u8 shareable : 1;
    u8 fromSharing : 1;
    u8 : 2;
    u8 padding_4F;
} MysteryGiftEventHeader;

typedef struct WonderCard {
    u16 tag;
    u16 flag;
    union { struct { BOOL fixedOT; Pokemon mon; u8 ribbons[10]; } pokemon; Pokemon egg; u32 item; struct { u16 name[12]; u16 totalLevel; u8 partyCount; u8 maxLevel; s8 heightLimit; s8 weightLimit; u8 evolvedPokemon : 1; u8 ubersClause : 1; u8 speciesDupeClause : 1; u8 itemDupeClause : 1; u8 dragonRageClause : 1; } ruleset; int baseDecoration; struct { int kind; int id; } monDecoration; u8 pokewalkerCourse; struct { u8 filler_0[4]; u8 gender : 1; u8 iconId : 7; u8 filler_5[2]; u8 numMons; u16 playerName[8]; u16 leadMonNick[12]; u8 avatarStateBak; u16 mapId; u16 x; u16 y; u32 date; u16 hour; u16 min; u16 unk_40[2]; u16 subjectSpriteId; u16 unk_46; struct { fx32 distance; struct { u16 x; u16 y; u16 z; u16 unused; } angle; u16 perspectiveType; u16 perspective; int unk_10[2]; VecFx32 lookAt; } unk_48; struct { u16 species; u8 form; u8 shiny : 1; u8 gender : 1; } party[6]; } photo; u8 raw[256]; } data;
    struct { u16 name[36]; u32 version; u16 id; u8 unique : 1; u8 unk8E_1 : 1; u8 unk8E_2 : 1; u8 unk8E_3 : 1; u8 unk8E_4 : 1; u8 unk8E_5 : 1; u8 unk8E_6 : 2; } unk104;
    u16 text[250];
    u8 shareMax;
    u16 monIcon[3];
    u8 shareCount;
    s32 receiveDate;
} WonderCard;

typedef struct MysteryGift MysteryGift;

typedef struct Sprite Sprite;

typedef struct TrainerInfo TrainerInfo;

typedef struct StringTemplate StringTemplate;

typedef struct MainMenuWindow {
    BOOL scrollable;
    BOOL drawFrame;
    BOOL clearBeforePrinting;
    BOOL textRightAligned;
    Window *window;
    StringTemplate *strTemplate;
    int width;
    int height;
    int textXOffset;
    int textYOffset;
    int baseTile;
    enum BgLayer bgLayer;
    int textPltt;
    int textBank;
    int frameBaseTile;
    int framePltt;
    enum Font font;
    TextColor textColor;
    u8 backgroundColorIdx;
    u8 pad_49[3];
    int textEntryID;
    int renderDelay;
} MainMenuWindow;

void ov74_02235308(BgConfig *bgConfig, enum BgLayer bgLayer, u8 screenSize, u32 screenBase, u32 charBase);

typedef struct GXBanks {
    GXVRamBG unk_00;
    GXVRamBGExtPltt unk_04;
    GXVRamSubBG unk_08;
    GXVRamSubBGExtPltt unk_0C;
    GXVRamOBJ unk_10;
    GXVRamOBJExtPltt unk_14;
    GXVRamSubOBJ unk_18;
    GXVRamSubOBJExtPltt unk_1C;
    GXVRamTex unk_20;
    GXVRamTexPltt unk_24;
} GXBanks;

void GfGfx_SetBanks(const GXBanks *banks);

typedef struct Pokedex Pokedex;

typedef struct WaitDial WaitDial;

enum StandardWindowType {
    STANDARD_WINDOW_SYSTEM = 0,
    STANDARD_WINDOW_FIELD,
    STANDARD_WINDOW_UNDERGROUND,
};

void LoadUserFrameGfx1(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset, u8 palOffset, u8 standardWindowType, u32 heapID);

void LoadUserFrameGfx2(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset, u8 palOffset, u8 messageBoxFrame, u32 heapID);

enum RangerLinkRangerCommState {
    RANGER_COMM_STATE_INIT = 0,
    RANGER_COMM_STATE_WAIT_READY,
    RANGER_COMM_STATE_2,
    RANGER_COMM_STATE_3,
    RANGER_COMM_STATE_4,
    RANGER_COMM_STATE_UNUSED_5,
    RANGER_COMM_STATE_UNUSED_6,
    RANGER_COMM_STATE_7,
    RANGER_COMM_STATE_8,
    RANGER_COMM_STATE_9,
    RANGER_COMM_STATE_WAIT_RANGER_RESPONSE,
    RANGER_COMM_STATE_11,
    RANGER_COMM_STATE_12,
    RANGER_COMM_STATE_13,
    RANGER_COMM_STATE_14,
    RANGER_COMM_STATE_15,
    RANGER_COMM_STATE_16,
    RANGER_COMM_STATE_17,
    RANGER_COMM_STATE_18,
    RANGER_COMM_STATE_TRANSFER_COMPLETED,
    RANGER_COMM_STATE_WAIT_TRANSFER_COMPLETED,
    RANGER_COMM_STATE_UNUSED_21,
    RANGER_COMM_STATE_GOTO_CONNECTION_LOST,
    RANGER_COMM_STATE_CONNECTION_LOST,
    RANGER_COMM_STATE_UNUSED_24,
    RANGER_COMM_STATE_UNUSED_25,
    RANGER_COMM_STATE_CONNECTION_CANCELED,
    RANGER_COMM_STATE_DONE,
    RANGER_COMM_STATE_SHUTDOWN_WIRELESS_AND_REBOOT,
    RANGER_COMM_STATE_29,
    RANGER_COMM_STATE_30
};

typedef struct RangerLinkManager {
    enum HeapID heapID;
    BgConfig *bgConfig;
    SaveData *saveData;
    Pokedex *pokedex;
    TrainerInfo *trainerInfo;
    Options *options;
    Window stdWindow;
    Window messageBoxWindow;
    Window appNumberStdWindow;
    struct { BOOL scrollable; BOOL drawFrame; BOOL clearBeforePrinting; BOOL textRightAligned; struct Window *window; struct StringTemplate *strTemplate; int width; int height; int textXOffset; int textYOffset; int baseTile; u32 bgLayer; int textPltt; int textBank; int frameBaseTile; int framePltt; u32 font; u32 textColor; u8 backgroundColorIdx; u8 unk_4C_pad[3]; int textEntryID; int renderDelay; } messageBox;
    struct { BOOL scrollable; BOOL drawFrame; BOOL clearBeforePrinting; BOOL textRightAligned; struct Window *window; struct StringTemplate *strTemplate; int width; int height; int textXOffset; int textYOffset; int baseTile; u32 bgLayer; int textPltt; int textBank; int frameBaseTile; int framePltt; u32 font; u32 textColor; u8 backgroundColorIdx; u8 unk_4C_pad[3]; int textEntryID; int renderDelay; } manaphyEggTransferWindow;
    struct { BOOL scrollable; BOOL drawFrame; BOOL clearBeforePrinting; BOOL textRightAligned; struct Window *window; struct StringTemplate *strTemplate; int width; int height; int textXOffset; int textYOffset; int baseTile; u32 bgLayer; int textPltt; int textBank; int frameBaseTile; int framePltt; u32 font; u32 textColor; u8 backgroundColorIdx; u8 unk_4C_pad[3]; int textEntryID; int renderDelay; } playerCommsDetails;
    int nextWindowBasetile;
    BOOL wirelessStarted;
    int unused_14C;
    int wirelessStatus;
    int unused_154;
    int wirelessActive;
    enum RangerLinkRangerCommState wirelessState;
    int timeout;
    int unk_164;
    int unk_168;
    u8 wirelessCommBuffer[12288];
    ApplicationManager *appMan;
    Sprite *downloadArrow;
    MysteryGift *mysteryGift;
    int unused_3178;
    int deliveryManOverloadedPrinterID;
    struct { u16 tag; u16 flag; u8 data[256]; struct { u16 name[36]; u32 version; u16 id; u8 unique : 1; u8 unk8E_1 : 1; u8 unk8E_2 : 1; u8 unk8E_3 : 1; u8 unk8E_4 : 1; u8 unk8E_5 : 1; u8 unk8E_6 : 2; } unk104; u16 text[250]; u8 shareMax; u8 unk_34A_pad[1]; u16 monIcon[3]; u8 shareCount; u8 unk_354_pad[3]; s32 receiveDate; } manaphyEggWonderCard;
    WaitDial *waitDial;
} RangerLinkManager;

void ov74_02228E98(RangerLinkManager *param0);

void ov74_02228D64(RangerLinkManager *param0);

extern const GXBanks _0223B340;

extern const GraphicsModes _0223B330;

void ov74_02228D64(RangerLinkManager *param0) {
    GXBanks banks = _0223B340;
    GraphicsModes graphicsModes = _0223B330;

    GfGfx_SetBanks(&banks);

    SetBothScreensModesAndDisable(&graphicsModes);
    ov74_02235308(param0->bgConfig, BG_LAYER_MAIN_0, BG_SCREEN_SIZE_256x256, GX_BG_SCRBASE_0xf000 * 0x800, GX_BG_CHARBASE_0x00000 * 0x4000);
    ov74_02235308(param0->bgConfig, BG_LAYER_MAIN_1, BG_SCREEN_SIZE_256x256, GX_BG_SCRBASE_0xf800 * 0x800, GX_BG_CHARBASE_0x04000 * 0x4000);
    ov74_02235308(param0->bgConfig, BG_LAYER_SUB_0, BG_SCREEN_SIZE_256x256, GX_BG_SCRBASE_0x7800 * 0x800, GX_BG_CHARBASE_0x00000 * 0x4000);
    ov74_02235308(param0->bgConfig, BG_LAYER_SUB_1, BG_SCREEN_SIZE_256x256, GX_BG_SCRBASE_0x7000 * 0x800, GX_BG_CHARBASE_0x04000 * 0x4000);

    ResetAllTextPrinters();
    LoadFontPal0(PAL_LOAD_MAIN_BG, ((0) * ((16) * sizeof(u16))), param0->heapID);
    LoadUserFrameGfx1(param0->bgConfig, BG_LAYER_MAIN_0, 1, 1, STANDARD_WINDOW_SYSTEM, param0->heapID);

    int frameType = Options_GetFrame(param0->options);

    LoadUserFrameGfx2(param0->bgConfig, BG_LAYER_MAIN_0, (1 + (3 * 3)), 2, frameType, param0->heapID);
    GfGfxLoader_GXLoadPal((enum NarcID)113, 0, PAL_LOAD_MAIN_BG, ((8) * ((16) * sizeof(u16))), ((16) * sizeof(u16)), param0->heapID);
    GfGfxLoader_LoadCharData((enum NarcID)113, 1, param0->bgConfig, BG_LAYER_MAIN_1, 0, 160 * (8 * (8 / 2)), 1, param0->heapID);
    GfGfxLoader_LoadScrnData((enum NarcID)113, 2, param0->bgConfig, BG_LAYER_MAIN_1, 0, 32 * 24 * sizeof(u16), 1, param0->heapID);
    BgTilemapRectChangePalette(param0->bgConfig, BG_LAYER_MAIN_1, 0, 0, 32, 24, 8);
    BgCommitTilemapBufferToVram(param0->bgConfig, BG_LAYER_MAIN_1);

    ov74_02228E98(param0);
}
