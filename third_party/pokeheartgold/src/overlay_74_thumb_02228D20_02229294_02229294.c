#include "global.h"
#include "overlay_74_app_templates.h"
#include "overlay_manager.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "font.h"
#include "save.h"
#include "options.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "player_data.h"
#include "message_format.h"
#include "overlay_74_main_menu_window.h"
#include "pokedex.h"
#include "render_window.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BallSeal;

struct BattleRegulation;

struct BgConfig;

struct MainMenuWindow;

struct MysteryGift;

struct MysteryGiftCosmeticsData;

struct UnkWonderCardSubstruct_104;

struct MysteryGiftItemData;

struct MysteryGiftPokemonData;

struct Options;

struct PGT;

struct Pokedex;

struct Pokemon;

struct RangerLinkManager;

struct SaveData;

struct Sprite;

struct SysTask;

struct SysTaskManager;

struct PlayerProfile;

struct WaitingIcon;

struct Window;

struct WonderCard;

enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};

u8 TextPrinterCheckActive(u8 printerID);

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
    PGT pgt;
    UnkWonderCardSubstruct_104 eventHeader;
    u16 description[250];
    u8 sharesLeft;
    u8 padding_349;
    u16 spritesSpecies[3];
    u8 timesShared;
    u8 padding_351[3];
    s32 receivedDate;
} WonderCard;

typedef struct MysteryGift MysteryGift;

BOOL SaveMysteryGift_FindAvailable(MysteryGift *mysteryGift);

typedef struct SysTaskManager SysTaskManager;

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

void ov74_02235390(BOOL enable);

void ov74_0223539C(enum FadeType fadeType, int destState, int *state, int waitState);

void ov74_022353FC(int *state);

int ov74_02235568(BgConfig *param0, MainMenuWindow *window, int xPos, int yPos, int textEntryID);

void ov74_022358BC(void);

void ov74_0222EC08(BgConfig *bgConfig, WonderCard *wonderCard, enum HeapID heapID);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

BOOL Pokedex_IsEnabled(const Pokedex *pokedex);

enum RangerLinkMainState {
    RANGER_LINK_STATE_INIT = 0,
    RANGER_LINK_STATE_DEX_REQUIRED_ALERT,
    RANGER_LINK_STATE_CHECK_PGT_SPACE,
    RANGER_LINK_STATE_WAIT_EXIT_DELIVERY_MAN_ALERT,
    RANGER_LINK_STATE_HANDLE_WIRELESS,
    RANGER_LINK_STATE_SHOW_WONDERCARD,
    RANGER_LINK_STATE_WAIT_PLAYER_EXIT_WONDERCARD,
    RANGER_LINK_STATE_UNUSED_7,
    RANGER_LINK_STATE_UNUSED_8,
    RANGER_LINK_STATE_UNUSED_9,
    RANGER_LINK_STATE_UNUSED_10,
    RANGER_LINK_STATE_RESET_SYSTEM,
    RANGER_LINK_STATE_EXIT,
    RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION,
    RANGER_LINK_STATE_UNUSED_14
};

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

enum RangerLinkRangerCommStatus {
    RANGER_COMM_STATUS_CONTINUE = 0,
    RANGER_COMM_STATUS_AWAITING_RANGER,
    RANGER_COMM_STATUS_UNUSED_2,
    RANGER_COMM_STATUS_UNUSED_3,
    RANGER_COMM_STATUS_TRANSFER_COMPLETED,
    RANGER_COMM_STATUS_CONNECTION_CANCELED,
};

typedef struct RangerLinkManager {
    enum HeapID heapID;
    BgConfig *bgConfig;
    SaveData *saveData;
    Pokedex *pokedex;
    PlayerProfile *trainerInfo;
    Options *options;
    Window stdWindow;
    Window messageBoxWindow;
    Window appNumberStdWindow;
    MainMenuWindow messageBox;
    MainMenuWindow manaphyEggTransferWindow;
    MainMenuWindow playerCommsDetails;
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
    OverlayManager *appMan;
    Sprite *downloadArrow;
    MysteryGift *mysteryGift;
    int unused_3178;
    int deliveryManOverloadedPrinterID;
    WonderCard manaphyEggWonderCard;
    WaitingIcon *waitDial;
} RangerLinkManager;

