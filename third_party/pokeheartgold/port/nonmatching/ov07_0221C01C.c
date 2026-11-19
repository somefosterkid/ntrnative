/* PORTOK hk39 */
#include "global.h"

struct Background;
struct BattleAnimBattlerContext;
struct BattleAnimContext;
struct BattleAnimScriptLoop;
struct BattleAnimSystem;
struct BattleBackgroundReference;
struct BattleBgAnim;
struct BattleBgSwitch;
struct BattleMonOBJData;
struct BattleSystem;
struct BgConfig;
struct BgTrackingTask;
struct BgWaveStrip;
struct BufferManager;
struct Camera;
struct CameraAngle;
struct CameraLookAt;
struct CameraPerspectiveProjection;
struct CameraPositionHistory;
struct ChatotCry;
struct CustomBgScrollContext;
struct G2dRenderer;
struct ManagedSprite;
struct MoveAnimation;
struct NARC;
struct PaletteBuffer;
struct PaletteData;
struct PaletteFadeControl;
struct ParticleSystem;
struct PokemonSprite;
struct PokemonSpriteData;
struct PokemonSpriteDrawContext;
struct PokemonSpriteShadow;
struct PokemonSpriteTemplate;
struct PokemonSpriteTrackingTask;
struct PokemonSpriteTransforms;
struct Resource;
struct ResourceCollection;
struct SPLAlphaAnim;
struct SPLBehavior;
struct SPLChildResource;
struct SPLColorAnim;
struct SPLEmitter;
struct SPLEmitterList;
struct SPLEmitterState;
struct SPLManager;
struct SPLParticle;
struct SPLParticleList;
struct SPLResource;
struct SPLResourceHeader;
struct SPLScaleAnim;
struct SPLTexAnim;
struct SPLTexture;
struct Sprite;
struct SpriteAnimFrame;
struct SpriteList;
struct SpriteManager;
struct SpriteResource;
struct SpriteResourceCollection;
struct SpriteResourceList;
struct SpriteResourceTable;
struct SpriteResourcesHeader;
struct SpriteResourcesHeaderList;
struct SpriteSystem;
struct SpriteSystemContext;
struct SpriteTemplate;
struct SpriteTrackingTask;
struct SysTask;
struct SysTaskManager;
struct VBlankDMAController;
struct WaveAnimContext;
enum BattleTerrain {
    TERRAIN_PLAIN            =  0,
    TERRAIN_SAND             =  1,
    TERRAIN_GRASS            =  2,
    TERRAIN_PUDDLE           =  3,
    TERRAIN_MOUNTAIN         =  4,
    TERRAIN_CAVE             =  5,
    TERRAIN_SNOW             =  6,
    TERRAIN_WATER            =  7,
    TERRAIN_ICE              =  8,
    TERRAIN_BUILDING         =  9,
    TERRAIN_GREAT_MARSH      = 10,
    TERRAIN_BRIDGE           = 11,
    TERRAIN_AARON            = 12,
    TERRAIN_BERTHA           = 13,
    TERRAIN_FLINT            = 14,
    TERRAIN_LUCIAN           = 15,
    TERRAIN_CYNTHIA          = 16,
    TERRAIN_DISTORTION_WORLD = 17,
    TERRAIN_BATTLE_TOWER     = 18,
    TERRAIN_BATTLE_FACTORY   = 19,
    TERRAIN_BATTLE_ARCADE    = 20,
    TERRAIN_BATTLE_CASTLE    = 21,
    TERRAIN_BATTLE_HALL      = 22,
    TERRAIN_GIRATINA         = 23,
    TERRAIN_MAX              = 24,
};
typedef struct BattleSystem BattleSystem;
typedef struct MoveAnimation {
    u8 command;
    u8 unk_01;
    u16 move;
    s32 damage;
    u16 power;
    u16 effectChance;
    u16 friendship;
    u16 isSubstitute : 1;
    u16 isTransformed : 1;
    u16 unk_0E_2 : 1;  // HGSS: two more flags the anim context copies
    u16 unk_0E_3 : 1;
    u16 : 12;
    u32 fieldConditions;
    u16 attacker;
    u16 defender;
    u16 species[4 ];
    u8 genders[4 ];
    u8 isShiny[4 ];
    u8 formNums[4 ];
    u32 personalities[4 ];
    u32 moveEffectMasks[4 ];
    int animMode;
    int secondaryAnimID;
    int terrain;
} MoveAnimation;
typedef struct ChatotCry {
    BOOL valid;
    s8 audioData[1000 ];
} ChatotCry;
typedef struct PokemonSpriteData {
    u8 *tiles;
    int narcID;
    int palette;
    int yOffset;
} PokemonSpriteData;
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
u8 GetBgPriority(BgConfig *bgConfig, u8 bgLayer);
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
typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;
typedef struct PaletteBuffer {
    u16 *unfaded;
    u16 *faded;
    u32 size;
    PaletteFadeControl selected;
} PaletteBuffer;
typedef struct PaletteData {
    PaletteBuffer buffers[14];
    u16 selectedFlag : 2;
    u16 selectedBuffers : 14;
    u16 fadedBuffers : 14;
    u16 fadeInProgress : 1;
    u16 autoTransparent : 1;
    u8 forceExit;
} PaletteData;
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
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
typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;
void *AllocAndReadWholeNarcMemberByIdPair(enum NarcID narcID, int memberIndex, enum HeapID heapID);
typedef struct SpriteList SpriteList;
typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;



    u32 animData[29 ];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;
struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};
typedef struct SpriteResourcesHeader {
    const NNSG2dImageProxy *imageProxy;
    const NNSG2dCharacterData *charData;
    const NNSG2dImagePaletteProxy *paletteProxy;
    NNSG2dCellDataBank *cellBank;
    const NNSG2dCellAnimBankData *cellAnimBank;
    const NNSG2dMultiCellDataBank *multiCellBank;
    const NNSG2dMultiCellAnimBankData *multiCellAnimBank;
    BOOL isVRamTransfer;
    u8 priority;
    u8 padding_21[3];
} SpriteResourcesHeader;
typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;
typedef struct ManagedSprite {
    Sprite *sprite;
    SpriteResourcesHeader *resourceHeader;
    SpriteResourcesHeaderList *resourceHeaderList;
    BOOL vramTransfer;
} ManagedSprite;
typedef struct Resource {
    int id;
    void *data;
} Resource;
typedef struct ResourceCollection {
    Resource *resources;
    int capacity;
    int count;
} ResourceCollection;
enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};
typedef struct SpriteResource {
    Resource *rawResource;
    enum SpriteResourceType type;
    void *data;
} SpriteResource;
typedef struct SpriteResourceCollection {
    ResourceCollection *collection;
    SpriteResource *resources;
    int capacity;
    int count;
    enum SpriteResourceType type;
} SpriteResourceCollection;
typedef struct SpriteResourceTable {
    void *entries;
    int count;
    enum SpriteResourceType type;
    u8 loadFromNARC;
} SpriteResourceTable;
typedef struct SpriteResourceList {
    SpriteResource **resources;
    int capacity;
    int count;
} SpriteResourceList;
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct SpriteSystem {
    enum HeapID heapID;
    int spriteManagerCount;
    BOOL inUse;
    NNSG2dCellTransferState *cellTransferStates;
    G2dRenderer renderer;
} SpriteSystem;
typedef struct SpriteManager {
    SpriteList *sprites;
    SpriteResourcesHeaderList *resourceHeaders;
    SpriteResourceTable *resourcePaths;
    SpriteResourceCollection *ownedResources[SPRITE_RESOURCE_MAX];
    SpriteResourceList *unownedResources[SPRITE_RESOURCE_MAX];
    int loadedResourceCount[SPRITE_RESOURCE_MAX];
    int numResourceTypes;
} SpriteManager;
typedef struct SpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animIdx;
    int priority;
    int plttIdx;
    enum NNS_G2D_VRAM_TYPE vramType;
    int resources[SPRITE_RESOURCE_MAX];
    int bgPriority;
    BOOL vramTransfer;
} SpriteTemplate;
typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;
typedef struct BattleAnimBattlerContext {
    SpriteSystem *spriteSystem;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    PokemonSpriteData *pokemonSpriteData[4 ];
    u8 battlerTypes[4 ];
    PokemonSprite *pokemonSprites[4 ];
    u32 battleType;
    u16 battlerSpecies[4 ];
    u8 battlerGenders[4 ];
    u8 battlerShinyFlags[4 ];
    u8 battlerForms[4 ];
    u32 battlerPersonalities[4 ];
    u32 battlerMoveEffects[4 ];
    int moveArcID;
    BattleBackgroundReference battleBgRef;
    ChatotCry *chatotCry;
    u8 *bgTiles;
    u16 *bgPaletteBuffer;
} BattleAnimBattlerContext;
typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;
typedef struct BattleMonOBJData {
    enum HeapID heapID;
    int battlerIdx;
    SpriteSystemContext spriteContext;
    int resourceIDs[4];
    ManagedSprite *managedSprites[4];
    PokemonSpriteData *pokemonSpriteDataArray[4];
    u8 battlerTypes[4];
    PokemonSprite *pokemonSprites[4];
} BattleMonOBJData;
enum CameraProjection {
    CAMERA_PROJECTION_PERSPECTIVE = 0,
    CAMERA_PROJECTION_ORTHOGRAPHIC,
    CAMERA_PROJECTION_COUNT
};
typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;
typedef struct CameraLookAt {
    VecFx32 position;
    VecFx32 target;
    VecFx32 up;
} CameraLookAt;
typedef struct CameraPerspectiveProjection {
    fx32 sinFovY;
    fx32 cosFovY;
    fx32 aspectRatio;
    fx32 nearClip;
    fx32 farClip;
} CameraPerspectiveProjection;
typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;
typedef struct Camera {
    CameraPerspectiveProjection perspective;
    CameraLookAt lookAt;
    fx32 distance;
    CameraAngle angle;
    u8 projection;
    u16 fovY;
    VecFx32 prevTargetPos;
    const VecFx32 *targetPos;
    BOOL trackTargetX;
    BOOL trackTargetY;
    BOOL trackTargetZ;
    CameraPositionHistory *history;
} Camera;
typedef struct SPLParticle {
    struct SPLParticle *next;
    struct SPLParticle *prev;
    VecFx32 position;
    VecFx32 velocity;
    u16 rotation;
    s16 angularVelocity;
    u16 lifeTime;
    u16 age;




    u16 loopTimeFactor;
    u16 lifeTimeFactor;

    u8 texture;




    u8 lifeRateOffset;

    struct {
        u16 baseAlpha : 5;
        u16 animAlpha : 5;
        u16 currentPolygonID : 6;
    } visibility;
    fx32 baseScale;
    fx16 animScale;
    GXRgb color;
    VecFx32 emitterPos;
} SPLParticle;
typedef struct SPLParticleList {
    SPLParticle *first;
    int count;
    SPLParticle *last;
} SPLParticleList;
typedef struct SPLBehavior {
    void (*applyFunc)(const void *, struct SPLParticle *, VecFx32 *, struct SPLEmitter *);
    const void *object;
} SPLBehavior;
typedef union SPLResourceFlags {
    u32 all;
    struct {
        u32 emissionType : 4;
        u32 drawType : 2;
        u32 circleAxis : 2;
        u32 hasScaleAnim : 1;
        u32 hasColorAnim : 1;
        u32 hasAlphaAnim : 1;
        u32 hasTexAnim : 1;
        u32 hasRotation : 1;
        u32 randomInitAngle : 1;



        u32 selfMaintaining : 1;
        u32 followEmitter : 1;
        u32 hasChildResource : 1;
        u32 polygonRotAxis : 2;
        u32 polygonReferencePlane : 1;
        u32 randomizeLoopedAnim : 1;
        u32 drawChildrenFirst : 1;
        u32 hideParent : 1;
        u32 useViewSpace : 1;
        u32 hasGravityBehavior : 1;
        u32 hasRandomBehavior : 1;
        u32 hasMagnetBehavior : 1;
        u32 hasSpinBehavior : 1;
        u32 hasCollisionPlaneBehavior : 1;
        u32 hasConvergenceBehavior : 1;
        u32 hasFixedPolygonID : 1;
        u32 childHasFixedPolygonID : 1;
    };
} SPLResourceFlags;
typedef union SPLChildResourceFlags {
    u16 all;
    struct {
        u16 usesBehaviors : 1;
        u16 hasScaleAnim : 1;
        u16 hasAlphaAnim : 1;
        u16 rotationType : 2;
        u16 followEmitter : 1;
        u16 useChildColor : 1;
        u16 drawType : 2;
        u16 polygonRotAxis : 2;
        u16 polygonReferencePlane : 1;
        u16 : 4;
    };
} SPLChildResourceFlags;
typedef union SPLCurveInOut {
    u16 all;
    struct {
        u16 in : 8;
        u16 out : 8;
    };
} SPLCurveInOut;
typedef union SPLCurveInPeakOut {
    u32 all;
    struct {
        u32 in : 8;
        u32 peak : 8;
        u32 out : 8;
        u32 : 8;
    };
} SPLCurveInPeakOut;
typedef struct SPLResourceHeader {
    SPLResourceFlags flags;
    VecFx32 emitterBasePos;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    VecFx16 axis;
    GXRgb color;
    fx32 initVelPosAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    fx16 aspectRatio;
    u16 startDelay;
    s16 minRotation;
    s16 maxRotation;
    u16 initAngle;
    u16 reserved;
    u16 emitterLifeTime;
    u16 particleLifeTime;




    struct {
        u32 baseScale : 8;
        u32 lifeTime : 8;
        u32 initVel : 8;
        u32 : 8;
    } randomAttenuation;

    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 airResistance : 8;
        u32 textureIndex : 8;
        u32 loopFrames : 8;
        u32 dbbScale : 16;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 scaleAnimDir : 3;
        u32 dpolFaceEmitter : 1;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 : 30;
    } misc;
    fx16 polygonX;
    fx16 polygonY;
    struct {
        u32 flags : 8;
        u32 : 24;
    } userData;
} SPLResourceHeader;
typedef struct SPLScaleAnim {
    fx16 start;
    fx16 mid;
    fx16 end;
    SPLCurveInOut curve;
    struct {
        u16 loop : 1;
        u16 : 15;
    } flags;
    u16 padding;
} SPLScaleAnim;
typedef struct SPLColorAnim {
    GXRgb start;
    GXRgb end;
    SPLCurveInPeakOut curve;
    struct {
        u16 randomStartColor : 1;
        u16 loop : 1;
        u16 interpolate : 1;
        u16 : 13;
    } flags;
    u16 padding;
} SPLColorAnim;
typedef struct SPLAlphaAnim {
    union {
        u16 all;
        struct {
            u16 start : 5;
            u16 mid : 5;
            u16 end : 5;
            u16 : 1;
        };
    } alpha;
    struct {
        u16 randomRange : 8;
        u16 loop : 1;
        u16 : 7;
    } flags;
    SPLCurveInOut curve;
    u16 padding;
} SPLAlphaAnim;
typedef struct SPLTexAnim {
    u8 textures[8 ];
    struct {
        u32 frameCount : 8;
        u32 step : 8;
        u32 randomizeInit : 1;
        u32 loop : 1;
        u32 : 14;
    } param;
} SPLTexAnim;
typedef struct SPLChildResource {
    SPLChildResourceFlags flags;
    fx16 randomInitVelMag;
    fx16 endScale;
    u16 lifeTime;
    u8 velocityRatio;
    u8 scaleRatio;
    GXRgb color;
    struct {
        u32 emissionCount : 8;
        u32 emissionDelay : 8;
        u32 emissionInterval : 8;
        u32 texture : 8;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 dpolFaceEmitter : 1;
        u32 : 25;
    } misc;
} SPLChildResource;
typedef struct SPLResource {
    SPLResourceHeader *header;
    SPLScaleAnim *scaleAnim;
    SPLColorAnim *colorAnim;
    SPLAlphaAnim *alphaAnim;
    SPLTexAnim *texAnim;
    SPLChildResource *childResource;
    SPLBehavior *behaviors;
    u16 behaviorCount;
    u16 reserved;
} SPLResource;
enum SPLUpdateCallbackType {
    SPL_CALLBACK_PRE_UPDATE = 0,
    SPL_CALLBACK_POST_UPDATE = 1,
};
typedef void (*SPLEmitterUpdateCallback)(struct SPLEmitter *emitter, enum SPLUpdateCallbackType type);
typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;
typedef struct SPLEmitter {
    struct SPLEmitter *next;
    struct SPLEmitter *prev;
    SPLParticleList particles;
    SPLParticleList childParticles;
    SPLResource *resource;
    SPLEmitterState state;
    VecFx32 position;
    VecFx32 velocity;
    VecFx32 particleInitVelocity;
    u16 age;
    fx16 emissionCountFractional;
    VecFx16 axis;
    u16 initAngle;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    fx32 initVelPositionAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    u16 particleLifeTime;
    GXRgb color;
    fx32 collisionPlaneHeight;
    fx16 textureS;
    fx16 textureT;
    fx16 childTextureS;
    fx16 childTextureT;
    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 updateCycle : 3;
        u32 reserved : 13;
    } misc;
    VecFx16 crossAxis1;
    VecFx16 crossAxis2;
    SPLEmitterUpdateCallback updateCallback;


    void *userDataPtr;
    union {
        u32 u32;
        u16 u16[2];
        u8 u8[4];
    } userData;
} SPLEmitter;
typedef struct SPLEmitterList {
    SPLEmitter *first;
    int count;
    SPLEmitter *last;
} SPLEmitterList;
typedef union SPLTextureParam {
    u32 all;
    struct {
        u32 format : 4;
        u32 s : 4;
        u32 t : 4;
        u32 repeat : 2;
        u32 flip : 2;
        u32 palColor0 : 1;
        u32 useSharedTexture : 1;
        u32 sharedTexID : 8;
        u32 : 6;
    };
} SPLTextureParam;
typedef struct SPLTexture {
    const void *resource;
    u32 texAddr;
    u32 palAddr;
    SPLTextureParam param;
    u16 width;
    u16 height;
} SPLTexture;
typedef void *(*SPLAllocFunc)(u32);
typedef struct SPLManager {
    SPLAllocFunc alloc;
    SPLEmitterList activeEmitters;
    SPLEmitterList inactiveEmitters;
    SPLParticleList inactiveParticles;
    SPLResource *resources;
    SPLTexture *textures;
    u16 resCount;
    u16 texCount;
    u16 maxEmitters;
    u16 maxParticles;
    struct {
        u32 min : 6;
        u32 max : 6;
        u32 current : 6;
        u32 fix : 6;
        u32 drawOrder : 1;
        u32 unused : 7;
    } polygonID;
    GXPolygonAttrMisc miscPolygonAttr;
    struct {
        SPLEmitter *emitter;
        const MtxFx43 *viewMatrix;
    } renderState;
    u16 currentCycle;
    u16 reserved;
} SPLManager;
typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);
typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);
typedef struct ParticleSystem {
    SPLManager *manager;
    void *resource;
    SPLEmitter *lastAddedEmitter;
    void *heapStart;
    void *heap;
    void *heapEnd;
    SPLTexVRAMAllocFunc texAllocFunc;
    SPLPalVRAMAllocFunc palAllocFunc;
    Camera *camera;
    VecFx32 sLotoId;
    u16 cameraFov;
    VecFx32 cameraPos;
    VecFx32 cameraUp;
    VecFx32 cameraTarget;
    union {
        NNSGfdFrmTexVramState textureVRAMState;
        NNSGfdTexKey textureKeys[16 ];
    };
    union {
        NNSGfdFrmPlttVramState paletteVRAMState;
        NNSGfdPlttKey paletteKeys[16 ];
    };
    u8 vramAutoRelease;
    u8 unused2;
    u8 id;
    u8 cameraProjection;
} ParticleSystem;
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
enum BattleAnimSystemArc {
    BATTLE_ANIM_SYSTEM_ARC_BATT_BG = 0,
    BATTLE_ANIM_SYSTEM_ARC_BATT_OBJ,
    BATTLE_ANIM_SYSTEM_ARC_WECHAR,
    BATTLE_ANIM_SYSTEM_ARC_WEPLTT,
    BATTLE_ANIM_SYSTEM_ARC_WECELL,
    BATTLE_ANIM_SYSTEM_ARC_WECELLANM,

