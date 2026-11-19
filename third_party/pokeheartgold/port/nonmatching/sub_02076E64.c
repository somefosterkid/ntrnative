/* PORTOK hk42 */
#include "global.h"

struct ApplicationManager;
struct ApplicationManagerTemplate;
struct Background;
struct Bag;
struct BgConfig;
struct BgTemplate;
struct EvolutionData;
struct G3DPipelineBuffers;
struct GXBanks;
struct GameRecords;
struct GraphicsModes;
struct MessageLoader;
struct NARC;
struct Options;
struct PaletteAnimator;
struct PaletteData;
struct ParticleSystem;
struct Party;
struct Pokedex;
struct Pokemon;
struct PokemonAnimManager;
struct PokemonSprite;
struct PokemonSpriteManager;
struct PokemonSpriteShadow;
struct PokemonSpriteTemplate;
struct PokemonSpriteTransforms;
struct PokemonSummary;
struct Poketch;
struct SPLEmitter;
struct SpriteAnimFrame;
struct String;
struct StringTemplate;
struct SysTask;
struct SysTaskManager;
struct Window;
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
typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;
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
typedef struct SPLEmitter SPLEmitter;
typedef struct ParticleSystem ParticleSystem;
typedef struct {
    UnkStruct_0207C894 unk_00;
    SPLEmitter *unk_08;
    ParticleSystem *unk_0C;
} UnkStruct_0207C8C4;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
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
enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
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
typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;

    int xOffset;
    int yOffset;

    u8 type;
    u8 screenSize;
    u8 colorMode;
    u8 tileSize;

    u16 rotation;

    fx32 xScale;
    fx32 yScale;
    int xCenter;
    int yCenter;
} Background;
typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    Background bgs[8];
} BgConfig;
typedef struct Window Window;
typedef struct GraphicsModes {
    GXDispMode displayMode;
    GXBGMode mainBgMode;
    GXBGMode subBgMode;
    GXBG0As bg0As2DOr3D;
} GraphicsModes;
void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);
void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);
void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);
typedef struct String String;
typedef struct Options Options;
int Options_GetFrame(const Options *options);
typedef struct NARC NARC;
typedef struct MessageLoader MessageLoader;
typedef struct Pokemon Pokemon;
enum PaletteBufferID {
    PLTTBUF_MAIN_BG = 0,
    PLTTBUF_SUB_BG,
    PLTTBUF_MAIN_OBJ,
    PLTTBUF_SUB_OBJ,
    PLTTBUF_EX_BEGIN,

    PLTTBUF_MAIN_EX_BG_0 = PLTTBUF_EX_BEGIN,
    PLTTBUF_MAIN_EX_BG_1,
    PLTTBUF_MAIN_EX_BG_2,
    PLTTBUF_MAIN_EX_BG_3,
    PLTTBUF_SUB_EX_BG_0,
    PLTTBUF_SUB_EX_BG_1,
    PLTTBUF_SUB_EX_BG_2,
    PLTTBUF_SUB_EX_BG_3,
    PLTTBUF_MAIN_EX_OBJ,
    PLTTBUF_SUB_EX_OBJ,