void ov74_02228E98(RangerLinkManager *param0);

enum RangerLinkRangerCommStatus ov74_0222962C(RangerLinkManager *param0);

void ov74_02228D64(RangerLinkManager *param0);

BOOL ov74_02228F14(RangerLinkManager *param0);

void ov74_02228F8C(RangerLinkManager *param0);

void ov74_02229190(OverlayManager *appMan);

BOOL ov74_02229294(OverlayManager *appMan, int *state) {
    RangerLinkManager *rangerLinkMan = OverlayManager_GetData(appMan);

    switch (*state) {
    case RANGER_LINK_STATE_INIT:
        ov74_02228D64(rangerLinkMan);

        if (Pokedex_IsEnabled(rangerLinkMan->pokedex) == 0) {
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_IN, RANGER_LINK_STATE_DEX_REQUIRED_ALERT, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        } else {
            *state = RANGER_LINK_STATE_CHECK_PGT_SPACE;
        }
        break;
    case RANGER_LINK_STATE_DEX_REQUIRED_ALERT:
        if (ov74_02228F14(rangerLinkMan) == 0) {
            ov74_02235390(1);
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_OUT, RANGER_LINK_STATE_EXIT, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        }
        break;
    case RANGER_LINK_STATE_CHECK_PGT_SPACE:
        ov74_02228F8C(rangerLinkMan);

        if (SaveMysteryGift_FindAvailable(rangerLinkMan->mysteryGift) == 0) {
            rangerLinkMan->messageBox.renderDelay = 1;
            rangerLinkMan->deliveryManOverloadedPrinterID = ov74_02235568(rangerLinkMan->bgConfig, &rangerLinkMan->messageBox, 2, 19, 22);
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_IN, RANGER_LINK_STATE_WAIT_EXIT_DELIVERY_MAN_ALERT, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        } else {
            rangerLinkMan->unused_14C = 7;
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_IN, RANGER_LINK_STATE_HANDLE_WIRELESS, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        }
        break;
    case RANGER_LINK_STATE_WAIT_EXIT_DELIVERY_MAN_ALERT:
        if ((TextPrinterCheckActive(rangerLinkMan->deliveryManOverloadedPrinterID) == 0) && (gSystem.newKeys & (0x0001))) {
            ov74_02235390(1);
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_OUT, RANGER_LINK_STATE_EXIT, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        }
        break;
    case RANGER_LINK_STATE_HANDLE_WIRELESS:
        rangerLinkMan->wirelessStatus = ov74_0222962C(rangerLinkMan);

        if (rangerLinkMan->wirelessStatus == RANGER_COMM_STATUS_CONNECTION_CANCELED) {
            ov74_02235390(1);
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_OUT, RANGER_LINK_STATE_EXIT, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        } else if (rangerLinkMan->wirelessStatus == RANGER_COMM_STATUS_TRANSFER_COMPLETED) {
            ov74_02235390(1);
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_OUT, RANGER_LINK_STATE_EXIT, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        }
        break;
    case RANGER_LINK_STATE_SHOW_WONDERCARD:
        ov74_02228E98(rangerLinkMan);

        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 0);

        ov74_0222EC08(rangerLinkMan->bgConfig, &rangerLinkMan->manaphyEggWonderCard, (enum HeapID)83);
        ov74_0223539C(FADE_TYPE_BRIGHTNESS_IN, RANGER_LINK_STATE_WAIT_PLAYER_EXIT_WONDERCARD, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        rangerLinkMan->manaphyEggWonderCard.eventHeader.hasWonderCard = 0;
        break;
    case RANGER_LINK_STATE_WAIT_PLAYER_EXIT_WONDERCARD:
        if (gSystem.newKeys) {
            ov74_0223539C(FADE_TYPE_BRIGHTNESS_OUT, RANGER_LINK_STATE_RESET_SYSTEM, state, RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION);
        }
        break;
    case RANGER_LINK_STATE_RESET_SYSTEM:
        OS_ResetSystem(0);
        break;
    case RANGER_LINK_STATE_EXIT:
        ov74_02229190(appMan);
        return 1;
        break;
    case RANGER_LINK_STATE_WAIT_SCREEN_TRANSITION:
        ov74_022353FC(state);
        break;
    }

    ov74_022358BC();

    return 0;
}
