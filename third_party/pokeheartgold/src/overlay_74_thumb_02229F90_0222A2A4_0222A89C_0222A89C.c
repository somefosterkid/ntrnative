#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "msgdata.h"
#include "struct_defs/resdat_narc_structs.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BallSeal;

struct BattleRegulation;

struct BgConfig;

struct ListMenuCursor;

struct DWCstInetControl;

struct G2dRenderer;

struct ListMenu;

struct ListMenuTemplate;

struct MsgData;

struct MysteryGiftAppData;

struct MysteryGiftCosmeticsData;

struct MysteryGiftEventData;

struct UnkWonderCardSubstruct_104;

struct MysteryGiftItemData;

struct MysteryGiftPokemonData;

struct Options;

struct PGT;

struct Pokemon;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct StateTransitionMenuOptionTemplate;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct Window;

struct WonderCard;

struct tagDWCWDSData;

typedef enum {
    DWC_WDS_STATE_IDLE = 0,
    DWC_WDS_STATE_PROCESS = 1,
    DWC_WDS_STATE_COMPLETED = 2,
    DWC_WDS_STATE_FAILED = 3,
    DWC_WDS_STATE_ERROR = 4
} DWCWDSState;

typedef struct tagDWCWDSData {
    u8 ssid[32];
    u8 wep[32];
    int wepMode;
    char apnum[10];
} DWCWDSData;

typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;

} DWCInetControl;

typedef enum {
    DWC_ETYPE_NO_ERROR = 0,
    DWC_ETYPE_LIGHT,
    DWC_ETYPE_SHOW_ERROR,
    DWC_ETYPE_SHUTDOWN_FM,
    DWC_ETYPE_SHUTDOWN_GHTTP,
    DWC_ETYPE_SHUTDOWN_ND,
    DWC_ETYPE_DISCONNECT,
    DWC_ETYPE_FATAL,
    DWC_ETYPE_NUM
} DWCErrorType;

typedef struct ListMenuCursor ListMenuCursor;

void DestroyMsgData(MsgData *loader);

typedef struct ListMenuItem ListMenuItem;

ListMenuItem *ListMenuItems_New(u32 capacity, u32 heapID);

void ListMenuItems_Delete(ListMenuItem *list);

void ListMenuItems_AppendFromMsgData(ListMenuItem *list, const MsgData *loader, u32 bankEntry, u32 index);

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const ListMenuItem *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;

struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

ListMenu *ListMenuInit(const ListMenuTemplate *template, u16 startListPos, u16 startCursorPos, enum HeapID heapID);

void DestroyListMenu(ListMenu *menu, u16 *outListPos, u16 *outCursorPos);

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

typedef struct UnkWonderCardSubstruct_104 {
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
} UnkWonderCardSubstruct_104;

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

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

void *OverlayManager_GetData(OverlayManager *appMan);

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

    u32 animData[29];
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

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

enum MysteryGiftAppState {
    MG_APP_STATE_SETUP = 0,
    MG_APP_STATE_INIT_GRAPHICS,
    MG_APP_STATE_WAIT_SCREEN_TRANSITION,
    MG_APP_STATE_WAIT_MAIN_MENU_INPUT,
    MG_APP_STATE_WAIT_RECEPTION_METHOD_CHOICE,
    MG_APP_STATE_WAIT_CONFIRM_WIRELESS_COMMS,
    MG_APP_STATE_UNUSED_6,

    MG_APP_STATE_SEARCH_FOR_FRIEND_GIFT,
    MG_APP_STATE_WAIT_CONFIRM_RECEIVE_FRIEND_GIFT,
    MG_APP_STATE_CONNECT_WITH_FRIEND,
    MG_APP_STATE_RECEIVING_FROM_FRIEND,
    MG_APP_STATE_SAVE_GIFT_FROM_FRIEND,
    MG_APP_STATE_CHECK_FRIEND_CONNECTION_STILL_VALID,
    MG_APP_STATE_WAIT_FRIEND_READY_TO_DISCONNECT,
    MG_APP_STATE_PERSON_SHARING_DISCONNECTED,
    MG_APP_STATE_RECEIVE_FROM_FRIEND_SUCCESS,
    MG_APP_STATE_FRIEND_LEFT_BEFORE_SENDING,