    PLTTBUF_MAX,
};
typedef struct PaletteData PaletteData;
void PaletteData_LoadNarc(PaletteData *paletteData, enum NarcID narcID, u32 narcMemberIdx, enum HeapID heapID, enum PaletteBufferID bufferID, u32 srcSize, u16 destStart);
typedef struct Party Party;
typedef struct PokemonSprite PokemonSprite;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);
typedef struct PokemonSpriteTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokemonSpriteTemplate;
struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};
typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;
struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokemonSpriteTemplate template;
    PokemonSpriteTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10 ];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10 ];
};
typedef struct PokemonSpriteManager PokemonSpriteManager;
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
typedef struct PokemonAnimManager PokemonAnimManager;
typedef struct StringTemplate StringTemplate;
u32 GfGfxLoader_LoadCharData(enum NarcID narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);
void GfGfxLoader_LoadScrnData(enum NarcID narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);
typedef struct PokemonSummary PokemonSummary;
typedef struct Bag Bag;
typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;
enum VramManagerType {
    VRAM_MANAGER_TYPE_LINKED_LIST = 0,
    VRAM_MANAGER_TYPE_FRAME,
};
enum TextureVramSize {
    TEXTURE_VRAM_SIZE_128K = 1,
    TEXTURE_VRAM_SIZE_256K,
    TEXTURE_VRAM_SIZE_384K,
    TEXTURE_VRAM_SIZE_512K,
};
enum PaletteVramSize {
    PALETTE_VRAM_SIZE_16K = 1,
    PALETTE_VRAM_SIZE_32K,
    PALETTE_VRAM_SIZE_48K,
    PALETTE_VRAM_SIZE_64K,
    PALETTE_VRAM_SIZE_80K,
    PALETTE_VRAM_SIZE_96K,
};
typedef void (*G3DPipelineSetupCallback)(void);
G3DPipelineBuffers *GF_3DVramMan_Create(enum HeapID heapID, enum VramManagerType texVramManagerType, enum TextureVramSize texVramSize, enum VramManagerType plttVramManagerType, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb);
static inline G3DPipelineBuffers *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb)
{
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}
typedef struct Pokedex Pokedex;
typedef struct GameRecords GameRecords;
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
typedef struct Poketch Poketch;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
typedef struct EvolutionData {
    BgConfig *bgConfig;
    Window *window;
    MessageLoader *msgLoader;
    StringTemplate *strTemplate;
    String *string;
    PaletteData *paletteData;
    PokemonSpriteManager *monSpriteMan;
    PokemonSprite *monSprites[2];
    Party *party;
    Pokemon *mon;
    Options *options;
    UnkStruct_0207C8C4 *unk_30;
    G3DPipelineBuffers *pipelineBuffers;
    ApplicationManager *summaryScreenAppMan;
    PokemonSummary *monSummary;
    void *menu;
    PokemonAnimManager *monAnimMan;
    Pokedex *pokedex;
    Bag *bag;
    GameRecords *records;
    Poketch *poketch;
    PaletteAnimator *unk_58;
    enum HeapID heapID;
    u16 currentSpecies;
    u16 targetSpecies;
    u8 state;
    u8 printerID;
    u8 delay;
    u8 done;
    int levelUpMoveIndex;
    u16 moveID;
    u8 moveSlot;
    u8 unused;
    u8 shrinkAndGrowState;
    u8 attributeDelta;
    u8 windowLeft;
    u8 windowTop;
    u8 windowRight;
    u8 windowBottom;
    int method;
    int flags;
    NARC *narc;
    u8 currentSpeciesCryDelay;
    u8 targetSpeciesCryDelay;
    u8 form;
} EvolutionData;
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
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);
void GfGfx_BothDispOn(void);
void sub_0200E398(BgConfig *bgConfig, u8 bgLayer, u16 offset, u8 standardWindowType, u32 heapID);
u32 GetStandardWindowPaletteNARCMember(void);
u32 GetMessageBoxPaletteNARCMember(u32 messageBoxFrame);
void sub_0200EB80(BgConfig *bgConfig, u8 bgLayer, u16 bgBaseTile, u8 withTile, u8 messageBoxFrame, u32 heapID);
typedef void (*Callback)(void *);
void Main_SetVBlankIntrCB(Callback cb, void *data);
void sub_02077270(void *data);
EvolutionData *sub_02075A7C(Party *party, Pokemon *mon, int targetSpecies, Options *options, int showContest, Pokedex *pokedex, Bag *bag, GameRecords *records, Poketch *poketch, int method, int flags, enum HeapID heapID);