    BATTLE_ANIM_SYSTEM_ARC_COUNT,
};
typedef struct BattleAnimContext {
    u8 command;
    u8 unk_01;


    u16 move;
    s32 damage;
    u16 power;
    u16 friendship;
    u32 fieldConditions;
    u16 effectChance;
    u16 terrain;

    u16 attacker;
    u16 defender;
    int currentParticleSystem;
    ParticleSystem *particleSystems[16 ];
    SPLEmitter *emitters[20 ];
    SpriteSystem *spriteSystem;


    PokemonSpriteData *pokemonSpriteData[4 ];
    u8 battlerTypes[4 ];
    PokemonSprite *battlerSprites[4 ];
    u32 battleType;
    u16 battlerSpecies[4 ];
    u8 battlerGenders[4 ];
    u8 battlerShinyFlags[4 ];
    u8 battlerForms[4 ];
    u32 battlerPersonalities[4 ];
    u32 battlerMoveEffects[4 ];
    ChatotCry *chatotCry;
    u8 *bgTiles;
    u16 *bgPaletteBuffer;
    u32 isTransformed : 1;  // 0x118 (HGSS: a bitfield word, not a BOOL)
    u32 unk_118_1 : 1;
    u32 unk_118_2 : 1;
    u32 : 29;
} BattleAnimContext;
typedef struct CustomBgScrollContext CustomBgScrollContext;
typedef struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
} BattleAnimScriptLoop;
typedef struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
} PokemonSpriteDrawContext;
typedef struct BgWaveStrip {
    s16 startScanline;
    s16 endScanline;
    s16 scrollSpeed;
    s16 scrollAccum;
    int baseOffset;
} BgWaveStrip;
typedef struct WaveAnimContext {
    BgWaveStrip strips[16];
    struct CustomBgScrollContext *bgScrollCtx;
} WaveAnimContext;
typedef struct BattleBgAnim {
    BgConfig *bgConfig;
    s16 offsetX;
    s16 offsetY;
    s16 stepX;
    s16 stepY;
    int bg;
    int unusedBg;
    BOOL unused;
    BOOL cancel;
    WaveAnimContext *waveAnimCtx;
} BattleBgAnim;
typedef struct PokemonSpriteTrackingTask {
    u8 frameCount;
    u8 interval;
    PokemonSprite *sprite;
} PokemonSpriteTrackingData;
typedef struct SpriteTrackingTask {
    ManagedSprite *sprite;
    PokemonSpriteTrackingData data;
    SysTask *task;
} SpriteTrackingTask;
typedef struct BgTrackingTask {
    BgConfig *bg;
    PokemonSpriteTrackingData data;
    SysTask *task;
} BgTrackingTask;
typedef struct BattleAnimSystem {
    enum HeapID heapID;
    enum NarcID moveArcID;
    BOOL isContest;
    BOOL isActive;
    BOOL moveActive;
    void *scriptData;
    u32 *scriptPtr;
    u32 *callStack[3];
    BattleAnimScriptLoop loopStack[3];
    PokemonSpriteDrawContext pokemonSpriteDrawContexts[2];
    u8 cameraProjections[16];
    u8 particleSystemCameraFlip[16];
    s8 unused_88;
    u8 scriptDelay;
    u16 activeAnimTasks;
    u16 activeSoundTasks;
    s32 scriptVars[10];
    void (*executeAnimScriptFunc)(struct BattleAnimSystem *);
    BattleAnimContext *context;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    SpriteManager *spriteManagers[4];
    ManagedSprite *sprites[10];
    SpriteTemplate lastSpriteTemplate;
    SpriteManager *pokemonSpriteManager;
    ManagedSprite *pokemonSprites[5];
    BOOL usedPokemonSprites[5];
    SpriteTrackingTask *spriteTrackingTasks[5];
    BgTrackingTask *bgTrackingTask;
    u8 bgSwitchState;
    u8 soundEffectWaitTimer;
    u8 endDelayFrames;
    u8 unused_17B;
    BattleBgAnim *bgAnims[4];  // 0x180 (Platinum: one bgAnim)
    BattleBackgroundReference battleBgRefs;  // 0x190
    int baseBgPalettes;
    u8 bgLayerPriorities[4];
    NARC *arcs[BATTLE_ANIM_SYSTEM_ARC_COUNT];
} BattleAnimSystem;
BOOL GF3dGfxRawResObj_GetTex4x4Key(BattleAnimSystem *system);
BOOL BattleAnimSystem_IsActive(BattleAnimSystem *system);
void ov07_0221C69C(void);
enum Move {
    MOVE_NONE          =   0,
    MOVE_POUND         =   1,
    MOVE_KARATE_CHOP   =   2,
    MOVE_DOUBLE_SLAP   =   3,
    MOVE_COMET_PUNCH   =   4,
    MOVE_MEGA_PUNCH    =   5,
    MOVE_PAY_DAY       =   6,
    MOVE_FIRE_PUNCH    =   7,
    MOVE_ICE_PUNCH     =   8,
    MOVE_THUNDER_PUNCH =   9,
    MOVE_SCRATCH       =  10,
    MOVE_VICE_GRIP     =  11,
    MOVE_GUILLOTINE    =  12,
    MOVE_RAZOR_WIND    =  13,
    MOVE_SWORDS_DANCE  =  14,
    MOVE_CUT           =  15,
    MOVE_GUST          =  16,
    MOVE_WING_ATTACK   =  17,
    MOVE_WHIRLWIND     =  18,
    MOVE_FLY           =  19,
    MOVE_BIND          =  20,
    MOVE_SLAM          =  21,
    MOVE_VINE_WHIP     =  22,
    MOVE_STOMP         =  23,
    MOVE_DOUBLE_KICK   =  24,
    MOVE_MEGA_KICK     =  25,
    MOVE_JUMP_KICK     =  26,
    MOVE_ROLLING_KICK  =  27,
    MOVE_SAND_ATTACK   =  28,
    MOVE_HEADBUTT      =  29,
    MOVE_HORN_ATTACK   =  30,
    MOVE_FURY_ATTACK   =  31,
    MOVE_HORN_DRILL    =  32,
    MOVE_TACKLE        =  33,
    MOVE_BODY_SLAM     =  34,
    MOVE_WRAP          =  35,
    MOVE_TAKE_DOWN     =  36,
    MOVE_THRASH        =  37,
    MOVE_DOUBLE_EDGE   =  38,
    MOVE_TAIL_WHIP     =  39,
    MOVE_POISON_STING  =  40,
    MOVE_TWINEEDLE     =  41,
    MOVE_PIN_MISSILE   =  42,
    MOVE_LEER          =  43,
    MOVE_BITE          =  44,
    MOVE_GROWL         =  45,
    MOVE_ROAR          =  46,
    MOVE_SING          =  47,
    MOVE_SUPERSONIC    =  48,
    MOVE_SONIC_BOOM    =  49,
    MOVE_DISABLE       =  50,
    MOVE_ACID          =  51,
    MOVE_EMBER         =  52,
    MOVE_FLAMETHROWER  =  53,
    MOVE_MIST          =  54,
    MOVE_WATER_GUN     =  55,
    MOVE_HYDRO_PUMP    =  56,
    MOVE_SURF          =  57,
    MOVE_ICE_BEAM      =  58,
    MOVE_BLIZZARD      =  59,
    MOVE_PSYBEAM       =  60,
    MOVE_BUBBLE_BEAM   =  61,
    MOVE_AURORA_BEAM   =  62,
    MOVE_HYPER_BEAM    =  63,
    MOVE_PECK          =  64,
    MOVE_DRILL_PECK    =  65,
    MOVE_SUBMISSION    =  66,
    MOVE_LOW_KICK      =  67,
    MOVE_COUNTER       =  68,
    MOVE_SEISMIC_TOSS  =  69,
    MOVE_STRENGTH      =  70,
    MOVE_ABSORB        =  71,
    MOVE_MEGA_DRAIN    =  72,
    MOVE_LEECH_SEED    =  73,
    MOVE_GROWTH        =  74,
    MOVE_RAZOR_LEAF    =  75,
    MOVE_SOLAR_BEAM    =  76,
    MOVE_POISON_POWDER =  77,
    MOVE_STUN_SPORE    =  78,
    MOVE_SLEEP_POWDER  =  79,
    MOVE_PETAL_DANCE   =  80,
    MOVE_STRING_SHOT   =  81,
    MOVE_DRAGON_RAGE   =  82,
    MOVE_FIRE_SPIN     =  83,
    MOVE_THUNDER_SHOCK =  84,
    MOVE_THUNDERBOLT   =  85,
    MOVE_THUNDER_WAVE  =  86,
    MOVE_THUNDER       =  87,
    MOVE_ROCK_THROW    =  88,
    MOVE_EARTHQUAKE    =  89,
    MOVE_FISSURE       =  90,
    MOVE_DIG           =  91,
    MOVE_TOXIC         =  92,
    MOVE_CONFUSION     =  93,
    MOVE_PSYCHIC       =  94,
    MOVE_HYPNOSIS      =  95,
    MOVE_MEDITATE      =  96,
    MOVE_AGILITY       =  97,
    MOVE_QUICK_ATTACK  =  98,
    MOVE_RAGE          =  99,
    MOVE_TELEPORT      = 100,
    MOVE_NIGHT_SHADE   = 101,
    MOVE_MIMIC         = 102,
    MOVE_SCREECH       = 103,
    MOVE_DOUBLE_TEAM   = 104,
    MOVE_RECOVER       = 105,
    MOVE_HARDEN        = 106,
    MOVE_MINIMIZE      = 107,
    MOVE_SMOKE_SCREEN  = 108,
    MOVE_CONFUSE_RAY   = 109,
    MOVE_WITHDRAW      = 110,
    MOVE_DEFENSE_CURL  = 111,
    MOVE_BARRIER       = 112,
    MOVE_LIGHT_SCREEN  = 113,
    MOVE_HAZE          = 114,
    MOVE_REFLECT       = 115,
    MOVE_FOCUS_ENERGY  = 116,
    MOVE_BIDE          = 117,
    MOVE_METRONOME     = 118,
    MOVE_MIRROR_MOVE   = 119,
    MOVE_SELFDESTRUCT  = 120,
    MOVE_EGG_BOMB      = 121,
    MOVE_LICK          = 122,
    MOVE_SMOG          = 123,
    MOVE_SLUDGE        = 124,
    MOVE_BONE_CLUB     = 125,
    MOVE_FIRE_BLAST    = 126,
    MOVE_WATERFALL     = 127,
    MOVE_CLAMP         = 128,
    MOVE_SWIFT         = 129,
    MOVE_SKULL_BASH    = 130,
    MOVE_SPIKE_CANNON  = 131,
    MOVE_CONSTRICT     = 132,
    MOVE_AMNESIA       = 133,
    MOVE_KINESIS       = 134,
    MOVE_SOFTBOILED    = 135,
    MOVE_HI_JUMP_KICK  = 136,
    MOVE_GLARE         = 137,
    MOVE_DREAM_EATER   = 138,
    MOVE_POISON_GAS    = 139,
    MOVE_BARRAGE       = 140,
    MOVE_LEECH_LIFE    = 141,
    MOVE_LOVELY_KISS   = 142,
    MOVE_SKY_ATTACK    = 143,
    MOVE_TRANSFORM     = 144,
    MOVE_BUBBLE        = 145,
    MOVE_DIZZY_PUNCH   = 146,
    MOVE_SPORE         = 147,
    MOVE_FLASH         = 148,
    MOVE_PSYWAVE       = 149,
    MOVE_SPLASH        = 150,
    MOVE_ACID_ARMOR    = 151,
    MOVE_CRABHAMMER    = 152,
    MOVE_EXPLOSION     = 153,
    MOVE_FURY_SWIPES   = 154,
    MOVE_BONEMERANG    = 155,
    MOVE_REST          = 156,
    MOVE_ROCK_SLIDE    = 157,
    MOVE_HYPER_FANG    = 158,
    MOVE_SHARPEN       = 159,
    MOVE_CONVERSION    = 160,
    MOVE_TRI_ATTACK    = 161,
    MOVE_SUPER_FANG    = 162,
    MOVE_SLASH         = 163,
    MOVE_SUBSTITUTE    = 164,
    MOVE_STRUGGLE      = 165,
    MOVE_SKETCH        = 166,
    MOVE_TRIPLE_KICK   = 167,
    MOVE_THIEF         = 168,
    MOVE_SPIDER_WEB    = 169,
    MOVE_MIND_READER   = 170,
    MOVE_NIGHTMARE     = 171,
    MOVE_FLAME_WHEEL   = 172,
    MOVE_SNORE         = 173,
    MOVE_CURSE         = 174,
    MOVE_FLAIL         = 175,
    MOVE_CONVERSION_2  = 176,
    MOVE_AEROBLAST     = 177,
    MOVE_COTTON_SPORE  = 178,
    MOVE_REVERSAL      = 179,
    MOVE_SPITE         = 180,
    MOVE_POWDER_SNOW   = 181,
    MOVE_PROTECT       = 182,
    MOVE_MACH_PUNCH    = 183,
    MOVE_SCARY_FACE    = 184,
    MOVE_FAINT_ATTACK  = 185,
    MOVE_SWEET_KISS    = 186,
    MOVE_BELLY_DRUM    = 187,
    MOVE_SLUDGE_BOMB   = 188,
    MOVE_MUD_SLAP      = 189,
    MOVE_OCTAZOOKA     = 190,
    MOVE_SPIKES        = 191,
    MOVE_ZAP_CANNON    = 192,
    MOVE_FORESIGHT     = 193,
    MOVE_DESTINY_BOND  = 194,
    MOVE_PERISH_SONG   = 195,
    MOVE_ICY_WIND      = 196,
    MOVE_DETECT        = 197,
    MOVE_BONE_RUSH     = 198,
    MOVE_LOCK_ON       = 199,
    MOVE_OUTRAGE       = 200,
    MOVE_SANDSTORM     = 201,
    MOVE_GIGA_DRAIN    = 202,
    MOVE_ENDURE        = 203,
    MOVE_CHARM         = 204,
    MOVE_ROLLOUT       = 205,
    MOVE_FALSE_SWIPE   = 206,
    MOVE_SWAGGER       = 207,
    MOVE_MILK_DRINK    = 208,
    MOVE_SPARK         = 209,
    MOVE_FURY_CUTTER   = 210,
    MOVE_STEEL_WING    = 211,
    MOVE_MEAN_LOOK     = 212,
    MOVE_ATTRACT       = 213,
    MOVE_SLEEP_TALK    = 214,
    MOVE_HEAL_BELL     = 215,
    MOVE_RETURN        = 216,
    MOVE_PRESENT       = 217,
    MOVE_FRUSTRATION   = 218,
    MOVE_SAFEGUARD     = 219,
    MOVE_PAIN_SPLIT    = 220,
    MOVE_SACRED_FIRE   = 221,
    MOVE_MAGNITUDE     = 222,
    MOVE_DYNAMIC_PUNCH = 223,
    MOVE_MEGAHORN      = 224,
    MOVE_DRAGON_BREATH = 225,
    MOVE_BATON_PASS    = 226,
    MOVE_ENCORE        = 227,
    MOVE_PURSUIT       = 228,
    MOVE_RAPID_SPIN    = 229,
    MOVE_SWEET_SCENT   = 230,
    MOVE_IRON_TAIL     = 231,
    MOVE_METAL_CLAW    = 232,
    MOVE_VITAL_THROW   = 233,
    MOVE_MORNING_SUN   = 234,
    MOVE_SYNTHESIS     = 235,
    MOVE_MOONLIGHT     = 236,
    MOVE_HIDDEN_POWER  = 237,
    MOVE_CROSS_CHOP    = 238,
    MOVE_TWISTER       = 239,
    MOVE_RAIN_DANCE    = 240,
    MOVE_SUNNY_DAY     = 241,
    MOVE_CRUNCH        = 242,
    MOVE_MIRROR_COAT   = 243,
    MOVE_PSYCH_UP      = 244,
    MOVE_EXTREME_SPEED = 245,
    MOVE_ANCIENT_POWER = 246,
    MOVE_SHADOW_BALL   = 247,
    MOVE_FUTURE_SIGHT  = 248,
    MOVE_ROCK_SMASH    = 249,
    MOVE_WHIRLPOOL     = 250,
    MOVE_BEAT_UP       = 251,
    MOVE_FAKE_OUT      = 252,
    MOVE_UPROAR        = 253,
    MOVE_STOCKPILE     = 254,
    MOVE_SPIT_UP       = 255,
    MOVE_SWALLOW       = 256,
    MOVE_HEAT_WAVE     = 257,
    MOVE_HAIL          = 258,
    MOVE_TORMENT       = 259,
    MOVE_FLATTER       = 260,
    MOVE_WILL_O_WISP   = 261,
    MOVE_MEMENTO       = 262,
    MOVE_FACADE        = 263,
    MOVE_FOCUS_PUNCH   = 264,
    MOVE_SMELLING_SALT = 265,
    MOVE_FOLLOW_ME     = 266,
    MOVE_NATURE_POWER  = 267,
    MOVE_CHARGE        = 268,
    MOVE_TAUNT         = 269,
    MOVE_HELPING_HAND  = 270,
    MOVE_TRICK         = 271,
    MOVE_ROLE_PLAY     = 272,
    MOVE_WISH          = 273,
    MOVE_ASSIST        = 274,
    MOVE_INGRAIN       = 275,
    MOVE_SUPERPOWER    = 276,
    MOVE_MAGIC_COAT    = 277,
    MOVE_RECYCLE       = 278,
    MOVE_REVENGE       = 279,
    MOVE_BRICK_BREAK   = 280,
    MOVE_YAWN          = 281,
    MOVE_KNOCK_OFF     = 282,
    MOVE_ENDEAVOR      = 283,
    MOVE_ERUPTION      = 284,
    MOVE_SKILL_SWAP    = 285,
    MOVE_IMPRISON      = 286,
    MOVE_REFRESH       = 287,
    MOVE_GRUDGE        = 288,
    MOVE_SNATCH        = 289,
    MOVE_SECRET_POWER  = 290,
    MOVE_DIVE          = 291,
    MOVE_ARM_THRUST    = 292,
    MOVE_CAMOUFLAGE    = 293,
    MOVE_TAIL_GLOW     = 294,
    MOVE_LUSTER_PURGE  = 295,
    MOVE_MIST_BALL     = 296,
    MOVE_FEATHER_DANCE = 297,
    MOVE_TEETER_DANCE  = 298,
    MOVE_BLAZE_KICK    = 299,
    MOVE_MUD_SPORT     = 300,
    MOVE_ICE_BALL      = 301,
    MOVE_NEEDLE_ARM    = 302,
    MOVE_SLACK_OFF     = 303,
    MOVE_HYPER_VOICE   = 304,
    MOVE_POISON_FANG   = 305,
    MOVE_CRUSH_CLAW    = 306,
    MOVE_BLAST_BURN    = 307,
    MOVE_HYDRO_CANNON  = 308,
    MOVE_METEOR_MASH   = 309,
    MOVE_ASTONISH      = 310,
    MOVE_WEATHER_BALL  = 311,
    MOVE_AROMATHERAPY  = 312,
    MOVE_FAKE_TEARS    = 313,
    MOVE_AIR_CUTTER    = 314,
    MOVE_OVERHEAT      = 315,
    MOVE_ODOR_SLEUTH   = 316,
    MOVE_ROCK_TOMB     = 317,
    MOVE_SILVER_WIND   = 318,
    MOVE_METAL_SOUND   = 319,
    MOVE_GRASS_WHISTLE = 320,
    MOVE_TICKLE        = 321,
    MOVE_COSMIC_POWER  = 322,
    MOVE_WATER_SPOUT   = 323,
    MOVE_SIGNAL_BEAM   = 324,
    MOVE_SHADOW_PUNCH  = 325,
    MOVE_EXTRASENSORY  = 326,
    MOVE_SKY_UPPERCUT  = 327,
    MOVE_SAND_TOMB     = 328,
    MOVE_SHEER_COLD    = 329,
    MOVE_MUDDY_WATER   = 330,
    MOVE_BULLET_SEED   = 331,
    MOVE_AERIAL_ACE    = 332,
    MOVE_ICICLE_SPEAR  = 333,
    MOVE_IRON_DEFENSE  = 334,
    MOVE_BLOCK         = 335,
    MOVE_HOWL          = 336,
    MOVE_DRAGON_CLAW   = 337,
    MOVE_FRENZY_PLANT  = 338,
    MOVE_BULK_UP       = 339,
    MOVE_BOUNCE        = 340,
    MOVE_MUD_SHOT      = 341,
    MOVE_POISON_TAIL   = 342,
    MOVE_COVET         = 343,
    MOVE_VOLT_TACKLE   = 344,
    MOVE_MAGICAL_LEAF  = 345,
    MOVE_WATER_SPORT   = 346,
    MOVE_CALM_MIND     = 347,
    MOVE_LEAF_BLADE    = 348,
    MOVE_DRAGON_DANCE  = 349,
    MOVE_ROCK_BLAST    = 350,
    MOVE_SHOCK_WAVE    = 351,
    MOVE_WATER_PULSE   = 352,
    MOVE_DOOM_DESIRE   = 353,
    MOVE_PSYCHO_BOOST  = 354,
    MOVE_ROOST         = 355,
    MOVE_GRAVITY       = 356,
    MOVE_MIRACLE_EYE   = 357,
    MOVE_WAKE_UP_SLAP  = 358,
    MOVE_HAMMER_ARM    = 359,
    MOVE_GYRO_BALL     = 360,
    MOVE_HEALING_WISH  = 361,
    MOVE_BRINE         = 362,
    MOVE_NATURAL_GIFT  = 363,
    MOVE_FEINT         = 364,
    MOVE_PLUCK         = 365,
    MOVE_TAILWIND      = 366,
    MOVE_ACUPRESSURE   = 367,
    MOVE_METAL_BURST   = 368,
    MOVE_U_TURN        = 369,
    MOVE_CLOSE_COMBAT  = 370,
    MOVE_PAYBACK       = 371,
    MOVE_ASSURANCE     = 372,
    MOVE_EMBARGO       = 373,
    MOVE_FLING         = 374,
    MOVE_PSYCHO_SHIFT  = 375,
    MOVE_TRUMP_CARD    = 376,
    MOVE_HEAL_BLOCK    = 377,
    MOVE_WRING_OUT     = 378,
    MOVE_POWER_TRICK   = 379,
    MOVE_GASTRO_ACID   = 380,
    MOVE_LUCKY_CHANT   = 381,
    MOVE_ME_FIRST      = 382,
    MOVE_COPYCAT       = 383,
    MOVE_POWER_SWAP    = 384,
    MOVE_GUARD_SWAP    = 385,
    MOVE_PUNISHMENT    = 386,
    MOVE_LAST_RESORT   = 387,
    MOVE_WORRY_SEED    = 388,
    MOVE_SUCKER_PUNCH  = 389,
    MOVE_TOXIC_SPIKES  = 390,
    MOVE_HEART_SWAP    = 391,
    MOVE_AQUA_RING     = 392,
    MOVE_MAGNET_RISE   = 393,
    MOVE_FLARE_BLITZ   = 394,
    MOVE_FORCE_PALM    = 395,
    MOVE_AURA_SPHERE   = 396,
    MOVE_ROCK_POLISH   = 397,
    MOVE_POISON_JAB    = 398,
    MOVE_DARK_PULSE    = 399,
    MOVE_NIGHT_SLASH   = 400,
    MOVE_AQUA_TAIL     = 401,
    MOVE_SEED_BOMB     = 402,
    MOVE_AIR_SLASH     = 403,
    MOVE_X_SCISSOR     = 404,
    MOVE_BUG_BUZZ      = 405,
    MOVE_DRAGON_PULSE  = 406,
    MOVE_DRAGON_RUSH   = 407,
    MOVE_POWER_GEM     = 408,
    MOVE_DRAIN_PUNCH   = 409,
    MOVE_VACUUM_WAVE   = 410,
    MOVE_FOCUS_BLAST   = 411,
    MOVE_ENERGY_BALL   = 412,
    MOVE_BRAVE_BIRD    = 413,
    MOVE_EARTH_POWER   = 414,
    MOVE_SWITCHEROO    = 415,
    MOVE_GIGA_IMPACT   = 416,
    MOVE_NASTY_PLOT    = 417,
    MOVE_BULLET_PUNCH  = 418,
    MOVE_AVALANCHE     = 419,
    MOVE_ICE_SHARD     = 420,
    MOVE_SHADOW_CLAW   = 421,
    MOVE_THUNDER_FANG  = 422,
    MOVE_ICE_FANG      = 423,
    MOVE_FIRE_FANG     = 424,
    MOVE_SHADOW_SNEAK  = 425,
    MOVE_MUD_BOMB      = 426,
    MOVE_PSYCHO_CUT    = 427,
    MOVE_ZEN_HEADBUTT  = 428,
    MOVE_MIRROR_SHOT   = 429,
    MOVE_FLASH_CANNON  = 430,
    MOVE_ROCK_CLIMB    = 431,
    MOVE_DEFOG         = 432,
    MOVE_TRICK_ROOM    = 433,
    MOVE_DRACO_METEOR  = 434,
    MOVE_DISCHARGE     = 435,
    MOVE_LAVA_PLUME    = 436,
    MOVE_LEAF_STORM    = 437,
    MOVE_POWER_WHIP    = 438,
    MOVE_ROCK_WRECKER  = 439,
    MOVE_CROSS_POISON  = 440,
    MOVE_GUNK_SHOT     = 441,
    MOVE_IRON_HEAD     = 442,
    MOVE_MAGNET_BOMB   = 443,
    MOVE_STONE_EDGE    = 444,
    MOVE_CAPTIVATE     = 445,
    MOVE_STEALTH_ROCK  = 446,
    MOVE_GRASS_KNOT    = 447,
    MOVE_CHATTER       = 448,
    MOVE_JUDGMENT      = 449,
    MOVE_BUG_BITE      = 450,
    MOVE_CHARGE_BEAM   = 451,
    MOVE_WOOD_HAMMER   = 452,
    MOVE_AQUA_JET      = 453,
    MOVE_ATTACK_ORDER  = 454,
    MOVE_DEFEND_ORDER  = 455,
    MOVE_HEAL_ORDER    = 456,
    MOVE_HEAD_SMASH    = 457,
    MOVE_DOUBLE_HIT    = 458,
    MOVE_ROAR_OF_TIME  = 459,
    MOVE_SPACIAL_REND  = 460,
    MOVE_LUNAR_DANCE   = 461,
    MOVE_CRUSH_GRIP    = 462,
    MOVE_MAGMA_STORM   = 463,
    MOVE_DARK_VOID     = 464,
    MOVE_SEED_FLARE    = 465,
    MOVE_OMINOUS_WIND  = 466,
    MOVE_SHADOW_FORCE  = 467,
    MAX_MOVES          = 468,
};
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
void Heap_Free(void *ptr);
typedef struct BufferManager {
    void *buffers[2];
    u8 writeBuffer;
    u8 mode;
    u8 padding[2];
} BufferManager;
typedef void (*VBlankDMAFunc)(void *);
typedef struct VBlankDMAController {
    SysTask *onVBlankTask;
    SysTask *postVBlankTask;
    BOOL doDMA;
    BOOL swapBuffers;
    VBlankDMAFunc dmaFunc;
    VBlankDMAFunc swapBufferFunc;
    void *param;
} VBlankDMAController;
struct CustomBgScrollContext {
    VBlankDMAController dmaController;
    BufferManager *bufferManager;
    u32 buffer1[192 ];
    u32 buffer2[192 ];
    u32 offsetReg;
};
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;
inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID)
{
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0)
{
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail()) ;
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1 );
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1)
{
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}
typedef struct BattleBgSwitch {
    int unread_00;
    u8 unused_04;
    u8 state;
    u8 unused_06;
    u8 unused_07;
    u8 unused_08;
    u8 blendCoeffA;
    u8 blendCoeffB;
    u8 targetBlendCoeffA;
    u8 targetBlendCoeffB;
    u8 fadeType;
    int bgID;
    int mode;
    int flags;
    int scriptVars[10 ];
    u16 bgMoveAnimActive : 1;
    u16 bgWaveAnimActive : 1;
    u16 unused_44_2 : 14;
    BattleAnimSystem *battleAnimSystem;
} BattleBgSwitch;
void ov07_0221BE44(BattleAnimSystem *system);
void BattleBgSwitch_SetBg(BattleBgSwitch *bgSwitch, BattleAnimSystem *system, enum BgLayer bgLayer, int bgID);
void BattleAnimSystem_LoadBaseBg(BattleAnimSystem *system, enum BgLayer bgLayer);
void BattleAnimSystem_UnloadBaseBg(BattleAnimSystem *system, enum BgLayer bgLayer);
enum BgLayer BattleAnimSystem_GetBgLayer(BattleAnimSystem *system, int bg);
void BattleAnimSystem_LoadBattleBgTiles(BattleAnimSystem *system, enum BgLayer layer);
BattleMonOBJData *ov07_0221FDFC(BattleSystem *battleSys, enum HeapID heapID);

