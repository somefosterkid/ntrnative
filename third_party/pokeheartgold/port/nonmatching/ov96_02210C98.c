/* PORTOK hk42 */
#include "global.h"

struct BgConfig;
struct BgTemplate;
struct GXBanks;
struct GraphicsModes;
struct HeapParam;
struct MessageLoader;
struct String;
struct SysTask;
struct SysTaskManager;
struct System;
struct TextPrinterTemplate;
struct Window;
struct WindowTemplate;
enum DSScreen {
    DS_SCREEN_MAIN = 0,
    DS_SCREEN_SUB,
    DS_SCREEN_MAX,
};
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
typedef struct BgTemplate {
    u32 x;
    u32 y;
    u32 bufferSize;
    u32 baseTile;
    u8 screenSize;
    u8 colorMode;
    u8 screenBase;
    u8 charBase;
    u8 bgExtPltt;
    u8 priority;
    u8 areaOver;
    BOOL mosaic;
} BgTemplate;
typedef struct BgConfig BgConfig;
typedef struct WindowTemplate {
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile;
} WindowTemplate;
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
BgConfig *BgConfig_Alloc(enum HeapID heapID);
void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);
void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);
void BG_SetMaskColor();
void BgClearTilemapBufferAndCommit(BgConfig *bgConfig);
void AddWindow(BgConfig *bgConfig, Window *window, const WindowTemplate *template);
void RemoveWindow(Window *window);
void FillWindowPixelRect(Window *window, u8 color, u16 x, u16 y);
void SetBlendBrightness(s16 brightness, int planeMask, u32 screenSelect);
typedef struct String String;
String *String_New(u8 size, u32 heapID);
void String_Delete(String *string);
BOOL sub_02038D90(void);
BOOL sub_02039AA4(void);
BOOL sub_02036144(void);
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
typedef struct TextPrinterTemplate TextPrinterTemplate;
typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);
enum Font {
    FONT_SYSTEM = 0,
    FONT_MESSAGE,
    FONT_SUBSCREEN,
    FONT_UNOWN,