    MG_APP_STATE_NO_GIFT_FOUND,
    MG_APP_STATE_RETURN_TO_MENU,
    MG_APP_STATE_FOUND_WIFI_OR_GBA_GIFT,
    MG_APP_STATE_WAIT_ACCEPT_WIFI_OR_GBA_GIFT,
    MG_APP_STATE_GOTO_SHOW_RECEIVING_GIFT_MSG,
    MG_APP_STATE_SHOW_RECEIVING_GIFT_MSG,
    MG_APP_STATE_SAVE_RECEIVED_MYSTERY_GIFT,
    MG_APP_STATE_WAIT_MYSTERY_GIFT_SAVED,
    MG_APP_STATE_WAIT_BEFORE_EXIT_AFTER_RECEIVING_GIFT,
    MG_APP_STATE_UNUSED_26,
    MG_APP_STATE_SEARCH_FOR_WIFI_EVENT,

    MG_APP_STATE_UNUSED_28,
    MG_APP_STATE_UNUSED_29,
    MG_APP_STATE_UNUSED_30,

    MG_APP_STATE_WAIT_CONFIRM_RECEIVE_LOCAL_WIRELESS_GIFT,
    MG_APP_STATE_SEARCH_FOR_LOCAL_WIRELESS_EVENT,
    MG_APP_STATE_RECEIVE_FROM_LOCAL_WIRELESS,
    MG_APP_STATE_WAIT_LOCAL_WIRELESS_GIFT_SAVED,
    MG_APP_STATE_WAIT_FOR_ANIMATION_FINISHED,
    MG_APP_STATE_DISCONNECT_FROM_LOCAL_WIRELESS,

    MG_APP_STATE_UNUSED_37,
    MG_APP_STATE_UNUSED_38,
    MG_APP_STATE_UNUSED_39,
    MG_APP_STATE_UNUSED_40,
    MG_APP_STATE_UNUSED_41,
    MG_APP_STATE_UNUSED_42,
    MG_APP_STATE_UNUSED_43,
    MG_APP_STATE_UNUSED_44,
    MG_APP_STATE_UNUSED_45,
    MG_APP_STATE_UNUSED_46,
    MG_APP_STATE_UNUSED_47,
    MG_APP_STATE_UNUSED_48,

    MG_APP_STATE_FROM_FRIEND_OR_CANT_RECEIVE,
    MG_APP_STATE_WAIT_MESSAGE_BOX_FOR_TRANSITION,
    MG_APP_STATE_WAIT_INPUT_RETURN_TO_MENU,
    MG_APP_STATE_ASK_CONFIRM_RECEIVE_SHARED_GIFT,
    MG_APP_STATE_EXIT_TO_TITLE_SCREEN,
    MG_APP_STATE_EXIT_TO_WONDER_CARDS_APP,
    MG_APP_STATE_EXIT_AFTER_RECEIVING_GIFT,
    MG_APP_STATE_SHOW_RECEIVED_WONDERCARD,
    MG_APP_STATE_RESET_SYSTEM,

    MG_APP_KEEP_PREVIOUS_STATE = -1,
};

enum MysteryGiftAnimationStatus {
    MG_ANIMATION_STATUS_DONE = 0,
    MG_ANIMATION_STATUS_BEGIN,
    MG_ANIMATION_STATUS_PROCEED_IMPLOSION
};

typedef struct MysteryGiftEventData {
    UnkWonderCardSubstruct_104 header;
    WonderCard wonderCard;
} MysteryGiftEventData;

typedef struct MysteryGiftAppData MysteryGiftAppData;