BOOL ov07_0221C01C(BattleAnimSystem *system, MoveAnimation *moveAnim, u16 move, BattleAnimBattlerContext *battlerContext);

BOOL ov07_0221C3DC(BattleAnimSystem *system);
typedef struct {
    int a[25];
} ov07_02234C64_Array;
extern const ov07_02234C64_Array ov07_02234C64;
BOOL ov07_0221C01C(BattleAnimSystem *system, MoveAnimation *moveAnim, u16 move, BattleAnimBattlerContext *battlerContext)
{
    int i;

    ov07_0221C69C();

    if (ov07_0221C3DC(system) == 0 ) {
        return 0 ;
    }

    for (i = 0; (s32)i < 16 ; i++) {
        system->cameraProjections[i] = CAMERA_PROJECTION_ORTHOGRAPHIC;
        system->particleSystemCameraFlip[i] = 0;
    }

    for (i = 0; i < 10 ; i++) {
        system->scriptVars[i] = 0;
    }

    for (i = 0; i < 3 ; i++) {
        system->loopStack[i].isActive = 0 ;
        system->loopStack[i].startPos = 0L ;
        system->loopStack[i].curIteration = 0;
        system->loopStack[i].maxIterations = 0;
    }

    system->context->command = moveAnim->command;
    system->context->unk_01 = moveAnim->unk_01;
    system->context->move = moveAnim->move;
    system->context->damage = moveAnim->damage;
    system->context->power = moveAnim->power;
    system->context->friendship = moveAnim->friendship;
    system->context->fieldConditions = moveAnim->fieldConditions;
    system->context->effectChance = moveAnim->effectChance;
    system->context->terrain = moveAnim->terrain;
    system->context->attacker = moveAnim->attacker;
    system->context->defender = moveAnim->defender;
    system->context->isTransformed = moveAnim->isTransformed;
    system->context->unk_118_1 = moveAnim->unk_0E_2;
    system->context->unk_118_2 = moveAnim->unk_0E_3;
    system->context->spriteSystem = battlerContext->spriteSystem;

    ((battlerContext->spriteSystem != 0L) ? (void)0 : GF_AssertFail()) ;

    system->bgConfig = battlerContext->bgConfig;
    system->paletteData = battlerContext->paletteData;
    system->context->battleType = battlerContext->battleType;

    for (i = 0; i < 4 ; i++) {
        system->context->pokemonSpriteData[i] = battlerContext->pokemonSpriteData[i];
        system->context->battlerTypes[i] = battlerContext->battlerTypes[i];
        system->context->battlerSprites[i] = battlerContext->pokemonSprites[i];
        system->context->battlerSpecies[i] = battlerContext->battlerSpecies[i];
        system->context->battlerGenders[i] = battlerContext->battlerGenders[i];
        system->context->battlerShinyFlags[i] = battlerContext->battlerShinyFlags[i];
        system->context->battlerForms[i] = battlerContext->battlerForms[i];
        system->context->battlerPersonalities[i] = battlerContext->battlerPersonalities[i];
        system->context->battlerMoveEffects[i] = battlerContext->battlerMoveEffects[i];
    }

    system->battleBgRefs = battlerContext->battleBgRef;
    system->context->chatotCry = battlerContext->chatotCry;
    system->context->bgPaletteBuffer = battlerContext->bgPaletteBuffer;
    system->context->bgTiles = battlerContext->bgTiles;

    int moveID = move;
    if (moveID == MOVE_SECRET_POWER) {
        ov07_02234C64_Array terrainMoveTable = ov07_02234C64;

        if (system->context->terrain >= TERRAIN_MAX) {
            moveID = MOVE_TRI_ATTACK;
        } else {
            moveID = terrainMoveTable.a[system->context->terrain];
        }
    }

    if (moveID == MOVE_NONE || moveID > (MAX_MOVES - 1) ) {
        moveID = MOVE_POUND;
    }

    system->moveArcID = (enum NarcID)(battlerContext->moveArcID);
    system->scriptData = AllocAndReadWholeNarcMemberByIdPair(system->moveArcID, moveID, system->heapID);

    if (system->scriptData == 0L ) {
        ((system->scriptData != 0L) ? (void)0 : GF_AssertFail()) ;
        return 0 ;
    }

    system->scriptPtr = (u32 *)system->scriptData;
    *(u8 *)((u8 *)system + 0x1AC) = GetBgPriority(system->bgConfig, 0U);
    *(u8 *)((u8 *)system + 0x1AD) = GetBgPriority(system->bgConfig, 1U);
    *(u8 *)((u8 *)system + 0x1AE) = GetBgPriority(system->bgConfig, BG_LAYER_MAIN_2);
    system->bgLayerPriorities[BG_LAYER_MAIN_3] = GetBgPriority(system->bgConfig, BG_LAYER_MAIN_3);

    for (i = 0; i < 10 ; i++) {
        system->sprites[i] = 0L ;
    }

    for (i = 0; i < 5 ; i++) {
        system->pokemonSprites[i] = 0L ;
        system->usedPokemonSprites[i] = 0 ;
    }

    for (i = 0; i < 4; i++) {
        system->bgAnims[i] = NULL;
    }
    system->executeAnimScriptFunc = ov07_0221BE44;
    system->scriptDelay = 0;

    system->baseBgPalettes = 0xFF;

    system->moveActive = 1 ;

    return 1 ;
}