    FONT_MAX,
};
void LoadFontPal0(enum PaletteLoadLocation palLocation, u32 palSlotOffset, enum HeapID heapID);
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
void GfGfx_SetBanks();
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void GfGfx_BothDispOn(void);
void GXLayers_SwapDisplay(void);
typedef struct HeapParam {
    u32 size;
    OSArenaId arena;
} HeapParam;
void Heap_InitSystem(const HeapParam *templates, u32 nTemplates, u32 totalNumHeaps, u32 preSize);
void Heap_Free();
void HandleDSLidAction(void);
enum TextBank {
    TEXT_BANK_MOVES_USED_IN_BATTLE                       =   0,
    TEXT_BANK_DIPLOMA                                    =   1,
    TEXT_BANK_BATTLE_BAG                                 =   2,
    TEXT_BANK_BATTLE_PARTY                               =   3,
    TEXT_BANK_UNK_0004                                   =   4,
    TEXT_BANK_UNK_0005                                   =   5,
    TEXT_BANK_UNK_0006                                   =   6,
    TEXT_BANK_BAG                                        =   7,
    TEXT_BANK_UNK_0008                                   =   8,
    TEXT_BANK_UNK_0009                                   =   9,
    TEXT_BANK_UNK_0010                                   =  10,
    TEXT_BANK_COMMUNICATION_CLUB                         =  11,
    TEXT_BANK_BALL_SEAL_NAMES                            =  12,
    TEXT_BANK_BALL_SEAL_NAMES_PLURAL                     =  13,
    TEXT_BANK_MAIN_MENU_ALERTS                           =  14,
    TEXT_BANK_WFC_FACILITY_SELECTOR                      =  15,
    TEXT_BANK_BATTLE_FRONTIER_RECORDS                    =  16,
    TEXT_BANK_BG_EVENTS                                  =  17,
    TEXT_BANK_POKEMON_STORAGE_SYSTEM                     =  18,
    TEXT_BANK_BOX_MESSAGES                               =  19,
    TEXT_BANK_UNK_0020                                   =  20,
    TEXT_BANK_FRONTIER_TRAINER_NAMES                     =  21,
    TEXT_BANK_BATTLE_TOWER_RECORDS_APP                   =  22,
    TEXT_BANK_JUBILIFE_CITY                              =  23,
    TEXT_BANK_JUBILIFE_CITY_MART                         =  24,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_1               =  25,
    TEXT_BANK_JUBILIFE_CITY_POKECENTER_1F                =  26,
    TEXT_BANK_POKETCH_CO_1F                              =  27,
    TEXT_BANK_POKETCH_CO_2F                              =  28,
    TEXT_BANK_POKETCH_CO_3F                              =  29,
    TEXT_BANK_JUBILIFE_TV_1F                             =  30,
    TEXT_BANK_JUBILIFE_TV_2F                             =  31,
    TEXT_BANK_JUBILIFE_TV_3F                             =  32,
    TEXT_BANK_JUBILIFE_TV_4F                             =  33,
    TEXT_BANK_JUBILIFE_TV_2F_GALLERY                     =  34,
    TEXT_BANK_JUBILIFE_TV_3F_GLOBAL_RANKING_ROOM         =  35,
    TEXT_BANK_JUBILIFE_TV_3F_GROUP_RANKING_ROOM          =  36,
    TEXT_BANK_JUBILIFE_TV_ELEVATOR                       =  37,
    TEXT_BANK_JUBILIFE_CITY_SOUTH_HOUSE_1F               =  38,
    TEXT_BANK_JUBILIFE_CITY_SOUTH_HOUSE_2F               =  39,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_SOUTH_HOUSE_3F        =  40,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_2               =  41,
    TEXT_BANK_JUBILIFE_CITY_CONDOMINIUMS_1F              =  42,
    TEXT_BANK_JUBILIFE_CITY_CONDOMINIUMS_2F              =  43,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_3F       =  44,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_CONDOMINIUMS_4F       =  45,
    TEXT_BANK_GLOBAL_TERMINAL_1F                         =  46,
    TEXT_BANK_GLOBAL_TERMINAL_2F                         =  47,
    TEXT_BANK_GLOBAL_TERMINAL_3F                         =  48,
    TEXT_BANK_TRAINERS_SCHOOL                            =  49,
    TEXT_BANK_JUBILIFE_CITY_SOUTHWEST_HOUSE_1F           =  50,
    TEXT_BANK_JUBILIFE_CITY_SOUTHWEST_HOUSE_2F           =  51,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_3               =  52,
    TEXT_BANK_UNUSED_JUBILIFE_CITY_HOUSE_4               =  53,
    TEXT_BANK_CANALAVE_CITY                              =  54,
    TEXT_BANK_CANALAVE_CITY_MART                         =  55,
    TEXT_BANK_CANALAVE_CITY_GYM                          =  56,
    TEXT_BANK_CANALAVE_CITY_POKECENTER_1F                =  57,
    TEXT_BANK_CANALAVE_LIBRARY_1F                        =  58,
    TEXT_BANK_CANALAVE_LIBRARY_2F                        =  59,
    TEXT_BANK_CANALAVE_LIBRARY_3F                        =  60,
    TEXT_BANK_CANALAVE_CITY_SOUTHEAST_HOUSE              =  61,
    TEXT_BANK_CANALAVE_CITY_EAST_HOUSE                   =  62,
    TEXT_BANK_CANALAVE_CITY_HARBOR_INN                   =  63,
    TEXT_BANK_CANALAVE_CITY_SAILOR_ELDRITCH_HOUSE        =  64,
    TEXT_BANK_OREBURGH_CITY                              =  65,
    TEXT_BANK_OREBURGH_CITY_MART                         =  66,
    TEXT_BANK_OREBURGH_CITY_GYM                          =  67,
    TEXT_BANK_OREBURGH_CITY_POKECENTER_1F                =  68,
    TEXT_BANK_OREBURGH_CITY_POKECENTER_B1F               =  69,
    TEXT_BANK_OREBURGH_CITY_NORTHWEST_HOUSE_1F           =  70,
    TEXT_BANK_OREBURGH_CITY_NORTHWEST_HOUSE_2F           =  71,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_3F    =  72,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTHWEST_HOUSE_4F    =  73,
    TEXT_BANK_OREBURGH_CITY_NORTH_HOUSE_1F               =  74,
    TEXT_BANK_OREBURGH_CITY_NORTH_HOUSE_2F               =  75,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTH_HOUSE_3F        =  76,
    TEXT_BANK_UNUSED_OREBURGH_CITY_NORTH_HOUSE_4F        =  77,
    TEXT_BANK_OREBURGH_CITY_MIDDLE_HOUSE                 =  78,
    TEXT_BANK_MINING_MUSEUM                              =  79,
    TEXT_BANK_OREBURGH_CITY_WEST_HOUSE                   =  80,
    TEXT_BANK_OREBURGH_CITY_EAST_HOUSE_1F                =  81,
    TEXT_BANK_OREBURGH_CITY_EAST_HOUSE_2F                =  82,
    TEXT_BANK_UNUSED_OREBURGH_CITY_EAST_HOUSE_3F         =  83,
    TEXT_BANK_OREBURGH_CITY_SOUTH_HOUSE                  =  84,
    TEXT_BANK_ETERNA_CITY                                =  85,
    TEXT_BANK_ETERNA_CITY_MART                           =  86,
    TEXT_BANK_ETERNA_CITY_GYM                            =  87,
    TEXT_BANK_ETERNA_CITY_DP_GYM                         =  88,
    TEXT_BANK_ETERNA_CITY_POKECENTER_1F                  =  89,
    TEXT_BANK_ETERNA_CITY_POKECENTER_2F                  =  90,
    TEXT_BANK_CYCLE_SHOP                                 =  91,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_1F           =  92,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_2F           =  93,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_3F           =  94,
    TEXT_BANK_TEAM_GALACTIC_ETERNA_BUILDING_4F           =  95,
    TEXT_BANK_ROTOMS_ROOM                                =  96,
    TEXT_BANK_ETERNA_CITY_CONDOMINIUMS_1F                =  97,
    TEXT_BANK_ETERNA_CITY_CONDOMINIUMS_2F                =  98,
    TEXT_BANK_ETERNA_CITY_CONDOMINIUMS_3F                =  99,
    TEXT_BANK_UNUSED_ETERNA_CITY_CONDOMINIUMS_4F         = 100,
    TEXT_BANK_ROUTE_206_CYCLING_ROAD_NORTH_GATE          = 101,
    TEXT_BANK_ETERNA_CITY_HERB_SHOP                      = 102,
    TEXT_BANK_ETERNA_CITY_SOUTH_HOUSE                    = 103,
    TEXT_BANK_ETERNA_CITY_EAST_HOUSE                     = 104,
    TEXT_BANK_ETERNA_CITY_UNDERGROUND_MAN_HOUSE          = 105,
    TEXT_BANK_HEARTHOME_CITY                             = 106,
    TEXT_BANK_HEARTHOME_CITY_MART                        = 107,
    TEXT_BANK_HEARTHOME_CITY_GYM_ENTRANCE_ROOM           = 108,
    TEXT_BANK_HEARTHOME_CITY_GYM_LEADER_ROOM             = 109,
    TEXT_BANK_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_1      = 110,
    TEXT_BANK_HEARTHOME_CITY_DP_GYM_ELEVATOR_ROOM_2      = 111,
    TEXT_BANK_HEARTHOME_CITY_DP_GYM_LEADER_ROOM          = 112,
    TEXT_BANK_HEARTHOME_CITY_POKECENTER_1F               = 113,
    TEXT_BANK_HEARTHOME_CITY_SOUTHEAST_HOUSE_1F          = 114,
    TEXT_BANK_HEARTHOME_CITY_SOUTHEAST_HOUSE_2F          = 115,
    TEXT_BANK_HEARTHOME_CITY_SOUTHEAST_HOUSE_ELEVATOR    = 116,
    TEXT_BANK_HEARTHOME_CITY_POKEMON_FAN_CLUB            = 117,
    TEXT_BANK_HEARTHOME_CITY_WEST_GATE_TO_AMITY_SQUARE   = 118,
    TEXT_BANK_HEARTHOME_CITY_EAST_GATE_TO_AMITY_SQUARE   = 119,
    TEXT_BANK_ROUTE_208_GATE_TO_HEARTHOME_CITY           = 120,
    TEXT_BANK_ROUTE_209_GATE_TO_HEARTHOME_CITY           = 121,
    TEXT_BANK_ROUTE_212_GATE_TO_HEARTHOME_CITY           = 122,
    TEXT_BANK_HEARTHOME_CITY_NORTHEAST_HOUSE_1F          = 123,
    TEXT_BANK_HEARTHOME_CITY_NORTHEAST_HOUSE_2F          = 124,
    TEXT_BANK_HEARTHOME_CITY_NORTHEAST_HOUSE_ELEVATOR    = 125,
    TEXT_BANK_HEARTHOME_CITY_NORTHWEST_HOUSE             = 126,
    TEXT_BANK_POFFIN_HOUSE                               = 127,
    TEXT_BANK_CONTEST_HALL_LOBBY                         = 128,
    TEXT_BANK_CONTEST_HALL_STAGE_NO_CONTEST              = 129,
    TEXT_BANK_FOREIGN_BUILDING                           = 130,
    TEXT_BANK_PASTORIA_CITY                              = 131,
    TEXT_BANK_PASTORIA_CITY_MART                         = 132,
    TEXT_BANK_PASTORIA_CITY_GYM                          = 133,
    TEXT_BANK_PASTORIA_CITY_POKECENTER_1F                = 134,
    TEXT_BANK_PASTORIA_CITY_POKECENTER_2F                = 135,
    TEXT_BANK_PASTORIA_CITY_OBSERVATORY_GATE_1F          = 136,
    TEXT_BANK_PASTORIA_CITY_OBSERVATORY_GATE_2F          = 137,
    TEXT_BANK_PASTORIA_CITY_SOUTHWEST_HOUSE              = 138,
    TEXT_BANK_PASTORIA_CITY_MIDDLE_HOUSE                 = 139,
    TEXT_BANK_PASTORIA_CITY_EAST_HOUSE                   = 140,
    TEXT_BANK_PASTORIA_CITY_NORTH_HOUSE                  = 141,
    TEXT_BANK_PASTORIA_CITY_NORTHEAST_HOUSE              = 142,
    TEXT_BANK_VEILSTONE_CITY                             = 143,
    TEXT_BANK_VEILSTONE_CITY_GYM                         = 144,
    TEXT_BANK_VEILSTONE_CITY_POKECENTER_1F               = 145,
    TEXT_BANK_VEILSTONE_CITY_POKECENTER_B1F              = 146,
    TEXT_BANK_GAME_CORNER                                = 147,
    TEXT_BANK_VEILSTONE_STORE_1F                         = 148,
    TEXT_BANK_VEILSTONE_STORE_2F                         = 149,
    TEXT_BANK_VEILSTONE_STORE_3F                         = 150,
    TEXT_BANK_VEILSTONE_STORE_4F                         = 151,
    TEXT_BANK_VEILSTONE_STORE_5F                         = 152,
    TEXT_BANK_VEILSTONE_STORE_ELEVATOR                   = 153,
    TEXT_BANK_VEILSTONE_STORE_B1F                        = 154,
    TEXT_BANK_VEILSTONE_CITY_GALACTIC_WAREHOUSE          = 155,
    TEXT_BANK_VEILSTONE_CITY_PRIZE_EXCHANGE              = 156,
    TEXT_BANK_VEILSTONE_CITY_SOUTHEAST_HOUSE             = 157,
    TEXT_BANK_VEILSTONE_CITY_NORTHWEST_HOUSE             = 158,
    TEXT_BANK_VEILSTONE_CITY_NORTHEAST_HOUSE             = 159,
    TEXT_BANK_VEILSTONE_CITY_SOUTHWEST_HOUSE             = 160,
    TEXT_BANK_ROUTE_215_GATE_TO_VEILSTONE_CITY           = 161,
    TEXT_BANK_SUNYSHORE_CITY                             = 162,
    TEXT_BANK_SUNYSHORE_CITY_MART                        = 163,
    TEXT_BANK_SUNYSHORE_CITY_GYM_ROOM_1                  = 164,
    TEXT_BANK_SUNYSHORE_CITY_GYM_ROOM_3                  = 165,
    TEXT_BANK_SUNYSHORE_CITY_POKECENTER_1F               = 166,
    TEXT_BANK_SUNYSHORE_MARKET                           = 167,
    TEXT_BANK_SUNYSHORE_CITY_NORTHEAST_HOUSE             = 168,
    TEXT_BANK_SUNYSHORE_CITY_WEST_HOUSE                  = 169,
    TEXT_BANK_SUNYSHORE_CITY_NORTHWEST_HOUSE             = 170,
    TEXT_BANK_UNUSED_SUNYSHORE_CITY_HOUSE_1              = 171,
    TEXT_BANK_UNUSED_SUNYSHORE_CITY_HOUSE_2              = 172,
    TEXT_BANK_SUNYSHORE_CITY_EAST_HOUSE                  = 173,
    TEXT_BANK_VISTA_LIGHTHOUSE                           = 174,
    TEXT_BANK_SUNYSHORE_CITY_EAST_HOUSE_UNUSED           = 175,
    TEXT_BANK_SNOWPOINT_CITY                             = 176,
    TEXT_BANK_SNOWPOINT_CITY_MART                        = 177,
    TEXT_BANK_SNOWPOINT_CITY_GYM                         = 178,
    TEXT_BANK_SNOWPOINT_CITY_POKECENTER_1F               = 179,
    TEXT_BANK_SNOWPOINT_CITY_WEST_HOUSE                  = 180,
    TEXT_BANK_SNOWPOINT_CITY_EAST_HOUSE                  = 181,
    TEXT_BANK_POKEMON_LEAGUE                             = 182,
    TEXT_BANK_POKEMON_LEAGUE_SOUTH_POKECENTER_1F         = 183,
    TEXT_BANK_POKEMON_LEAGUE_NORTH_POKECENTER_1F         = 184,
    TEXT_BANK_POKEMON_LEAGUE_AARON_ROOM                  = 185,
    TEXT_BANK_POKEMON_LEAGUE_BERTHA_ROOM                 = 186,
    TEXT_BANK_POKEMON_LEAGUE_FLINT_ROOM                  = 187,
    TEXT_BANK_POKEMON_LEAGUE_LUCIAN_ROOM                 = 188,
    TEXT_BANK_POKEMON_LEAGUE_CHAMPION_ROOM               = 189,
    TEXT_BANK_POKEMON_LEAGUE_HALLWAY_TO_HALL_OF_FAME     = 190,
    TEXT_BANK_POKEMON_LEAGUE_HALL_OF_FAME                = 191,
    TEXT_BANK_FIGHT_AREA                                 = 192,
    TEXT_BANK_FIGHT_AREA_MART                            = 193,
    TEXT_BANK_FIGHT_AREA_POKECENTER_1F                   = 194,
    TEXT_BANK_UNUSED_BATTLE_PARK_GATE_TO_FIGHT_AREA      = 195,
    TEXT_BANK_ROUTE_225_GATE_TO_FIGHT_AREA               = 196,
    TEXT_BANK_FIGHT_AREA_MIDDLE_HOUSE                    = 197,
    TEXT_BANK_FIGHT_AREA_SOUTH_HOUSE                     = 198,
    TEXT_BANK_BATTLE_CASTLE_SELF_APP                     = 199,
    TEXT_BANK_BATTLE_CASTLE_SCENE                        = 200,
    TEXT_BANK_BATTLE_CASTLE_OPPONENT_APP                 = 201,
    TEXT_BANK_NATURE_NAMES                               = 202,
    TEXT_BANK_POKEMON_CENTER_DAILY_TRAINERS              = 203,
    TEXT_BANK_CONTEST_TEXT                               = 204,
    TEXT_BANK_CONTEST_OPPONENT_NAMES                     = 205,
    TEXT_BANK_CONTEST_DANCE_COMPETITION                  = 206,
    TEXT_BANK_CONTEST_JUDGE_NAMES                        = 207,
    TEXT_BANK_LINK_CONTEST_RECORDS                       = 208,
    TEXT_BANK_CONTEST_VISUAL_COMPETITION                 = 209,
    TEXT_BANK_CONTEST_EFFECTS                            = 210,
    TEXT_BANK_CONTEST_ACTING_COMPETITION                 = 211,
    TEXT_BANK_UNK_0212                                   = 212,
    TEXT_BANK_COMMON_STRINGS                             = 213,
    TEXT_BANK_NETWORK_ERRORS                             = 214,
    TEXT_BANK_DUMMY_0215                                 = 215,
    TEXT_BANK_DUMMY_0216                                 = 216,
    TEXT_BANK_CONTEST_COMMON                             = 217,
    TEXT_BANK_CONTEST_RESULTS                            = 218,
    TEXT_BANK_STATUS_CONDITION_NAMES                     = 219,
    TEXT_BANK_OPTIONS_MENU                               = 220,
    TEXT_BANK_POKEMON_CENTER_2F_COMMON                   = 221,
    TEXT_BANK_OREBURGH_MINE_B1F                          = 222,
    TEXT_BANK_OREBURGH_MINE_B2F                          = 223,
    TEXT_BANK_VALLEY_WINDWORKS_OUTSIDE                   = 224,
    TEXT_BANK_VALLEY_WINDWORKS_BUILDING                  = 225,
    TEXT_BANK_ETERNA_FOREST_OUTSIDE                      = 226,
    TEXT_BANK_ETERNA_FOREST                              = 227,
    TEXT_BANK_FUEGO_IRONWORKS_OUTSIDE                    = 228,
    TEXT_BANK_FUEGO_IRONWORKS_BUILDING                   = 229,
    TEXT_BANK_MT_CORONET_1F_SOUTH                        = 230,
    TEXT_BANK_MT_CORONET_2F                              = 231,
    TEXT_BANK_MT_CORONET_1F_TUNNEL_ROOM                  = 232,
    TEXT_BANK_MT_CORONET_1F_NORTH_ROOM_1                 = 233,
    TEXT_BANK_SPEAR_PILLAR                               = 234,
    TEXT_BANK_SPEAR_PILLAR_DISTORTED                     = 235,
    TEXT_BANK_HALL_OF_ORIGIN                             = 236,
    TEXT_BANK_SPEAR_PILLAR_DIALGA                        = 237,
    TEXT_BANK_SPEAR_PILLAR_PALKIA                        = 238,
    TEXT_BANK_GREAT_MARSH_1_UNUSED                       = 239,
    TEXT_BANK_GREAT_MARSH_1                              = 240,
    TEXT_BANK_GREAT_MARSH_2                              = 241,
    TEXT_BANK_GREAT_MARSH_3                              = 242,
    TEXT_BANK_GREAT_MARSH_4                              = 243,
    TEXT_BANK_GREAT_MARSH_5                              = 244,
    TEXT_BANK_GREAT_MARSH_6                              = 245,
    TEXT_BANK_SOLACEON_RUINS_MANIAC_TUNNEL_ROOM          = 246,
    TEXT_BANK_SOLACEON_RUINS_ROOM_1                      = 247,
    TEXT_BANK_SOLACEON_RUINS_ROOM_2                      = 248,
    TEXT_BANK_SOLACEON_RUINS_ROOM_7                      = 249,
    TEXT_BANK_VICTORY_ROAD_1F                            = 250,
    TEXT_BANK_VICTORY_ROAD_1F_ROOM_2                     = 251,
    TEXT_BANK_PAL_PARK                                   = 252,
    TEXT_BANK_AMITY_SQUARE                               = 253,
    TEXT_BANK_UNK_0254                                   = 254,
    TEXT_BANK_FLOAROMA_MEADOW                            = 255,
    TEXT_BANK_FLOAROMA_MEADOW_HOUSE                      = 256,
    TEXT_BANK_OREBURGH_GATE_1F                           = 257,
    TEXT_BANK_OREBURGH_GATE_B1F                          = 258,
    TEXT_BANK_FULLMOON_ISLAND                            = 259,
    TEXT_BANK_FULLMOON_ISLAND_FOREST                     = 260,
    TEXT_BANK_STARK_MOUNTAIN_OUTSIDE                     = 261,
    TEXT_BANK_STARK_MOUNTAIN_ROOM_1                      = 262,
    TEXT_BANK_STARK_MOUNTAIN_ROOM_2                      = 263,
    TEXT_BANK_STARK_MOUNTAIN_ROOM_3                      = 264,
    TEXT_BANK_SENDOFF_SPRING                             = 265,
    TEXT_BANK_TURNBACK_CAVE_ENTRANCE                     = 266,
    TEXT_BANK_TURNBACK_CAVE_PILLAR_ROOM                  = 267,
    TEXT_BANK_TURNBACK_CAVE_GIRATINA_ROOM                = 268,
    TEXT_BANK_FLOWER_PARADISE                            = 269,
    TEXT_BANK_SNOWPOINT_TEMPLE_1F                        = 270,
    TEXT_BANK_SNOWPOINT_TEMPLE_B5F                       = 271,
    TEXT_BANK_WAYWARD_CAVE_1F                            = 272,
    TEXT_BANK_RUIN_MANIAC_CAVE_SHORT                     = 273,
    TEXT_BANK_RUIN_MANIAC_CAVE_LONG                      = 274,
    TEXT_BANK_MANIAC_TUNNEL                              = 275,
    TEXT_BANK_IRON_ISLAND                                = 276,
    TEXT_BANK_IRON_ISLAND_B2F_LEFT_ROOM                  = 277,
    TEXT_BANK_IRON_ISLAND_HOUSE                          = 278,
    TEXT_BANK_OLD_CHATEAU                                = 279,
    TEXT_BANK_OLD_CHATEAU_SIDE_ROOMS                     = 280,
    TEXT_BANK_OLD_CHATEAU_BACK_MIDDLE_WEST_ROOM          = 281,
    TEXT_BANK_GALACTIC_HQ_1F                             = 282,
    TEXT_BANK_GALACTIC_HQ_2F                             = 283,
    TEXT_BANK_GALACTIC_HQ_3F                             = 284,
    TEXT_BANK_GALACTIC_HQ_4F                             = 285,
    TEXT_BANK_GALACTIC_HQ_B1F                            = 286,
    TEXT_BANK_GALACTIC_HQ_B2F                            = 287,
    TEXT_BANK_GALACTIC_HQ_CONTROL_ROOM                   = 288,
    TEXT_BANK_GALACTIC_HQ_LABORATORY                     = 289,
    TEXT_BANK_GALACTIC_HQ_HALL                           = 290,
    TEXT_BANK_LAKE_VERITY_LOW_WATER                      = 291,
    TEXT_BANK_LAKE_VERITY                                = 292,
    TEXT_BANK_VERITY_CAVERN                              = 293,
    TEXT_BANK_LAKE_VALOR_DRAINED                         = 294,
    TEXT_BANK_LAKE_VALOR                                 = 295,
    TEXT_BANK_VALOR_CAVERN                               = 296,
    TEXT_BANK_LAKE_ACUITY_LOW_WATER                      = 297,
    TEXT_BANK_LAKE_ACUITY                                = 298,
    TEXT_BANK_ACUITY_CAVERN                              = 299,
    TEXT_BANK_NEWMOON_ISLAND                             = 300,
    TEXT_BANK_NEWMOON_ISLAND_FOREST                      = 301,
    TEXT_BANK_UNUSED_BATTLE_PARK                         = 302,
    TEXT_BANK_UNUSED_BATTLE_PARK_EXCHANGE_SERVICE_CORNER = 303,
    TEXT_BANK_BATTLE_TOWER                               = 304,
    TEXT_BANK_BATTLE_TOWER_BATTLE_ROOM                   = 305,
    TEXT_BANK_BATTLE_TOWER_MULTI_BATTLE_ROOM             = 306,
    TEXT_BANK_BATTLE_TOWER_BATTLE_SALON                  = 307,
    TEXT_BANK_BATTLE_FRONTIER                            = 308,
    TEXT_BANK_BATTLE_FRONTIER_GATE_TO_FIGHT_AREA         = 309,
    TEXT_BANK_BATTLE_FACTORY                             = 310,
    TEXT_BANK_BATTLE_HALL                                = 311,
    TEXT_BANK_BATTLE_CASTLE                              = 312,
    TEXT_BANK_BATTLE_ARCADE                              = 313,
    TEXT_BANK_DISTORTION_WORLD_1F                        = 314,
    TEXT_BANK_DISTORTION_WORLD_B1F                       = 315,
    TEXT_BANK_DISTORTION_WORLD_B2F                       = 316,
    TEXT_BANK_DISTORTION_WORLD_B3F                       = 317,
    TEXT_BANK_DISTORTION_WORLD_B6F                       = 318,
    TEXT_BANK_DISTORTION_WORLD_B7F                       = 319,
    TEXT_BANK_DISTORTION_WORLD_GIRATINA_ROOM             = 320,
    TEXT_BANK_DISTORTION_WORLD_TURNBACK_CAVE_ROOM        = 321,
    TEXT_BANK_IRON_RUINS                                 = 322,
    TEXT_BANK_ICEBERG_RUINS                              = 323,
    TEXT_BANK_ROCK_PEAK_RUINS                            = 324,
    TEXT_BANK_UNK_0325                                   = 325,
    TEXT_BANK_UNK_0326                                   = 326,
    TEXT_BANK_UNK_0327                                   = 327,
    TEXT_BANK_UNK_0328                                   = 328,
    TEXT_BANK_UNK_0329                                   = 329,
    TEXT_BANK_UNK_0330                                   = 330,
    TEXT_BANK_UNK_0331                                   = 331,
    TEXT_BANK_UNK_0332                                   = 332,
    TEXT_BANK_UNK_0333                                   = 333,
    TEXT_BANK_UNK_0334                                   = 334,
    TEXT_BANK_UNK_0335                                   = 335,
    TEXT_BANK_UNK_0336                                   = 336,
    TEXT_BANK_UNK_0337                                   = 337,
    TEXT_BANK_UNK_0338                                   = 338,
    TEXT_BANK_UNK_0339                                   = 339,
    TEXT_BANK_UNK_0340                                   = 340,
    TEXT_BANK_UNK_0341                                   = 341,
    TEXT_BANK_UNK_0342                                   = 342,
    TEXT_BANK_UNK_0343                                   = 343,
    TEXT_BANK_UNK_0344                                   = 344,
    TEXT_BANK_UNK_0345                                   = 345,
    TEXT_BANK_UNK_0346                                   = 346,
    TEXT_BANK_UNK_0347                                   = 347,
    TEXT_BANK_UNK_0348                                   = 348,
    TEXT_BANK_DUMMY_0349                                 = 349,
    TEXT_BANK_TRADE                                      = 350,
    TEXT_BANK_HALL_OF_FAME                               = 351,
    TEXT_BANK_PC_HALL_OF_FAME                            = 352,
    TEXT_BANK_UNK_0353                                   = 353,
    TEXT_BANK_TRADE_ROOM                                 = 354,
    TEXT_BANK_DUMMY_0355                                 = 355,
    TEXT_BANK_WIFI_EARTH                                 = 356,
    TEXT_BANK_EGG_HATCH                                  = 357,
    TEXT_BANK_UNK_0358                                   = 358,
    TEXT_BANK_UNK_0359                                   = 359,
    TEXT_BANK_UNK_0360                                   = 360,
    TEXT_BANK_MENU_ENTRIES                               = 361,
    TEXT_BANK_DUMMY_0362                                 = 362,
    TEXT_BANK_BATTLE_FACTORY_OT_NAME                     = 363,
    TEXT_BANK_BATTLE_FACTORY_APP                         = 364,
    TEXT_BANK_BATTLE_FACTORY_SCENE                       = 365,
    TEXT_BANK_JOURNAL_ENTRIES                            = 366,
    TEXT_BANK_START_MENU                                 = 367,
    TEXT_BANK_BATTLE_STRINGS                             = 368,
    TEXT_BANK_VISIBLE_ITEMS                              = 369,
    TEXT_BANK_NPC_TRADE_NAMES                            = 370,
    TEXT_BANK_FURNITURE_NAMES                            = 371,
    TEXT_BANK_MYSTERY_GIFT_PHRASE                        = 372,
    TEXT_BANK_BLACK_OUT_SCENE                            = 373,
    TEXT_BANK_GROUP_CONNECTION                           = 374,
    TEXT_BANK_UNK_0375                                   = 375,
    TEXT_BANK_SPIN_TRADE                                 = 376,
    TEXT_BANK_UNK_0377                                   = 377,
    TEXT_BANK_GYM_NAMES                                  = 378,
    TEXT_BANK_MYSTERY_GIFT_DELIVERYMAN                   = 379,
    TEXT_BANK_HIDDEN_ITEMS                               = 380,
    TEXT_BANK_FIELD_MOVES                                = 381,
    TEXT_BANK_WIFI_PLAZA_ENTRANCE                        = 382,
    TEXT_BANK_POKEDEX_RATINGS                            = 383,
    TEXT_BANK_UNK_0384                                   = 384,
    TEXT_BANK_UNK_0385                                   = 385,
    TEXT_BANK_CONTEST_ACCESSORY_NAMES                    = 386,
    TEXT_BANK_CONTEST_ACCESSORY_NAMES_WITH_ARTICLES      = 387,
    TEXT_BANK_CONTEST_BACKDROP_NAMES                     = 388,
    TEXT_BANK_ROWAN_INTRO                                = 389,
    TEXT_BANK_DUMMY_0390                                 = 390,
    TEXT_BANK_ITEM_DESCRIPTIONS                          = 391,
    TEXT_BANK_ITEM_NAMES                                 = 392,
    TEXT_BANK_ITEM_NAMES_WITH_ARTICLES                   = 393,
    TEXT_BANK_ITEM_NAMES_PLURAL                          = 394,
    TEXT_BANK_BAG_POCKET_NAMES                           = 395,
    TEXT_BANK_BAG_POCKET_NAMES_WITH_ICONS                = 396,
    TEXT_BANK_BERRY_TREES                                = 397,
    TEXT_BANK_BERRY_TAGS                                 = 398,
    TEXT_BANK_VERITY_LAKEFRONT                           = 399,
    TEXT_BANK_UNUSED_VERITY_LAKEFRONT_HOUSE              = 400,
    TEXT_BANK_VALOR_LAKEFRONT                            = 401,
    TEXT_BANK_RESTAURANT                                 = 402,
    TEXT_BANK_GRAND_LAKE_VALOR_LAKEFRONT_EAST_HOUSE      = 403,
    TEXT_BANK_GRAND_LAKE_VALOR_LAKEFRONT_WEST_HOUSE      = 404,
    TEXT_BANK_ACUITY_LAKEFRONT                           = 405,
    TEXT_BANK_SAVE_CORRUPTED                             = 406,
    TEXT_BANK_UNK_0407                                   = 407,
    TEXT_BANK_MAILBOX                                    = 408,
    TEXT_BANK_MAIL                                       = 409,
    TEXT_BANK_UNK_0410                                   = 410,
    TEXT_BANK_UNK_0411                                   = 411,
    TEXT_BANK_SPECIES_NAME                               = 412,
    TEXT_BANK_SPECIES_NAME_WITH_ARTICLES                 = 413,
    TEXT_BANK_MONTH_NAMES                                = 414,
    TEXT_BANK_TV_PROGRAMS_INTERVIEWS                     = 415,
    TEXT_BANK_TV_PROGRAMS_TRAINER_SIGHTINGS              = 416,
    TEXT_BANK_TV_PROGRAMS_RECORDS                        = 417,
    TEXT_BANK_TV_PROGRAMS_SINNOH_NOW                     = 418,
    TEXT_BANK_TV_PROGRAMS_VARIETY_HOUR                   = 419,
    TEXT_BANK_UNK_0420                                   = 420,
    TEXT_BANK_MYSTERY_GIFT_MENU                          = 421,
    TEXT_BANK_NAMING_SCREEN                              = 422,
    TEXT_BANK_BERRY_DESCRIPTIONS                         = 423,
    TEXT_BANK_BERRY_NAMES                                = 424,
    TEXT_BANK_DRAWING                                    = 425,
    TEXT_BANK_DUMMY_0426                                 = 426,
    TEXT_BANK_GENERIC_NAMES                              = 427,
    TEXT_BANK_UNK_0428                                   = 428,
    TEXT_BANK_FOLLOWER_PARTNERS                          = 429,
    TEXT_BANK_POKEMON_CENTER_B1F_COMMON                  = 430,
    TEXT_BANK_RECORD_CHATOT_CRY                          = 431,
    TEXT_BANK_POFFIN_COMMON                              = 432,
    TEXT_BANK_LOCATION_NAMES                             = 433,
    TEXT_BANK_MYSTERY_GIFT_EVENT_NAMES                   = 434,
    TEXT_BANK_SPECIAL_MET_LOCATION_NAMES                 = 435,
    TEXT_BANK_EASY_CHAT_GROUPS                           = 436,
    TEXT_BANK_EASY_CHAT                                  = 437,
    TEXT_BANK_UNK_0438                                   = 438,
    TEXT_BANK_TRAINER_WORDS                              = 439,
    TEXT_BANK_PEOPLE_WORDS                               = 440,
    TEXT_BANK_GREETINGS                                  = 441,
    TEXT_BANK_LIFESTYLE_WORDS                            = 442,
    TEXT_BANK_FEELINGS                                   = 443,
    TEXT_BANK_TOUGH_WORDS                                = 444,
    TEXT_BANK_UNION_WORDS                                = 445,
    TEXT_BANK_LOSS_SENTENCES                             = 446,
    TEXT_BANK_GENERAL_SENTENCES                          = 447,
    TEXT_BANK_PRE_BATTLE_SENTENCES                       = 448,
    TEXT_BANK_UNION_ROOM_SENTENCES                       = 449,
    TEXT_BANK_WIN_SENTENCES                              = 450,
    TEXT_BANK_UNK_0451                                   = 451,
    TEXT_BANK_UNK_0452                                   = 452,
    TEXT_BANK_PARTY_MENU                                 = 453,
    TEXT_BANK_MIGRATE_FROM_GBA                           = 454,
    TEXT_BANK_POKEMON_SUMMARY_SCREEN                     = 455,
    TEXT_BANK_POKETCH_MOVE_TESTER                        = 456,
    TEXT_BANK_POKETCH_APP_NAMES                          = 457,
    TEXT_BANK_POKETCH_POKEMON_HISTORY                    = 458,
    TEXT_BANK_POKETCH_BERRY_SEARCHER                     = 459,
    TEXT_BANK_DUMMY_0460                                 = 460,
    TEXT_BANK_POKETCH_LINK_SEARCHER                      = 461,
    TEXT_BANK_POFFIN_CUTSCENE                            = 462,
    TEXT_BANK_POFFIN_CASE                                = 463,
    TEXT_BANK_POFFIN_MAKING                              = 464,
    TEXT_BANK_POFFIN_TYPES                               = 465,
    TEXT_BANK_ROUTE_201                                  = 466,
    TEXT_BANK_ROUTE_202                                  = 467,
    TEXT_BANK_ROUTE_203                                  = 468,
    TEXT_BANK_ROUTE_204_SOUTH                            = 469,
    TEXT_BANK_ROUTE_204_NORTH                            = 470,
    TEXT_BANK_ROUTE_205_SOUTH                            = 471,
    TEXT_BANK_ROUTE_205_HOUSE                            = 472,
    TEXT_BANK_ROUTE_205_NORTH                            = 473,
    TEXT_BANK_ROUTE_206                                  = 474,
    TEXT_BANK_ROUTE_206_CYCLING_ROAD_SOUTH_GATE          = 475,
    TEXT_BANK_UNUSED_GATE_BETWEEN_ETERNA_CITY_ROUTE_206  = 476,
    TEXT_BANK_ROUTE_207                                  = 477,
    TEXT_BANK_ROUTE_208                                  = 478,
    TEXT_BANK_ROUTE_208_HOUSE                            = 479,
    TEXT_BANK_ROUTE_209                                  = 480,
    TEXT_BANK_ROUTE_209_LOST_TOWER_1F                    = 481,
    TEXT_BANK_ROUTE_209_LOST_TOWER_5F                    = 482,
    TEXT_BANK_ROUTE_210_SOUTH                            = 483,
    TEXT_BANK_CAFE                                       = 484,
    TEXT_BANK_ROUTE_210_NORTH                            = 485,
    TEXT_BANK_ROUTE_210_GRANDMA_WILMA_HOUSE              = 486,
    TEXT_BANK_ROUTE_211_WEST                             = 487,
    TEXT_BANK_ROUTE_211_EAST                             = 488,
    TEXT_BANK_ROUTE_212_NORTH                            = 489,
    TEXT_BANK_POKEMON_MANSION                            = 490,
    TEXT_BANK_POKEMON_MANSION_MAIDS_ROOM                 = 491,
    TEXT_BANK_POKEMON_MANSION_OFFICE                     = 492,
    TEXT_BANK_ROUTE_212_SOUTH                            = 493,
    TEXT_BANK_ROUTE_212_HOUSE                            = 494,
    TEXT_BANK_ROUTE_213                                  = 495,
    TEXT_BANK_ROUTE_213_GATE_TO_PASTORIA_CITY            = 496,
    TEXT_BANK_FOOTSTEP_HOUSE                             = 497,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_LOBBY                 = 498,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_EAST_HOUSE            = 499,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_NORTHWEST_HOUSE       = 500,
    TEXT_BANK_GRAND_LAKE_ROUTE_213_NORTHEAST_HOUSE       = 501,
    TEXT_BANK_ROUTE_214                                  = 502,
    TEXT_BANK_ROUTE_214_GATE_TO_VEILSTONE_CITY           = 503,
    TEXT_BANK_ROUTE_215                                  = 504,
    TEXT_BANK_ROUTE_216                                  = 505,
    TEXT_BANK_ROUTE_216_HOUSE                            = 506,
    TEXT_BANK_ROUTE_217                                  = 507,
    TEXT_BANK_ROUTE_217_WEST_HOUSE                       = 508,
    TEXT_BANK_ROUTE_217_NORTHEAST_HOUSE                  = 509,
    TEXT_BANK_ROUTE_218                                  = 510,
    TEXT_BANK_ROUTE_218_GATE_TO_JUBILIFE_CITY            = 511,
    TEXT_BANK_ROUTE_218_GATE_TO_CANALAVE_CITY            = 512,
    TEXT_BANK_ROUTE_219                                  = 513,
    TEXT_BANK_ROUTE_221                                  = 514,
    TEXT_BANK_PAL_PARK_LOBBY                             = 515,
    TEXT_BANK_ROUTE_221_HOUSE                            = 516,
    TEXT_BANK_ROUTE_222                                  = 517,
    TEXT_BANK_ROUTE_222_WEST_HOUSE                       = 518,
    TEXT_BANK_ROUTE_222_EAST_HOUSE                       = 519,
    TEXT_BANK_ROUTE_222_GATE_TO_SUNYSHORE_CITY           = 520,
    TEXT_BANK_ROUTE_224                                  = 521,
    TEXT_BANK_ROUTE_225                                  = 522,
    TEXT_BANK_ROUTE_225_HOUSE                            = 523,
    TEXT_BANK_UNK_0524                                   = 524,
    TEXT_BANK_ROUTE_227                                  = 525,
    TEXT_BANK_ROUTE_227_HOUSE                            = 526,
    TEXT_BANK_ROUTE_228                                  = 527,
    TEXT_BANK_ROUTE_228_GATE_TO_ROUTE_226                = 528,
    TEXT_BANK_ROUTE_228_NORTH_HOUSE                      = 529,
    TEXT_BANK_ROUTE_228_SOUTH_HOUSE                      = 530,
    TEXT_BANK_ROUTE_229                                  = 531,
    TEXT_BANK_RANKINGS_MACHINE                           = 532,
    TEXT_BANK_UNK_0533                                   = 533,
    TEXT_BANK_SAVE_INFO_WINDOW                           = 534,
    TEXT_BANK_RIBBON_NAMES                               = 535,
    TEXT_BANK_DUMMY_0536                                 = 536,
    TEXT_BANK_BATTLE_ARCADE_SCENE                        = 537,
    TEXT_BANK_SAFARI_GAME                                = 538,
    TEXT_BANK_VS_SEEKER                                  = 539,
    TEXT_BANK_SCRATCH_OFF_CARDS_APP                      = 540,
    TEXT_BANK_SCRATCH_OFF_CARDS                          = 541,
    TEXT_BANK_UNK_0542                                   = 542,
    TEXT_BANK_UNK_0543                                   = 543,
    TEXT_BANK_UNK_0544                                   = 544,
    TEXT_BANK_SEQ_NAMES                                  = 545,
    TEXT_BANK_UNK_0546                                   = 546,
    TEXT_BANK_DAY_CARE_COMMON                            = 547,
    TEXT_BANK_END_CREDITS                                = 548,
    TEXT_BANK_BATTLE_HALL_SCENE                          = 549,
    TEXT_BANK_MAIN_MENU_OPTIONS                          = 550,
    TEXT_BANK_POKEMON_STAT_NAMES                         = 551,
    TEXT_BANK_COUNTERPART_TALK                           = 552,
    TEXT_BANK_COUNTERPART_NAMES                          = 553,
    TEXT_BANK_TWINLEAF_TOWN                              = 554,
    TEXT_BANK_TWINLEAF_TOWN_RIVAL_HOUSE_1F               = 555,
    TEXT_BANK_TWINLEAF_TOWN_RIVAL_HOUSE_2F               = 556,
    TEXT_BANK_TWINLEAF_TOWN_PLAYER_HOUSE_1F              = 557,
    TEXT_BANK_TWINLEAF_TOWN_PLAYER_HOUSE_2F              = 558,
    TEXT_BANK_TWINLEAF_TOWN_NORTHEAST_HOUSE              = 559,
    TEXT_BANK_TWINLEAF_TOWN_SOUTHWEST_HOUSE              = 560,
    TEXT_BANK_SANDGEM_TOWN                               = 561,
    TEXT_BANK_SANDGEM_TOWN_MART                          = 562,
    TEXT_BANK_SANDGEM_TOWN_POKECENTER_1F                 = 563,
    TEXT_BANK_SANDGEM_TOWN_POKECENTER_2F                 = 564,
    TEXT_BANK_SANDGEM_TOWN_POKEMON_RESEARCH_LAB          = 565,
    TEXT_BANK_SANDGEM_TOWN_COUNTERPART_HOUSE_1F          = 566,
    TEXT_BANK_SANDGEM_TOWN_COUNTERPART_HOUSE_2F          = 567,
    TEXT_BANK_SANDGEM_TOWN_HOUSE                         = 568,
    TEXT_BANK_FLOAROMA_TOWN                              = 569,
    TEXT_BANK_FLOAROMA_TOWN_MART                         = 570,
    TEXT_BANK_FLOAROMA_TOWN_POKECENTER_1F                = 571,
    TEXT_BANK_FLOWER_SHOP                                = 572,
    TEXT_BANK_FLOAROMA_TOWN_SOUTHEAST_HOUSE              = 573,
    TEXT_BANK_FLOAROMA_TOWN_MIDDLE_HOUSE                 = 574,
    TEXT_BANK_SOLACEON_TOWN                              = 575,
    TEXT_BANK_SOLACEON_TOWN_MART                         = 576,
    TEXT_BANK_SOLACEON_TOWN_POKECENTER_1F                = 577,
    TEXT_BANK_POKEMON_DAY_CARE                           = 578,
    TEXT_BANK_SOLACEON_TOWN_NORTHEAST_HOUSE              = 579,
    TEXT_BANK_SOLACEON_TOWN_POKEMON_NEWS_PRESS           = 580,
    TEXT_BANK_SOLACEON_TOWN_NORTH_HOUSE                  = 581,
    TEXT_BANK_SOLACEON_TOWN_EAST_HOUSE                   = 582,
    TEXT_BANK_CELESTIC_TOWN                              = 583,
    TEXT_BANK_CELESTIC_TOWN_POKECENTER_1F                = 584,
    TEXT_BANK_CELESTIC_TOWN_NORTH_HOUSE                  = 585,
    TEXT_BANK_CELESTIC_TOWN_NORTHWEST_HOUSE              = 586,
    TEXT_BANK_CELESTIC_TOWN_NORTHEAST_HOUSE              = 587,
    TEXT_BANK_CELESTIC_TOWN_SOUTHWEST_HOUSE              = 588,
    TEXT_BANK_CELESTIC_TOWN_CAVE                         = 589,
    TEXT_BANK_SURVIVAL_AREA                              = 590,
    TEXT_BANK_SURVIVAL_AREA_MART                         = 591,
    TEXT_BANK_SURVIVAL_AREA_POKECENTER_1F                = 592,
    TEXT_BANK_BATTLEGROUND                               = 593,
    TEXT_BANK_SURVIVAL_AREA_SOUTH_HOUSE                  = 594,
    TEXT_BANK_SURVIVAL_AREA_NORTH_HOUSE                  = 595,
    TEXT_BANK_RESORT_AREA                                = 596,
    TEXT_BANK_UNUSED_RESORT_AREA_MART                    = 597,
    TEXT_BANK_RESORT_AREA_POKECENTER_1F                  = 598,
    TEXT_BANK_RESORT_AREA_RIBBON_SYNDICATE_1F            = 599,
    TEXT_BANK_RESORT_AREA_RIBBON_SYNDICATE_2F            = 600,
    TEXT_BANK_RESORT_AREA_RIBBON_SYNDICATE_ELEVATOR      = 601,
    TEXT_BANK_VILLA                                      = 602,
    TEXT_BANK_VILLA_FURNITURE_NAMES                      = 603,
    TEXT_BANK_RESORT_AREA_HOUSE                          = 604,
    TEXT_BANK_BATTLE_VIDEO                               = 605,
    TEXT_BANK_FLAVOR_NAMES                               = 606,
    TEXT_BANK_ROWAN_INTRO_TV_APP                         = 607,
    TEXT_BANK_TIMES_OF_DAY                               = 608,
    TEXT_BANK_TITLE_SCREEN                               = 609,
    TEXT_BANK_ABILITY_NAMES                              = 610,
    TEXT_BANK_ABILITY_NAMES_UPPERCASE                    = 611,
    TEXT_BANK_ABILITY_DESCRIPTIONS                       = 612,
    TEXT_BANK_UNK_0613                                   = 613,
    TEXT_BANK_FRONTIER_TRAINER_MESSAGES                  = 614,
    TEXT_BANK_TOWN_MAP                                   = 615,
    TEXT_BANK_TRAINER_CARD                               = 616,
    TEXT_BANK_NPC_TRAINER_MESSAGES                       = 617,
    TEXT_BANK_NPC_TRAINER_NAMES                          = 618,
    TEXT_BANK_TRAINER_CLASS_NAMES                        = 619,
    TEXT_BANK_TRAINER_CLASS_NAMES_WITH_ARTICLES          = 620,
    TEXT_BANK_TV_REPORTER_INTERVIEWS                     = 621,
    TEXT_BANK_TV_PROGRAMS                                = 622,
    TEXT_BANK_TV_COMMERCIALS                             = 623,
    TEXT_BANK_POKEMON_TYPE_NAMES                         = 624,
    TEXT_BANK_UNDERGROUND                                = 625,
    TEXT_BANK_UNDERGROUND_GOODS                          = 626,
    TEXT_BANK_UNDERGROUND_GOODS_WITH_ARTICLES            = 627,
    TEXT_BANK_UNDERGROUND_ITEMS                          = 628,
    TEXT_BANK_UNDERGROUND_ITEM_NAMES_WITH_ARTICLES       = 629,
    TEXT_BANK_UNDERGROUND_TRAPS                          = 630,
    TEXT_BANK_UNDERGROUND_TRAP_NAMES_WITH_ARTICLES       = 631,
    TEXT_BANK_UNDERGROUND_ANSWERS                        = 632,
    TEXT_BANK_UNDERGROUND_QUESTIONS                      = 633,
    TEXT_BANK_UNDERGROUND_COMMON                         = 634,
    TEXT_BANK_UNION_ROOM                                 = 635,
    TEXT_BANK_UNDERGROUND_NPCS                           = 636,
    TEXT_BANK_UNDERGROUND_BASE_DECORATION                = 637,
    TEXT_BANK_UNDERGROUND_CAPTURE_FLAG                   = 638,
    TEXT_BANK_UNDERGROUND_PC                             = 639,
    TEXT_BANK_UNDERGROUND_RECORDS                        = 640,
    TEXT_BANK_DUMMY_0641                                 = 641,
    TEXT_BANK_ROUTE_226                                  = 642,
    TEXT_BANK_ROUTE_226_HOUSE                            = 643,
    TEXT_BANK_ROUTE_230                                  = 644,
    TEXT_BANK_MOVE_REMINDER                              = 645,
    TEXT_BANK_MOVE_DESCRIPTIONS                          = 646,
    TEXT_BANK_MOVE_NAMES                                 = 647,
    TEXT_BANK_MOVE_NAMES_UPPERCASE                       = 648,
    TEXT_BANK_PLAZA_EVENT_NAMES                          = 649,
    TEXT_BANK_UNK_0650                                   = 650,
    TEXT_BANK_PLAZA_MINIGAME_NAMES                       = 651,
    TEXT_BANK_PLAZA_ITEM_NAMES                           = 652,
    TEXT_BANK_UNK_0653                                   = 653,
    TEXT_BANK_UNK_0654                                   = 654,
    TEXT_BANK_UNK_0655                                   = 655,
    TEXT_BANK_UNK_0656                                   = 656,
    TEXT_BANK_UNK_0657                                   = 657,
    TEXT_BANK_UNK_0658                                   = 658,
    TEXT_BANK_UNK_0659                                   = 659,
    TEXT_BANK_UNK_0660                                   = 660,
    TEXT_BANK_UNK_0661                                   = 661,
    TEXT_BANK_UNK_0662                                   = 662,
    TEXT_BANK_GREETINGS_EN                               = 663,
    TEXT_BANK_GREETINGS_FR                               = 664,
    TEXT_BANK_GREETINGS_DE                               = 665,
    TEXT_BANK_GREETINGS_IT                               = 666,
    TEXT_BANK_GREETINGS_JP                               = 667,
    TEXT_BANK_GREETINGS_ES                               = 668,
    TEXT_BANK_DUMMY_0669                                 = 669,
    TEXT_BANK_UNK_0670                                   = 670,
    TEXT_BANK_GTS                                        = 671,
    TEXT_BANK_UNK_0672                                   = 672,
    TEXT_BANK_UNK_0673                                   = 673,
    TEXT_BANK_UNK_0674                                   = 674,
    TEXT_BANK_PAL_PAD                                    = 675,
    TEXT_BANK_COUNTRY_REGION_NAMES_ARGENTINA             = 676,
    TEXT_BANK_COUNTRY_REGION_NAMES_AUSTRALIA             = 677,
    TEXT_BANK_COUNTRY_REGION_NAMES_BRAZIL                = 678,
    TEXT_BANK_COUNTRY_REGION_NAMES_CANADA                = 679,
    TEXT_BANK_COUNTRY_REGION_NAMES_CHINA                 = 680,
    TEXT_BANK_COUNTRY_REGION_NAMES_GERMANY               = 681,
    TEXT_BANK_COUNTRY_REGION_NAMES_SPAIN                 = 682,
    TEXT_BANK_COUNTRY_REGION_NAMES_FINLAND               = 683,
    TEXT_BANK_COUNTRY_REGION_NAMES_FRANCE                = 684,
    TEXT_BANK_COUNTRY_REGION_NAMES_UNITED_KINGDOM        = 685,
    TEXT_BANK_COUNTRY_REGION_NAMES_INDIA                 = 686,
    TEXT_BANK_COUNTRY_REGION_NAMES_ITALY                 = 687,
    TEXT_BANK_COUNTRY_REGION_NAMES_JAPAN                 = 688,
    TEXT_BANK_COUNTRY_REGION_NAMES_NORWAY                = 689,
    TEXT_BANK_COUNTRY_REGION_NAMES_POLAND                = 690,
    TEXT_BANK_COUNTRY_REGION_NAMES_RUSSIA                = 691,
    TEXT_BANK_COUNTRY_REGION_NAMES_SWEDEN                = 692,
    TEXT_BANK_COUNTRY_REGION_NAMES_UNITED_STATES         = 693,
    TEXT_BANK_COUNTRY_NAMES                              = 694,
    TEXT_BANK_UNK_0695                                   = 695,
    TEXT_BANK_UNK_0696                                   = 696,
    TEXT_BANK_POKEDEX                                    = 697,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_DIAMOND              = 698,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_PEARL                = 699,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_UNUSED               = 700,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_FR                   = 701,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_DE                   = 702,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_IT                   = 703,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_ES                   = 704,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_JP                   = 705,
    TEXT_BANK_SPECIES_POKEDEX_ENTRY_EN                   = 706,
    TEXT_BANK_SPECIES_WEIGHT                             = 707,
    TEXT_BANK_SPECIES_WEIGHT_GIRA                        = 708,
    TEXT_BANK_SPECIES_HEIGHT                             = 709,
    TEXT_BANK_SPECIES_HEIGHT_GIRA                        = 710,
    TEXT_BANK_SPECIES_CATEGORY                           = 711,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_EN         = 712,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_FR         = 713,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_DE         = 714,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_IT         = 715,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_ES         = 716,
    TEXT_BANK_SPECIES_NAME_WITH_NATDEX_NUMBER_JP         = 717,
    TEXT_BANK_SPECIES_CATEGORY_EN                        = 718,
    TEXT_BANK_SPECIES_CATEGORY_FR                        = 719,
    TEXT_BANK_SPECIES_CATEGORY_DE                        = 720,
    TEXT_BANK_SPECIES_CATEGORY_IT                        = 721,
    TEXT_BANK_SPECIES_CATEGORY_ES                        = 722,
    TEXT_BANK_SPECIES_CATEGORY_JP                        = 723,
};
typedef struct MessageLoader MessageLoader;
enum MessageLoaderMode {
    MSG_LOADER_PRELOAD_ENTIRE_BANK,
    MSG_LOADER_LOAD_ON_DEMAND,
};
MessageLoader *NewMsgDataFromNarc(enum MessageLoaderMode mode, enum NarcID narcID, u32 bankID, u32 heapID);
void DestroyMsgData(MessageLoader *loader);
void ReadMsgDataIntoString(const MessageLoader *loader, u32 entryID, String *string);
typedef struct SysTask SysTask;
typedef struct SysTaskManager SysTaskManager;
typedef void (*SysTaskFunc)(SysTask *, void *);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
void LoadUserFrameGfx1(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset);
void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);
typedef void (*Callback)(void *);
typedef void (*UnkCallback)(void);
enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};
typedef struct System {
    Callback vblankCallback;
    void *vblankCallbackData;
    Callback hblankCallback;
    void *hblankCallbackData;
    UnkCallback dummyCallback_10;
    UnkCallback dummyCallback_14;
    SysTaskManager *mainTaskMgr;
    SysTaskManager *vBlankTaskMgr;
    SysTaskManager *postVBlankTaskMgr;
    SysTaskManager *printTaskMgr;
    u32 *unused_28;
    u32 vblankCounter;
    u32 frameCounter;
    enum ButtonMode buttonMode;
    u32 heldKeysRaw;
    u32 pressedKeysRaw;
    u32 pressedKeysRepeatableRaw;
    u32 heldKeys;
    u32 pressedKeys;
    u32 pressedKeysRepeatable;
    int autorepeatTimer;
    int autorepeatRate;
    int autorepeatDelay;
    int simulatedInputs; // HGSS layout (was the Platinum System)
    u16 touchX;
    u16 touchY;
    u16 touchPressed;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;
extern System gSystem;
void Main_SetVBlankIntrCB(Callback cb, void *data);
BOOL Main_SetHBlankIntrCB(Callback cb, void *data);
void SetKeyRepeatTimers(int rate, int delay);
void SetMasterBrightnessNeutral(enum DSScreen screen);
void sub_0200FBF4();
void ResetAllTextPrinters(void);
u8 AddTextPrinterParameterized(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextPrinterCallback callback);
extern const GXBanks sErrorMessageBanksConfig;
extern const GraphicsModes sErrorMessageBgModeSet;
extern const BgTemplate sErrorMessageBgTemplate;
extern const WindowTemplate sErrorMessageWindowTemplate;
extern const HeapParam sErrorMessageHeapParams[1];
extern BOOL sErrorMessagePrinterLock;
void VBlankCB_DmaTasksFramecounter(void);





typedef struct Ov96Sub7C { /* invented */
    void *unk0;
    void *unk4;
    void *unk8[3];
    fx32 unk14;
    fx32 unk18;
    u8 unk1C[4];
    void *unk20;
    fx32 unk24;
    fx32 unk28;
    u8 unk2C[4];
    fx32 unk30;
    fx32 unk34;
    u8 unk38[0x24];
    u16 unk5C;
    u8 unk5E[0x1A];
    int unk78;
} Ov96Sub7C;

typedef struct Ov96Sub4C { /* invented */
    void *unk0;
    void *unk4;
    VecFx32 unk8;
    u8 unk14[0x24];
    u8 unk38;
    u8 unk39;
    u8 unk3A[8];
    u8 unk42;
    u8 unk43[9];
} Ov96Sub4C;

typedef struct Ov96Data { /* invented */
    void *unk0;
    BgConfig *bgConfig;
    u8 unk8[0x50];
    int heapID;
    Ov96Sub7C sub[4][3];
    Ov96Sub4C elem[2];
    void *unk6C4[4];
    void *unk6D4[4];
    void *unk6E4[4];
    u8 unk6F4[0x40];
    u8 unk734[4];
    int unk738;
    u8 unk73C[8];
    void *unk744;
    void *unk748;
    void *unk74C;
    void *unk750;
    VecFx32 unk754[8];
    u8 unk7B4[0x68];
    void *unk81C;
} Ov96Data;

typedef struct Ov96BgArgs { /* invented */
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u8 hk9pad133_3[0x8]; /* invented: asm offset */ u32 unkC;
} Ov96BgArgs;

typedef struct Ov96Flags { /* invented */
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14[11];
} Ov96Flags;

typedef struct Ov96Pos { /* invented */
    u16 x;
    u16 y;
} Ov96Pos;

void *PokeathlonCourse_GetHeapAllocPtr4(void *);
int PokeathlonCourse_GetField1ED(void *);
void *PokeathlonCourse_AllocPtr4FromHeap(void *, u32);
void PokeathlonCourse_IncrementField1ED(void *);
int PokeathlonCourse_GetParticipantCount(void *);
int PokeathlonCourse_GetMode(void *);
void PokeathlonCourse_SetVBlankIntrCB(BgConfig *);
void PokeathlonCourse_SetField1F4(void *, int);
void *PokeathlonCourse_GetDataCopyArea(void *);
void ov96_022117CC(void);
void Heap_Create(int param0, int param1, int param2);
void *Heap_Alloc(int, u32);
void FontID_Alloc(int, int);
void NNS_G2dInitOamManagerModule(void);
void OamManager_Create(int, int, int, int, int, int, int, int, int);
void GfGfx_SwapDisplay(void);
void GfGfx_EngineATogglePlanes(u32, int);
void GfGfx_EngineBTogglePlanes(u32, int);
void Sprite_SetDrawPriority(void *, int);
void ReadWholeNarcMemberByIdPair(void *, int, int);
BOOL IsPaletteFadeFinished(void);
void BeginNormalPaletteFade(int, int, int, int, int, int, int);
void sub_0203A994(int);
void ov96_021E6670(void *, int);
void ov96_021E92B0(Ov96BgArgs *, int, int, u32, int);
void ov96_022118C4(BgConfig *);
void ov96_0221362C(Ov96Data *);
void ov96_022140F4(void *);
void ov96_02214044(Ov96Data *, u8, int);
void ov96_022141F8(Ov96Data *);
void *ov96_0221464C(int, BgConfig *, void *);
void *ov96_02214A24(int, int, int);
void *ov96_021E9A78(int, int, int);
void ov96_021E64B8(void *);
void *ov96_021EB180(int, VecFx32 *);
void ov96_021EB5C8(void *, int, int, int, int);
int ov96_021EB5E8(void *);
void *ov96_021EA854(int, int, int, void *, int);
void ov96_021EB29C(void *, int, int);
void ov96_021EB2BC(void *, int, int, int, int);
void ov96_021EB2F4(void *, int, int, int, int, int);
void ov96_021EB334(void *, int, int, int);
void ov96_021EB36C(void *, int, int, int);
void ov96_021EB3A4(void *);
void *ov96_021EB3E4(void *, int, int, int, int);
void ov96_021EB630(void *, int);
void ov96_021EB564(void *, int);
void ov96_02213444(Ov96Sub4C *);
void ov96_021EB588(void *, const VecFx32 *);
void ov96_021EB52C(void *, int, int);
void ov96_021EB5AC(void *, u16, int);
void **ov96_021E6290(void *, int, void *, void *);
void ov96_02214718(void *, void *, void *);
void ov96_021E6168(void *, int, int, void *);
void *ov96_021E60C0(void *, int, int);
int ov96_021E6108(void *);
int ov96_021E6138(void *);
int ov96_021E6104(void);
void ov96_021EA8A8(void *, int, void *, Ov96Flags *, int, int);
BOOL ov96_021EAA00(void *);
int ov96_021E5F24(void *);
void *ov96_021EAA04(void *, u8);
void ov96_021EAB38(void *, int);
void ov96_021EAF70(void *, u32, u32);
void *ov96_021EAA20(void *);
void *ov96_021EAF8C(void *);
void ov96_021E90FC(void *);
void ov96_021EAC0C(void *, int);
void ov96_021EAF94(void *, u32, u32);
void ov96_021EAF6C(void *, int);
void ov96_021EB0A4(void *, u32, u32, int *, int *);
void ov96_021EABA8(void *, int);
void ov96_021EABDC(void *, int);
void ov96_021E64F8(void *, void *, void *, int, int);
void ov96_021E634C(void *, int, void *, void *, int, int, Ov96Pos *);
void ov96_0221359C(void *, void *, u8, u8, Ov96Sub7C *);
void ov96_02214A6C(void *, u8, Ov96Sub7C *);
void ov96_02214B74(void *, Ov96Sub4C *);
void ov96_02214A9C(void *);
void *ov96_021E8A20(void *);
void ov96_02211DE4(Ov96Data *, void *);
void ov96_02211A24(Ov96Data *);
void ov96_022147FC(void *);
void ov96_0221490C(void *, int);

extern const VecFx32 ov96_0221D238;
extern const VecFx32 ov96_0221D4B4[4][2];
extern const VecFx32 ov96_0221D3A8[4];
extern const Ov96Pos ov96_0221D408[4][3];
extern const VecFx32 ov96_0221D220;
extern const VecFx32 ov96_0221D250;
extern const VecFx32 ov96_0221D208;

static VecFx32 hgasset_ov96_02210C98_pos[4] __attribute__((section(BWPORT_DATA_SECTION))) = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

BOOL ov96_02210C98(void *param0)
{
    Ov96Data *heapPtr;
    Ov96Data *data;
    int i;
    int j;
    u32 q;
    int r;
    int myIdx;
    void *obj;
    void *obj2;
    Ov96Sub4C *elem;
    int kind;
    int sp6C;
    int sp68;
    u8 k;
    VecFx32 v78;
    VecFx32 v84;
    VecFx32 v90;
    u32 narc[3][2];
    Ov96Pos posB4[3];
    VecFx32 vec;
    Ov96BgArgs bgArgs;
    u8 narc2[0x78];
    Ov96Flags flags;
    u32 objArgs[12][4];

    heapPtr = PokeathlonCourse_GetHeapAllocPtr4(param0);

    switch (PokeathlonCourse_GetField1ED(param0)) {
    case 0:
        Heap_Create(92, 147, 5 << 16);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        ov96_022117CC();
        data = PokeathlonCourse_AllocPtr4FromHeap(param0, 0x820);
        MI_CpuFill8(data, 0, 0x820);
        data->unk81C = Heap_Alloc(147, 0x28);
        MI_CpuFill8(data->unk81C, 0, 0x28);
        data->bgConfig = BgConfig_Alloc((enum HeapID)147);
        ov96_021E6670(param0, 8);
        bgArgs.unk0 = 0x85;
        bgArgs.unk4 = 0x40000;
        bgArgs.unk8 = 0x4000;
        bgArgs.unkC = 147;
        ov96_021E92B0(&bgArgs, 0x10, 147, 0x300010, 0x10);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 126, 0, 32, 1, 126, 0, 32, 147);
        data->heapID = 147;
        FontID_Alloc(4, 147);
        ov96_022118C4(data->bgConfig);
        ov96_0221362C(data);
        ov96_022140F4(param0);
        for (k = 0; k < 4; k++) {
            ov96_02214044(data, k, 0);
        }
        ov96_022141F8(data);
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(param0);
        break;
    case 1:
        heapPtr->unk750 = ov96_0221464C(heapPtr->heapID, heapPtr->bgConfig, param0);
        heapPtr->unk74C = ov96_02214A24(heapPtr->heapID, 4 - PokeathlonCourse_GetParticipantCount(param0), PokeathlonCourse_GetMode(param0));
        heapPtr->unk744 = ov96_021E9A78(heapPtr->heapID, 0xAAF, 1);
        ov96_021E64B8(param0);
        PokeathlonCourse_IncrementField1ED(param0);
        break;
    case 2:
        vec = ov96_0221D238;
        heapPtr->unk0 = ov96_021EB180(heapPtr->heapID, &vec);
        ov96_021EB5C8(heapPtr->unk0, 0, 0, 0, 0x200000);
        heapPtr->unk748 = ov96_021EA854(heapPtr->heapID, 12, 8, heapPtr->unk744, ov96_021EB5E8(heapPtr->unk0));
        ov96_021EB29C(heapPtr->unk0, 0, 0x65);
        ov96_021EB29C(heapPtr->unk0, 1, 0x66);
        ov96_021EB2BC(heapPtr->unk0, 0xEC, 0x12, 0x65, 1);
        ov96_021EB2F4(heapPtr->unk0, 0xEC, 0xF, 0x65, 1, 2);
        ov96_021EB334(heapPtr->unk0, 0xEC, 0x11, 0x65);
        ov96_021EB36C(heapPtr->unk0, 0xEC, 0x10, 0x65);
        ov96_021EB2BC(heapPtr->unk0, 0xEC, 0xE, 0x66, 2);
        ov96_021EB2F4(heapPtr->unk0, 0xEC, 0xB, 0x66, 2, 1);
        ov96_021EB334(heapPtr->unk0, 0xEC, 0xD, 0x66);
        ov96_021EB36C(heapPtr->unk0, 0xEC, 0xC, 0x66);
        ov96_021EB3A4(heapPtr->unk0);
        for (i = 0; i < 12; i++) {
            heapPtr->sub[i / 3][i % 3].unk4 = ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 2);
            ov96_021EB630(heapPtr->sub[i / 3][i % 3].unk4, 5);
            for (j = 0; j < 3; j++) {
                heapPtr->sub[i / 3][i % 3].unk8[j] = ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 7);
                ov96_021EB630(heapPtr->sub[i / 3][i % 3].unk8[j], 0x2F);
            }
        }
        for (i = 0, elem = heapPtr->elem; i < 2; i++, elem++) {
            VecFx32 pos2 = { 0, 0, 0 };
            elem->unk0 = ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 3);
            elem->unk4 = ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 8);
            ov96_021EB564(elem->unk0, 10);
            ov96_021EB564(elem->unk4, 13);
            ov96_021EB630(elem->unk0, 0x2E);
            ov96_021EB630(elem->unk4, 0x2D);
            ov96_02213444(elem);
            elem->unk39 = 0;
            pos2.x = 0x80000;
            pos2.y = 0x68000;
            ov96_021EB588(elem->unk0, &pos2);
            ov96_021EB588(elem->unk4, &pos2);
            elem->unk8 = pos2;
            elem->unk42 = 7;
            if (i == 0) {
                ov96_021EB52C(elem->unk0, 1, 1);
                elem->unk38 = 1;
            }
        }
        {
        u16 sp70[4] = { 0, 0x4000, 0xC000, 0x8000 }; // ov96_0221D1F4+0xC
        __typeof__(hgasset_ov96_02210C98_pos) pos; __builtin_memcpy((void *)&pos, &hgasset_ov96_02210C98_pos, sizeof(pos)); // ov96_0221D3D8
        for (i = 0; i < 4; i++) {
            heapPtr->unk6C4[i] =ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 5);
            ov96_021EB52C(heapPtr->unk6C4[i], 1, 1);
            ov96_021EB564(heapPtr->unk6C4[i], 1);
            ov96_021EB588(heapPtr->unk6C4[i], &pos[i]);
            ov96_021EB5AC(heapPtr->unk6C4[i], sp70[i], 1);
            ov96_021EB630(heapPtr->unk6C4[i], 3);
            for (j = 0; j < 2; j++) {
                obj = ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 6);
                ov96_021EB52C(obj, 1, 1);
                ov96_021EB564(obj, i * 2 + 2 + j);
                ov96_021EB588(obj, &ov96_0221D4B4[i][j]);
                ov96_021EB630(obj, 0x67);
            }
            heapPtr->unk6D4[i] = ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 11);
            ov96_021EB588(heapPtr->unk6D4[i], &ov96_0221D3A8[i]);
            ov96_021EB630(heapPtr->unk6D4[i], 2);
            heapPtr->unk6E4[i] = ov96_021EB3E4(heapPtr->unk0, 1, 1, 0x65, 12);
            ov96_021EB588(heapPtr->unk6E4[i], &pos[i]);
            ov96_021EB630(heapPtr->unk6E4[i], 0x66);
        }
        }
        Sprite_SetDrawPriority(*ov96_021E6290(param0, 0, heapPtr->unk744, heapPtr->unk0), 1);
        ov96_02214718(heapPtr->unk0, heapPtr->unk750, heapPtr->unk744);
        PokeathlonCourse_IncrementField1ED(param0);
        break;
    case 3:
        for (i = 0; i < 12; i++) {
            r = i % 3;
            q = i / 3;
            ov96_021E6168(param0, q, r, objArgs[i]);
            flags.unk14[i] = ov96_021E6108(ov96_021E60C0(param0, q, r));
        }
        flags.unk0 = 0;
        flags.unk4 = 1;
        flags.unk8 = 0;
        flags.unkC = 1;
        flags.unk10 = 1;
        ov96_021EA8A8(heapPtr->unk748, 12, objArgs, &flags, 0, 0);
        PokeathlonCourse_IncrementField1ED(param0);
        break;
    case 4:
        if (!ov96_021EAA00(heapPtr->unk748)) {
            break;
        }
        myIdx = ov96_021E5F24(param0);
        PokeathlonCourse_SetVBlankIntrCB(heapPtr->bgConfig);
        PokeathlonCourse_SetField1F4(param0, 1);
        ReadWholeNarcMemberByIdPair(narc, 0xAA, 11);
        for (i = 0; i < 12; i++) {
            int y;
            int x;

            obj = ov96_021EAA04(heapPtr->unk748, i);
            ov96_021EAB38(obj, 1);
            kind = ov96_021E6138(ov96_021E60C0(param0, i / 3, i % 3));
            ov96_021EAF70(obj, narc[kind - 1][0], narc[kind - 1][1]);
            obj = ov96_021EAA04(heapPtr->unk748, i);
            obj2 = ov96_021EAA20(obj);
            q = i / 3;
            heapPtr->sub[q][r].unk0 = obj;
            r = i % 3;
            heapPtr->sub[q][r].unk20 = ov96_021EAF8C(obj);
            heapPtr->sub[q][r].unk78 = 0;
            ov96_021E90FC(obj2);
            sp6C = ov96_0221D408[q][r].x;
            sp68 = ov96_0221D408[q][r].y;
            heapPtr->sub[q][r].unk5C = 2;
            ov96_021EAC0C(obj, 2);
            ov96_021EAF94(obj, sp6C, sp68);
            ov96_021EAF6C(obj, ov96_021E6104());
            ov96_021EB0A4(obj, sp6C, sp68, &x, &y);
            heapPtr->sub[q][r].unk14 = x << 12;
            heapPtr->sub[q][r].unk18 = y << 12;
            heapPtr->sub[q][r].unk30 = x << 12;
            heapPtr->sub[q][r].unk34 = y << 12;
            heapPtr->sub[q][r].unk24 = x << 12;
            heapPtr->sub[q][r].unk28 = y << 12;
            if (q == ov96_021E5F24(param0)) {
                ov96_021EABA8(obj, 8);
            } else {
                ov96_021EABA8(obj, 16);
            }
            ov96_021EABDC(obj, 20);
            if (q == myIdx) {
                posB4[r].x = x;
                posB4[r].y = y;
                ov96_021E64F8(param0, obj, heapPtr->unk744, ov96_021EB5E8(heapPtr->unk0), 1);
            }
        }
        ov96_021E634C(param0, 0, heapPtr->unk744, heapPtr->unk0, 1, 3, posB4);
        if (ov96_021E5F24(param0) == 0) {
            ReadWholeNarcMemberByIdPair(narc2, 0xAA, 8);
            for (i = 0; i < 4; i++) {
                heapPtr->unk734[i] = 12;
                for (j = 0; j < 3; j++) {
                    ov96_0221359C(param0, narc2, i, j, &heapPtr->sub[i][j]);
                    ov96_02214A6C(heapPtr->unk74C, j + i * 3, &heapPtr->sub[i][j]);
                }
            }
            ov96_02214B74(heapPtr->unk74C, heapPtr->elem);
            ov96_02214A9C(heapPtr->unk74C);
            v90 = ov96_0221D220;
            VEC_Normalize(&v90, &v90);
            v84 = ov96_0221D250;
            heapPtr->unk754[0] = v84;
            heapPtr->unk754[1] = v84;
            heapPtr->unk754[1].x *= -1;
            v78 = ov96_0221D208;
            heapPtr->unk754[2] = v78;
            heapPtr->unk754[3] = v78;
            heapPtr->unk754[3].y *= -1;
            heapPtr->unk754[4] = v90;
            heapPtr->unk754[5] = v90;
            heapPtr->unk754[5].x *= -1;
            heapPtr->unk754[6] = v90;
            heapPtr->unk754[6].y *= -1;
            heapPtr->unk754[7] = v90;
            heapPtr->unk754[7].x *= -1;
            heapPtr->unk754[7].y *= -1;
        }
        if (ov96_021E5F24(param0) == 0) {
            ov96_02211DE4(heapPtr, ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(param0) + 0x28));
        }
        ov96_02211A24(heapPtr);
        ov96_022147FC(heapPtr->unk750);
        GfGfx_EngineATogglePlanes(16, 1);
        GfGfx_EngineBTogglePlanes(16, 1);
        heapPtr->unk738 = 0xA8C;
        ov96_0221490C(heapPtr->unk750, heapPtr->unk738);
        sub_0203A994(2);
        BeginNormalPaletteFade(2, 3, 3, 0, 6, 1, heapPtr->heapID);
        PokeathlonCourse_IncrementField1ED(param0);
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_port_nonmatching_ov96_02210C98_c_0(void) {
    PORT_AssetAdd("IPKE", "port/nonmatching/ov96_02210C98.c:hgasset_ov96_02210C98_pos", (void *)&hgasset_ov96_02210C98_pos, sizeof(hgasset_ov96_02210C98_pos), "u8");
    PORT_AssetAdd("IPGE", "port/nonmatching/ov96_02210C98.c:hgasset_ov96_02210C98_pos", (void *)&hgasset_ov96_02210C98_pos, sizeof(hgasset_ov96_02210C98_pos), "u8");
}