void sub_02076E64(EvolutionData *evolutionData, BgConfig *bgConfig);
void GfGfx_EngineBTogglePlanes(u8 planeMask, u8 enable);
NARC *NARC_New(enum NarcID narcID, u32 heapID);
void NARC_Delete(NARC *narc);
void GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);
void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);
void ToggleBgLayer(u8 bgLayer, u8 flag);
extern const GXBanks _020FFF34;
extern const GraphicsModes _020FFED8;
typedef struct {
    BgTemplate a[3];
} _020FFF90_Array;
extern const _020FFF90_Array _020FFF90;
typedef struct {
    BgTemplate a[3];
} _020FFFE4_Array;
extern const _020FFFE4_Array _020FFFE4;
u32 sub_0200E640(u32 frame);
u32 sub_0200E3D8(void);
void sub_02076E64(EvolutionData *evolutionData, BgConfig *bgConfig)
{
  GfGfx_DisableEngineAPlanes();
  GXBanks banks = _020FFF34;
  GfGfx_SetBanks(&banks);
  MI_CpuClear32((void *) HW_BG_VRAM, 0x06080000 - 0x06000000);
  MI_CpuClear32((void *) HW_DB_BG_VRAM, 0x06220000 - 0x06200000);
  MI_CpuClear32((void *) HW_OBJ_VRAM, 0x06440000 - 0x06400000);
  MI_CpuClear32((void *) HW_DB_OBJ_VRAM, 0x06620000 - 0x06600000);
  GraphicsModes graphicsModes = _020FFED8;
  SetBothScreensModesAndDisable(&graphicsModes);
  _020FFF90_Array bgTemplates = _020FFF90;
  InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_1, &bgTemplates.a[0], BG_TYPE_STATIC);
  BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_1);
  InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_2, &bgTemplates.a[1], BG_TYPE_STATIC);
  BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_2);
  InitBgFromTemplate(bgConfig, BG_LAYER_MAIN_3, &bgTemplates.a[2], BG_TYPE_STATIC);
  BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_MAIN_3);
  G2_SetBG0Priority(1);
  GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
  _020FFFE4_Array bgTemplate = _020FFFE4;
  InitBgFromTemplate(bgConfig, BG_LAYER_SUB_0, &bgTemplate.a[0], BG_TYPE_STATIC);
  BgClearTilemapBufferAndCommit(bgConfig, BG_LAYER_SUB_0);
  InitBgFromTemplate(bgConfig, 5, &bgTemplate.a[1], BG_TYPE_STATIC);
  BgClearTilemapBufferAndCommit(bgConfig, 5);
  InitBgFromTemplate(bgConfig, 6, &bgTemplate.a[2], BG_TYPE_STATIC);
  BgClearTilemapBufferAndCommit(bgConfig, 6);
  int frame = Options_GetFrame(evolutionData->options);
  sub_0200EB80(bgConfig, BG_LAYER_MAIN_1, 1, 10, frame, evolutionData->heapID);
  GfGfxLoader_LoadCharData((enum NarcID) 115, 0, bgConfig, BG_LAYER_MAIN_3, 0, 0, 1, evolutionData->heapID);
  GfGfxLoader_LoadScrnData((enum NarcID) 115, 1, bgConfig, BG_LAYER_MAIN_3, 0, 0, 1, evolutionData->heapID);
  PaletteData_LoadNarc(evolutionData->paletteData, (enum NarcID) 115, 8, evolutionData->heapID, PLTTBUF_MAIN_BG, (16 * (sizeof(u16))) * 2, 0 * 16);
  PaletteData_LoadNarc(evolutionData->paletteData, NARC_INDEX_GRAPHIC__PL_WINFRAME, sub_0200E640(frame), evolutionData->heapID, PLTTBUF_MAIN_BG, 16 * (sizeof(u16)), 10 * 16);
  PaletteData_LoadNarc(evolutionData->paletteData, (enum NarcID) 16, 8, evolutionData->heapID, PLTTBUF_MAIN_BG, 16 * (sizeof(u16)), 11 * 16);
  sub_0200E398(evolutionData->bgConfig, BG_LAYER_MAIN_2, 1, 0, evolutionData->heapID);
  PaletteData_LoadNarc(evolutionData->paletteData, NARC_INDEX_GRAPHIC__PL_WINFRAME, sub_0200E3D8(), evolutionData->heapID, PLTTBUF_MAIN_BG, 16 * (sizeof(u16)), 8 * 16);
  PaletteData_LoadNarc(evolutionData->paletteData, (enum NarcID) 239, 0, evolutionData->heapID, PLTTBUF_SUB_BG, 0xa0, 0);
  PaletteData_LoadNarc(evolutionData->paletteData, (enum NarcID) 239, 0xf, evolutionData->heapID, PLTTBUF_SUB_BG, 0x20, 0x90);
  PaletteData_LoadNarc(evolutionData->paletteData, (enum NarcID) 16, 9, evolutionData->heapID, PLTTBUF_SUB_BG, 0x20, 0xf0);
  NARC *narc = NARC_New((enum NarcID) 239, evolutionData->heapID);
  GfGfxLoader_LoadCharDataFromOpenNarc(narc, 16, bgConfig, BG_LAYER_SUB_0, 0, 0, 0, evolutionData->heapID);
  GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, bgConfig, BG_LAYER_SUB_0, 0, 0, 0, evolutionData->heapID);
  GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bgConfig, BG_LAYER_SUB_1, 0, 0, 0, evolutionData->heapID);
  GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, bgConfig, BG_LAYER_SUB_1, 0, 0, 0, evolutionData->heapID);
  NARC_Delete(narc);
  ToggleBgLayer(BG_LAYER_SUB_1, 0);
  ToggleBgLayer(BG_LAYER_SUB_2, 0);
  GX_SetVisibleWnd(GX_WNDMASK_W0);
  GXS_SetVisibleWnd(GX_WNDMASK_NONE);
  G2_SetWnd0InsidePlane((((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1) | GX_WND_PLANEMASK_BG2) | GX_WND_PLANEMASK_BG3) | GX_WND_PLANEMASK_OBJ, 0L);
  G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_OBJ, 0L);
  evolutionData->windowLeft = 0;
    evolutionData->windowTop = 0;
  evolutionData->windowRight = 256 - 1;
  evolutionData->windowBottom = 160;
  GfGfx_BothDispOn();
  GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
  GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
  Main_SetVBlankIntrCB(sub_02077270, evolutionData);
}