struct MysteryGiftAppData {
    BgConfig *bgConfig;
    SaveData *saveData;
    Options *options;
    MessageFormat *strTemplate;
    MsgData *msgLoader;
    String *string;
    Window messageBox;
    Window menuWindows[3];
    Window wonderCardTitleWindow;
    int msgBoxPrinterDelay;
    int textPrinterId;
    int unused_70;
    u32 queuedState;
    ListMenu *listMenu;
    ListMenuItem *listMenuOptions;
    int canReceiveGiftStatus;
    int foundGiftType;
    int unused_88;
    struct { u8 header[80]; struct { u16 tag; u16 flag; u8 data[256]; struct { u16 name[36]; u32 version; u16 id; u8 unique : 1; u8 unk8E_1 : 1; u8 unk8E_2 : 1; u8 unk8E_3 : 1; u8 unk8E_4 : 1; u8 unk8E_5 : 1; u8 unk8E_6 : 2; } unk104; u16 text[250]; u8 shareMax; u8 unk_34A_pad[1]; u16 monIcon[3]; u8 shareCount; u8 unk_354_pad[3]; s32 receiveDate; } wonderCard; } eventData;
    int wirelessCommsTimeout;
    int unk_438;
    int delay;
    BOOL exitToWondercardsApp;
    SpriteList *unusedSpriteList;
    G2dRenderer unusedG2dRenderer;
    SpriteResourceCollection *unusedSpriteResourceCollections[GF_GFX_RES_TYPE_MAX];
    SpriteResource *unusedSpriteResources[GF_GFX_RES_TYPE_MAX];
    SpriteResourcesHeader unusedSpriteResourcesHeader;
    Sprite *dowloadArrowSprite;
    int unk_62C;
    int unk_630;
    int unk_634;
    u8 wirelessDistributionBuffer[4096];
    int receptionMethod;
    enum MysteryGiftAppState *statePtr;
    int giftSearchResultDelay;
    int nasAuthState;
    u8 *unused_1648;
    DWCInetControl unk_164C;
    BOOL unused_16AC;
    int unk_16B0;
    int unk_16B4;
    s8 wifiDistributionBuffer[4096];
    int unused_26B8;
    u32 unk_26BC;
    u32 unk_26C0;
    u32 unk_26C4;
    BOOL wifiCommErrored;
    int wifiCommErrorStringID;
    int wifiCommErrorCode;
    DWCErrorType wifiCommErrorType;
    BOOL wifiExitRequested;
    void *dwcCallback;
    Window wifiCommErrorWindow;
    struct { u16 tag; u16 flag; u8 data[256]; struct { u16 name[36]; u32 version; u16 id; u8 unique : 1; u8 unk8E_1 : 1; u8 unk8E_2 : 1; u8 unk8E_3 : 1; u8 unk8E_4 : 1; u8 unk8E_5 : 1; u8 unk8E_6 : 2; } unk104; u16 text[250]; u8 shareMax; u8 unk_34A_pad[1]; u16 monIcon[3]; u8 shareCount; u8 unk_354_pad[3]; s32 receiveDate; } wonderCard;
    void *mainCallback;
    BOOL cancelSave;
    void *waitDial;
    enum MysteryGiftAnimationStatus animationStatus;
};

void ov74_02235FF8(ListMenu *param0, u32 param1, u8 param2);

typedef enum MysteryGiftAppState (*StateTransitionFuncPtr)(OverlayManager *);

typedef struct StateTransitionMenuOptionTemplate {
    int textEntryId;
    StateTransitionFuncPtr stateTransitionFuncPtr;
} StateTransitionMenuOptionTemplate;

void ov74_0222AA18(OverlayManager *appMan, Window *window, u32 entryId);

extern ListMenuTemplate ov74_0223C270;

enum MysteryGiftAppState ov74_02229F90(OverlayManager *appMan);

enum MysteryGiftAppState ov74_02229FF4(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A2A4(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A2E0(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A35C(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A3CC(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A494(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A4CC(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A504(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A560(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A5AC(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A5CC(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A650(OverlayManager *appMan);

enum MysteryGiftAppState ov74_0222A6C0(OverlayManager *appMan);

void ProcessStateTransitionMenuInput(OverlayManager *appMan, enum MysteryGiftAppState *state, StateTransitionFuncPtr onCancel);

void ov74_0222AC1C(OverlayManager *appMan, enum MysteryGiftAppState *state);

int ov74_0222ADBC(OverlayManager *appMan, Window *window, int textEntryId, enum MysteryGiftAppState nextState);

BOOL ov74_0222C2EC(OverlayManager *appMan, enum MysteryGiftAppState *state);

void ov74_0222A89C(OverlayManager *appMan, StateTransitionMenuOptionTemplate *entries, int numEntries, Window *window, u32 msgBoxEntryId);

void ov74_0222A89C(OverlayManager *appMan, StateTransitionMenuOptionTemplate *entries, int numEntries, Window *window, u32 msgBoxEntryId) {
    MysteryGiftAppData *appData = OverlayManager_GetData(appMan);

    if (appData->listMenuOptions) {
        ListMenuItems_Delete(appData->listMenuOptions);
    }

    appData->listMenuOptions = ListMenuItems_New(numEntries, HEAP_ID_84);
    appData->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (enum NarcId)27, 247, HEAP_ID_84);

    for (int i = 0; i < numEntries; i++) {
        ListMenuItems_AppendFromMsgData(appData->listMenuOptions, appData->msgLoader, entries[i].textEntryId, (u32)entries[i].stateTransitionFuncPtr);
    }

    DestroyMsgData(appData->msgLoader);

    ListMenuTemplate listMenuTemplate = ov74_0223C270;

    listMenuTemplate.choices = appData->listMenuOptions;
    listMenuTemplate.count = listMenuTemplate.maxDisplay = numEntries;
    listMenuTemplate.window = window;

    if (appData->listMenu) {
        DestroyListMenu(appData->listMenu, NULL, NULL);
    }

    appData->listMenu = ListMenuInit(&listMenuTemplate, 0, 0, HEAP_ID_84);

    if (msgBoxEntryId != -1) {
        ov74_0222AA18(appMan, &appData->messageBox, msgBoxEntryId);
    }
}
