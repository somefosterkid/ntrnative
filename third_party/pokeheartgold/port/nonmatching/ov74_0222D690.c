#include "global.h"
#include "bg_window.h"
struct ApplicationManager;
struct ApplicationManagerTemplate;
struct BallCapsule;
struct BallSeal;
struct BattleRegulation;
struct BgConfig;
struct BoxPokemon;
struct BufferManager;
struct ColoredArrow;
struct DWCstInetControl;
struct EasyChatSentence;
struct G2dRenderer;
struct ListMenu;
struct ListMenuTemplate;
struct Mail;
struct MessageLoader;
struct MysteryGift;
struct MysteryGiftAppData;
struct MysteryGiftCosmeticsData;
struct MysteryGiftEventData;
struct MysteryGiftEventHeader;
struct MysteryGiftItemData;
struct MysteryGiftPokemonData;
struct Options;
struct PGT;
struct PartyPokemon;
struct Pokemon;
struct PokemonDataBlockA;
struct PokemonDataBlockB;
struct PokemonDataBlockC;
struct PokemonDataBlockD;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct SpriteResourcesHeader;
struct StateTransitionMenuOptionTemplate;
struct String;
struct StringList;
struct StringTemplate;
struct SysTask;
struct SysTaskManager;
struct TrainerInfo;
struct WaitDial;
struct Window;
struct WonderCard;
struct WonderCardFlipAnimManager;
struct WonderCardsAppData;
struct WonderCardsAppWindowTemplate;
struct tagDWCWDSData;
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
enum Item {
    ITEM_NONE         =   0,
    ITEM_MASTER_BALL  =   1,
    ITEM_ULTRA_BALL   =   2,
    ITEM_GREAT_BALL   =   3,
    ITEM_POKE_BALL    =   4,
    ITEM_SAFARI_BALL  =   5,
    ITEM_NET_BALL     =   6,
    ITEM_DIVE_BALL    =   7,
    ITEM_NEST_BALL    =   8,
    ITEM_REPEAT_BALL  =   9,
    ITEM_TIMER_BALL   =  10,
    ITEM_LUXURY_BALL  =  11,
    ITEM_PREMIER_BALL =  12,
    ITEM_DUSK_BALL    =  13,
    ITEM_HEAL_BALL    =  14,
    ITEM_QUICK_BALL   =  15,
    ITEM_CHERISH_BALL =  16,
    ITEM_POTION       =  17,
    ITEM_ANTIDOTE     =  18,
    ITEM_BURN_HEAL    =  19,
    ITEM_ICE_HEAL     =  20,
    ITEM_AWAKENING    =  21,
    ITEM_PARLYZ_HEAL  =  22,
    ITEM_FULL_RESTORE =  23,
    ITEM_MAX_POTION   =  24,
    ITEM_HYPER_POTION =  25,
    ITEM_SUPER_POTION =  26,
    ITEM_FULL_HEAL    =  27,
    ITEM_REVIVE       =  28,
    ITEM_MAX_REVIVE   =  29,
    ITEM_FRESH_WATER  =  30,
    ITEM_SODA_POP     =  31,
    ITEM_LEMONADE     =  32,
    ITEM_MOOMOO_MILK  =  33,
    ITEM_ENERGYPOWDER =  34,
    ITEM_ENERGY_ROOT  =  35,
    ITEM_HEAL_POWDER  =  36,
    ITEM_REVIVAL_HERB =  37,
    ITEM_ETHER        =  38,
    ITEM_MAX_ETHER    =  39,
    ITEM_ELIXIR       =  40,
    ITEM_MAX_ELIXIR   =  41,
    ITEM_LAVA_COOKIE  =  42,
    ITEM_BERRY_JUICE  =  43,
    ITEM_SACRED_ASH   =  44,
    ITEM_HP_UP        =  45,
    ITEM_PROTEIN      =  46,
    ITEM_IRON         =  47,
    ITEM_CARBOS       =  48,
    ITEM_CALCIUM      =  49,
    ITEM_RARE_CANDY   =  50,
    ITEM_PP_UP        =  51,
    ITEM_ZINC         =  52,
    ITEM_PP_MAX       =  53,
    ITEM_OLD_GATEAU   =  54,
    ITEM_GUARD_SPEC   =  55,
    ITEM_DIRE_HIT     =  56,
    ITEM_X_ATTACK     =  57,
    ITEM_X_DEFENSE    =  58,
    ITEM_X_SPEED      =  59,
    ITEM_X_ACCURACY   =  60,
    ITEM_X_SPECIAL    =  61,
    ITEM_X_SP_DEF     =  62,
    ITEM_POKE_DOLL    =  63,
    ITEM_FLUFFY_TAIL  =  64,
    ITEM_BLUE_FLUTE   =  65,
    ITEM_YELLOW_FLUTE =  66,
    ITEM_RED_FLUTE    =  67,
    ITEM_BLACK_FLUTE  =  68,
    ITEM_WHITE_FLUTE  =  69,
    ITEM_SHOAL_SALT   =  70,
    ITEM_SHOAL_SHELL  =  71,
    ITEM_RED_SHARD    =  72,
    ITEM_BLUE_SHARD   =  73,
    ITEM_YELLOW_SHARD =  74,
    ITEM_GREEN_SHARD  =  75,
    ITEM_SUPER_REPEL  =  76,
    ITEM_MAX_REPEL    =  77,
    ITEM_ESCAPE_ROPE  =  78,
    ITEM_REPEL        =  79,
    ITEM_SUN_STONE    =  80,
    ITEM_MOON_STONE   =  81,
    ITEM_FIRE_STONE   =  82,
    ITEM_THUNDERSTONE =  83,
    ITEM_WATER_STONE  =  84,
    ITEM_LEAF_STONE   =  85,
    ITEM_TINYMUSHROOM =  86,
    ITEM_BIG_MUSHROOM =  87,
    ITEM_PEARL        =  88,
    ITEM_BIG_PEARL    =  89,
    ITEM_STARDUST     =  90,
    ITEM_STAR_PIECE   =  91,
    ITEM_NUGGET       =  92,
    ITEM_HEART_SCALE  =  93,
    ITEM_HONEY        =  94,
    ITEM_GROWTH_MULCH =  95,
    ITEM_DAMP_MULCH   =  96,
    ITEM_STABLE_MULCH =  97,
    ITEM_GOOEY_MULCH  =  98,
    ITEM_ROOT_FOSSIL  =  99,
    ITEM_CLAW_FOSSIL  = 100,
    ITEM_HELIX_FOSSIL = 101,
    ITEM_DOME_FOSSIL  = 102,
    ITEM_OLD_AMBER    = 103,
    ITEM_ARMOR_FOSSIL = 104,
    ITEM_SKULL_FOSSIL = 105,
    ITEM_RARE_BONE    = 106,
    ITEM_SHINY_STONE  = 107,
    ITEM_DUSK_STONE   = 108,
    ITEM_DAWN_STONE   = 109,
    ITEM_OVAL_STONE   = 110,
    ITEM_ODD_KEYSTONE = 111,
    ITEM_GRISEOUS_ORB = 112,
    ITEM_UNUSED_113   = 113,
    ITEM_UNUSED_114   = 114,
    ITEM_UNUSED_115   = 115,
    ITEM_UNUSED_116   = 116,
    ITEM_UNUSED_117   = 117,
    ITEM_UNUSED_118   = 118,
    ITEM_UNUSED_119   = 119,
    ITEM_UNUSED_120   = 120,
    ITEM_UNUSED_121   = 121,
    ITEM_UNUSED_122   = 122,
    ITEM_UNUSED_123   = 123,
    ITEM_UNUSED_124   = 124,
    ITEM_UNUSED_125   = 125,
    ITEM_UNUSED_126   = 126,
    ITEM_UNUSED_127   = 127,
    ITEM_UNUSED_128   = 128,
    ITEM_UNUSED_129   = 129,
    ITEM_UNUSED_130   = 130,
    ITEM_UNUSED_131   = 131,
    ITEM_UNUSED_132   = 132,
    ITEM_UNUSED_133   = 133,
    ITEM_UNUSED_134   = 134,
    ITEM_ADAMANT_ORB  = 135,
    ITEM_LUSTROUS_ORB = 136,
    ITEM_GRASS_MAIL   = 137,
    ITEM_FLAME_MAIL   = 138,
    ITEM_BUBBLE_MAIL  = 139,
    ITEM_BLOOM_MAIL   = 140,
    ITEM_TUNNEL_MAIL  = 141,
    ITEM_STEEL_MAIL   = 142,
    ITEM_HEART_MAIL   = 143,
    ITEM_SNOW_MAIL    = 144,
    ITEM_SPACE_MAIL   = 145,
    ITEM_AIR_MAIL     = 146,
    ITEM_MOSAIC_MAIL  = 147,
    ITEM_BRICK_MAIL   = 148,
    ITEM_CHERI_BERRY  = 149,
    ITEM_CHESTO_BERRY = 150,
    ITEM_PECHA_BERRY  = 151,
    ITEM_RAWST_BERRY  = 152,
    ITEM_ASPEAR_BERRY = 153,
    ITEM_LEPPA_BERRY  = 154,
    ITEM_ORAN_BERRY   = 155,
    ITEM_PERSIM_BERRY = 156,
    ITEM_LUM_BERRY    = 157,
    ITEM_SITRUS_BERRY = 158,
    ITEM_FIGY_BERRY   = 159,
    ITEM_WIKI_BERRY   = 160,
    ITEM_MAGO_BERRY   = 161,
    ITEM_AGUAV_BERRY  = 162,
    ITEM_IAPAPA_BERRY = 163,
    ITEM_RAZZ_BERRY   = 164,
    ITEM_BLUK_BERRY   = 165,
    ITEM_NANAB_BERRY  = 166,
    ITEM_WEPEAR_BERRY = 167,
    ITEM_PINAP_BERRY  = 168,
    ITEM_POMEG_BERRY  = 169,
    ITEM_KELPSY_BERRY = 170,
    ITEM_QUALOT_BERRY = 171,
    ITEM_HONDEW_BERRY = 172,
    ITEM_GREPA_BERRY  = 173,
    ITEM_TAMATO_BERRY = 174,
    ITEM_CORNN_BERRY  = 175,
    ITEM_MAGOST_BERRY = 176,
    ITEM_RABUTA_BERRY = 177,
    ITEM_NOMEL_BERRY  = 178,
    ITEM_SPELON_BERRY = 179,
    ITEM_PAMTRE_BERRY = 180,
    ITEM_WATMEL_BERRY = 181,
    ITEM_DURIN_BERRY  = 182,
    ITEM_BELUE_BERRY  = 183,
    ITEM_OCCA_BERRY   = 184,
    ITEM_PASSHO_BERRY = 185,
    ITEM_WACAN_BERRY  = 186,
    ITEM_RINDO_BERRY  = 187,
    ITEM_YACHE_BERRY  = 188,
    ITEM_CHOPLE_BERRY = 189,
    ITEM_KEBIA_BERRY  = 190,
    ITEM_SHUCA_BERRY  = 191,
    ITEM_COBA_BERRY   = 192,
    ITEM_PAYAPA_BERRY = 193,
    ITEM_TANGA_BERRY  = 194,
    ITEM_CHARTI_BERRY = 195,
    ITEM_KASIB_BERRY  = 196,
    ITEM_HABAN_BERRY  = 197,
    ITEM_COLBUR_BERRY = 198,
    ITEM_BABIRI_BERRY = 199,
    ITEM_CHILAN_BERRY = 200,
    ITEM_LIECHI_BERRY = 201,
    ITEM_GANLON_BERRY = 202,
    ITEM_SALAC_BERRY  = 203,
    ITEM_PETAYA_BERRY = 204,
    ITEM_APICOT_BERRY = 205,
    ITEM_LANSAT_BERRY = 206,
    ITEM_STARF_BERRY  = 207,
    ITEM_ENIGMA_BERRY = 208,
    ITEM_MICLE_BERRY  = 209,
    ITEM_CUSTAP_BERRY = 210,
    ITEM_JABOCA_BERRY = 211,
    ITEM_ROWAP_BERRY  = 212,
    ITEM_BRIGHTPOWDER = 213,
    ITEM_WHITE_HERB   = 214,
    ITEM_MACHO_BRACE  = 215,
    ITEM_EXP_SHARE    = 216,
    ITEM_QUICK_CLAW   = 217,
    ITEM_SOOTHE_BELL  = 218,
    ITEM_MENTAL_HERB  = 219,
    ITEM_CHOICE_BAND  = 220,
    ITEM_KINGS_ROCK   = 221,
    ITEM_SILVERPOWDER = 222,
    ITEM_AMULET_COIN  = 223,
    ITEM_CLEANSE_TAG  = 224,
    ITEM_SOUL_DEW     = 225,
    ITEM_DEEPSEATOOTH = 226,
    ITEM_DEEPSEASCALE = 227,
    ITEM_SMOKE_BALL   = 228,
    ITEM_EVERSTONE    = 229,
    ITEM_FOCUS_BAND   = 230,
    ITEM_LUCKY_EGG    = 231,
    ITEM_SCOPE_LENS   = 232,
    ITEM_METAL_COAT   = 233,
    ITEM_LEFTOVERS    = 234,
    ITEM_DRAGON_SCALE = 235,
    ITEM_LIGHT_BALL   = 236,
    ITEM_SOFT_SAND    = 237,
    ITEM_HARD_STONE   = 238,
    ITEM_MIRACLE_SEED = 239,
    ITEM_BLACKGLASSES = 240,
    ITEM_BLACK_BELT   = 241,
    ITEM_MAGNET       = 242,
    ITEM_MYSTIC_WATER = 243,
    ITEM_SHARP_BEAK   = 244,
    ITEM_POISON_BARB  = 245,
    ITEM_NEVERMELTICE = 246,
    ITEM_SPELL_TAG    = 247,
    ITEM_TWISTEDSPOON = 248,
    ITEM_CHARCOAL     = 249,
    ITEM_DRAGON_FANG  = 250,
    ITEM_SILK_SCARF   = 251,
    ITEM_UPGRADE      = 252,
    ITEM_SHELL_BELL   = 253,
    ITEM_SEA_INCENSE  = 254,
    ITEM_LAX_INCENSE  = 255,
    ITEM_LUCKY_PUNCH  = 256,
    ITEM_METAL_POWDER = 257,
    ITEM_THICK_CLUB   = 258,
    ITEM_STICK        = 259,
    ITEM_RED_SCARF    = 260,
    ITEM_BLUE_SCARF   = 261,
    ITEM_PINK_SCARF   = 262,
    ITEM_GREEN_SCARF  = 263,
    ITEM_YELLOW_SCARF = 264,
    ITEM_WIDE_LENS    = 265,
    ITEM_MUSCLE_BAND  = 266,
    ITEM_WISE_GLASSES = 267,
    ITEM_EXPERT_BELT  = 268,
    ITEM_LIGHT_CLAY   = 269,
    ITEM_LIFE_ORB     = 270,
    ITEM_POWER_HERB   = 271,
    ITEM_TOXIC_ORB    = 272,
    ITEM_FLAME_ORB    = 273,
    ITEM_QUICK_POWDER = 274,
    ITEM_FOCUS_SASH   = 275,
    ITEM_ZOOM_LENS    = 276,
    ITEM_METRONOME    = 277,
    ITEM_IRON_BALL    = 278,
    ITEM_LAGGING_TAIL = 279,
    ITEM_DESTINY_KNOT = 280,
    ITEM_BLACK_SLUDGE = 281,
    ITEM_ICY_ROCK     = 282,
    ITEM_SMOOTH_ROCK  = 283,
    ITEM_HEAT_ROCK    = 284,
    ITEM_DAMP_ROCK    = 285,
    ITEM_GRIP_CLAW    = 286,
    ITEM_CHOICE_SCARF = 287,
    ITEM_STICKY_BARB  = 288,
    ITEM_POWER_BRACER = 289,
    ITEM_POWER_BELT   = 290,
    ITEM_POWER_LENS   = 291,
    ITEM_POWER_BAND   = 292,
    ITEM_POWER_ANKLET = 293,
    ITEM_POWER_WEIGHT = 294,
    ITEM_SHED_SHELL   = 295,
    ITEM_BIG_ROOT     = 296,
    ITEM_CHOICE_SPECS = 297,
    ITEM_FLAME_PLATE  = 298,
    ITEM_SPLASH_PLATE = 299,
    ITEM_ZAP_PLATE    = 300,
    ITEM_MEADOW_PLATE = 301,
    ITEM_ICICLE_PLATE = 302,
    ITEM_FIST_PLATE   = 303,
    ITEM_TOXIC_PLATE  = 304,
    ITEM_EARTH_PLATE  = 305,
    ITEM_SKY_PLATE    = 306,
    ITEM_MIND_PLATE   = 307,
    ITEM_INSECT_PLATE = 308,
    ITEM_STONE_PLATE  = 309,
    ITEM_SPOOKY_PLATE = 310,
    ITEM_DRACO_PLATE  = 311,
    ITEM_DREAD_PLATE  = 312,
    ITEM_IRON_PLATE   = 313,
    ITEM_ODD_INCENSE  = 314,
    ITEM_ROCK_INCENSE = 315,
    ITEM_FULL_INCENSE = 316,
    ITEM_WAVE_INCENSE = 317,
    ITEM_ROSE_INCENSE = 318,
    ITEM_LUCK_INCENSE = 319,
    ITEM_PURE_INCENSE = 320,
    ITEM_PROTECTOR    = 321,
    ITEM_ELECTIRIZER  = 322,
    ITEM_MAGMARIZER   = 323,
    ITEM_DUBIOUS_DISC = 324,
    ITEM_REAPER_CLOTH = 325,
    ITEM_RAZOR_CLAW   = 326,
    ITEM_RAZOR_FANG   = 327,
    ITEM_TM01         = 328,
    ITEM_TM02         = 329,
    ITEM_TM03         = 330,
    ITEM_TM04         = 331,
    ITEM_TM05         = 332,
    ITEM_TM06         = 333,
    ITEM_TM07         = 334,
    ITEM_TM08         = 335,
    ITEM_TM09         = 336,
    ITEM_TM10         = 337,
    ITEM_TM11         = 338,
    ITEM_TM12         = 339,
    ITEM_TM13         = 340,
    ITEM_TM14         = 341,
    ITEM_TM15         = 342,
    ITEM_TM16         = 343,
    ITEM_TM17         = 344,
    ITEM_TM18         = 345,
    ITEM_TM19         = 346,
    ITEM_TM20         = 347,
    ITEM_TM21         = 348,
    ITEM_TM22         = 349,
    ITEM_TM23         = 350,
    ITEM_TM24         = 351,
    ITEM_TM25         = 352,
    ITEM_TM26         = 353,
    ITEM_TM27         = 354,
    ITEM_TM28         = 355,
    ITEM_TM29         = 356,
    ITEM_TM30         = 357,
    ITEM_TM31         = 358,
    ITEM_TM32         = 359,
    ITEM_TM33         = 360,
    ITEM_TM34         = 361,
    ITEM_TM35         = 362,
    ITEM_TM36         = 363,
    ITEM_TM37         = 364,
    ITEM_TM38         = 365,
    ITEM_TM39         = 366,
    ITEM_TM40         = 367,
    ITEM_TM41         = 368,
    ITEM_TM42         = 369,
    ITEM_TM43         = 370,
    ITEM_TM44         = 371,
    ITEM_TM45         = 372,
    ITEM_TM46         = 373,
    ITEM_TM47         = 374,
    ITEM_TM48         = 375,
    ITEM_TM49         = 376,
    ITEM_TM50         = 377,
    ITEM_TM51         = 378,
    ITEM_TM52         = 379,
    ITEM_TM53         = 380,
    ITEM_TM54         = 381,
    ITEM_TM55         = 382,
    ITEM_TM56         = 383,
    ITEM_TM57         = 384,
    ITEM_TM58         = 385,
    ITEM_TM59         = 386,
    ITEM_TM60         = 387,
    ITEM_TM61         = 388,
    ITEM_TM62         = 389,
    ITEM_TM63         = 390,
    ITEM_TM64         = 391,
    ITEM_TM65         = 392,
    ITEM_TM66         = 393,
    ITEM_TM67         = 394,
    ITEM_TM68         = 395,
    ITEM_TM69         = 396,
    ITEM_TM70         = 397,
    ITEM_TM71         = 398,
    ITEM_TM72         = 399,
    ITEM_TM73         = 400,
    ITEM_TM74         = 401,
    ITEM_TM75         = 402,
    ITEM_TM76         = 403,
    ITEM_TM77         = 404,
    ITEM_TM78         = 405,
    ITEM_TM79         = 406,
    ITEM_TM80         = 407,
    ITEM_TM81         = 408,
    ITEM_TM82         = 409,
    ITEM_TM83         = 410,
    ITEM_TM84         = 411,
    ITEM_TM85         = 412,
    ITEM_TM86         = 413,
    ITEM_TM87         = 414,
    ITEM_TM88         = 415,
    ITEM_TM89         = 416,
    ITEM_TM90         = 417,
    ITEM_TM91         = 418,
    ITEM_TM92         = 419,
    ITEM_HM01         = 420,
    ITEM_HM02         = 421,
    ITEM_HM03         = 422,
    ITEM_HM04         = 423,
    ITEM_HM05         = 424,
    ITEM_HM06         = 425,
    ITEM_HM07         = 426,
    ITEM_HM08         = 427,
    ITEM_EXPLORER_KIT = 428,
    ITEM_LOOT_SACK    = 429,
    ITEM_RULE_BOOK    = 430,
    ITEM_POKE_RADAR   = 431,
    ITEM_POINT_CARD   = 432,
    ITEM_JOURNAL      = 433,
    ITEM_SEAL_CASE    = 434,
    ITEM_FASHION_CASE = 435,
    ITEM_SEAL_BAG     = 436,
    ITEM_PAL_PAD      = 437,
    ITEM_WORKS_KEY    = 438,
    ITEM_OLD_CHARM    = 439,
    ITEM_GALACTIC_KEY = 440,
    ITEM_RED_CHAIN    = 441,
    ITEM_TOWN_MAP     = 442,
    ITEM_VS_SEEKER    = 443,
    ITEM_COIN_CASE    = 444,
    ITEM_OLD_ROD      = 445,
    ITEM_GOOD_ROD     = 446,
    ITEM_SUPER_ROD    = 447,
    ITEM_SPRAYDUCK    = 448,
    ITEM_POFFIN_CASE  = 449,
    ITEM_BICYCLE      = 450,
    ITEM_SUITE_KEY    = 451,
    ITEM_OAKS_LETTER  = 452,
    ITEM_LUNAR_WING   = 453,
    ITEM_MEMBER_CARD  = 454,
    ITEM_AZURE_FLUTE  = 455,
    ITEM_S_S_TICKET   = 456,
    ITEM_CONTEST_PASS = 457,
    ITEM_MAGMA_STONE  = 458,
    ITEM_PARCEL       = 459,
    ITEM_COUPON_1     = 460,
    ITEM_COUPON_2     = 461,
    ITEM_COUPON_3     = 462,
    ITEM_STORAGE_KEY  = 463,
    ITEM_SECRETPOTION = 464,
    ITEM_VS_RECORDER  = 465,
    ITEM_GRACIDEA     = 466,
    ITEM_SECRET_KEY   = 467,
    MAX_ITEMS         = 468,
};
enum PoketchAppID {
    POKETCH_APPID_DIGITALWATCH      =  0,
    POKETCH_APPID_CALCULATOR        =  1,
    POKETCH_APPID_MEMOPAD           =  2,
    POKETCH_APPID_PEDOMETER         =  3,
    POKETCH_APPID_PARTYSTATUS       =  4,
    POKETCH_APPID_FRIENDSHIPCHECKER =  5,
    POKETCH_APPID_DOWSINGMACHINE    =  6,
    POKETCH_APPID_BERRYSEARCHER     =  7,
    POKETCH_APPID_DAYCARECHECKER    =  8,
    POKETCH_APPID_POKEMONHISTORY    =  9,
    POKETCH_APPID_COUNTER           = 10,
    POKETCH_APPID_ANALOGWATCH       = 11,
    POKETCH_APPID_MARKINGMAP        = 12,
    POKETCH_APPID_LINKSEARCHER      = 13,
    POKETCH_APPID_COINTOSS          = 14,
    POKETCH_APPID_MOVETESTER        = 15,
    POKETCH_APPID_CALENDAR          = 16,
    POKETCH_APPID_DOTART            = 17,
    POKETCH_APPID_ROULETTE          = 18,
    POKETCH_APPID_TRAINERCOUNTER    = 19,
    POKETCH_APPID_KITCHENTIMER      = 20,
    POKETCH_APPID_COLORCHANGER      = 21,
    POKETCH_APPID_MATCHUPCHECKER    = 22,
    POKETCH_APPID_STOPWATCH         = 23,
    POKETCH_APPID_ALARMCLOCK        = 24,
    POKETCH_APPID_MAX               = 25,
    POKETCH_APPID_NONE              = -1,
};
typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;
typedef struct BallCapsule {
    BallSeal seals[8 ];
} BallCapsule;
typedef u16 charcode_t;
typedef struct String String;
typedef struct EasyChatSentence {
    u16 type;
    u16 id;
    u16 words[2 ];
} EasyChatSentence;
typedef struct SaveData SaveData;
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
    MailIconData iconData[3 ];
    u16 platExclusiveFormIcons;
    EasyChatSentence sentences[3 ];
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
    u16 moves[4 ];
    u8 moveCurrentPPs[4 ];
    u8 movePPUps[4 ];

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
    u8 sLotoId;
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
    u16 unused;
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
    PokemonDataBlock dataBlocks[4];
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
    u16 name[12 ];
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
    u16 title[36 ];
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
    PGT pgt;
    MysteryGiftEventHeader eventHeader;
    u16 description[250 ];
    u8 sharesLeft;
    u8 padding_349;
    u16 spritesSpecies[3 ];
    u8 timesShared;
    u8 padding_351[3];
    s32 receivedDate;
} WonderCard;
typedef struct MysteryGift MysteryGift;
typedef enum {
    DWC_WDS_STATE_IDLE         = 0,
    DWC_WDS_STATE_PROCESS      = 1,
    DWC_WDS_STATE_COMPLETED    = 2,
    DWC_WDS_STATE_FAILED       = 3,
    DWC_WDS_STATE_ERROR        = 4
} DWCWDSState;
typedef struct tagDWCWDSData {
    u8 ssid[32 ];
    u8 wep[32 ];
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
typedef struct Options Options;
enum FontAttribute {
    FONTATTR_MAX_LETTER_WIDTH = 0,
    FONTATTR_MAX_LETTER_HEIGHT,
    FONTATTR_LETTER_SPACING,
    FONTATTR_LINE_SPACING,
    FONTATTR_DUMMY,
    FONTATTR_FG_COLOR,
    FONTATTR_BG_COLOR,
    FONTATTR_SHADOW_COLOR,
};
typedef u32 TextColor;
typedef struct ColoredArrow ColoredArrow;
typedef struct MessageLoader MessageLoader;
typedef struct StringList StringList;
typedef struct ListMenu ListMenu;
typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);
typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);
typedef struct ListMenuTemplate {
    const StringList *choices;
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
    struct {
        u8 textColorFg : 4;
        u8 textColorBg : 4;
        u8 textColorShadow : 4;
        u8 letterSpacing : 6;
        u8 dummy : 6;
        u8 fontID : 7;
        u8 prefer : 1;
    } altFont;
    ColoredArrow *cursor;
    u16 listPos;
    u16 cursorPos;
    u8 dummy2C;
    u8 dummy2D;
    u8 dummy2E;
    u8 lastAction;
    u8 heapID;
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
enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};
typedef struct SpriteResource SpriteResource;
typedef struct SpriteResourceCollection SpriteResourceCollection;
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct SysTaskManager SysTaskManager;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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
typedef struct TrainerInfo TrainerInfo;
typedef struct StringTemplate StringTemplate;
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
    MysteryGiftEventHeader header;
    WonderCard wonderCard;
} MysteryGiftEventData;
typedef struct MysteryGiftAppData MysteryGiftAppData;
typedef void (*MysteryGiftAppMainCallbackFuncPtr)(MysteryGiftAppData *);
typedef BOOL (*DWCCallbackFuncPtr)(void);
struct MysteryGiftAppData {
    BgConfig *bgConfig;
    SaveData *saveData;
    Options *options;
    StringTemplate *strTemplate;
    MessageLoader *msgLoader;
    String *string;
    Window messageBox;
    Window menuWindows[3 ];
    Window wonderCardTitleWindow;
    int msgBoxPrinterDelay;
    int textPrinterId;
    int unused_70;
    enum MysteryGiftAppState queuedState;
    ListMenu *listMenu;
    StringList *listMenuOptions;
    int canReceiveGiftStatus;
    int foundGiftType;
    int unused_88;
    MysteryGiftEventData eventData;
    int wirelessCommsTimeout;
    int unk_438;
    int delay;
    BOOL exitToWondercardsApp;
    SpriteList *unusedSpriteList;
    G2dRenderer unusedG2dRenderer;
    SpriteResourceCollection *unusedSpriteResourceCollections[SPRITE_RESOURCE_MAX];
    SpriteResource *unusedSpriteResources[SPRITE_RESOURCE_MAX];
    SpriteResourcesHeader unusedSpriteResourcesHeader;
    Sprite *dowloadArrowSprite;
    int unk_62C;
    int unk_630;
    int unk_634;
    u8 wirelessDistributionBuffer[4096 ];
    int receptionMethod;
    enum MysteryGiftAppState *statePtr;
    int giftSearchResultDelay;
    int nasAuthState;
    u8 *unused_1648;
    DWCInetControl unk_164C;
    BOOL unused_16AC;
    int unk_16B0;
    int unk_16B4;
    char wifiDistributionBuffer[4096 ];
    int unused_26B8;
    u32 unk_26BC;
    u32 unk_26C0;
    u32 unk_26C4;
    BOOL wifiCommErrored;
    int wifiCommErrorStringID;
    int wifiCommErrorCode;
    DWCErrorType wifiCommErrorType;
    BOOL wifiExitRequested;
    DWCCallbackFuncPtr dwcCallback;
    Window wifiCommErrorWindow;
    WonderCard wonderCard;
    MysteryGiftAppMainCallbackFuncPtr mainCallback;
    BOOL cancelSave;
    void *waitDial;
    enum MysteryGiftAnimationStatus animationStatus;
};
typedef struct BufferManager BufferManager;
typedef struct WaitDial WaitDial;
enum WonderCardsAppWindow {
    WC_FRONT_TEXT_WONDER_CARD = 0,
    WC_FRONT_TEXT_DATE_RECEIVED,
    WC_FRONT_GIFT_STATUS,
    WC_FRONT_WONDERCARD_TITLE,
    WC_FRONT_RECEIVED_DATE,
    WC_BACK_DESCRIPTION,
    WC_BACK_SHARES_LEFT,
    MSG_BOX_TEXT_WONDER_CARD,
    MSG_BOX_WC_ACTIONS_MENU,
    MSG_BOX_CONFIRM_WC_DELETE,
    MSG_BOX_CONFIRM_WC_SHARE,
    WC_SHARING_TEXT_PAL_DISTRIBUTION_REGISTRATION,
    WC_SHARING_TEXT_CURRENT_ENTRIES,
    WC_SHARING_TEXT_SEND,
    WC_SHARING_TEXT_CANCEL,
    WC_SHARING_ENTRIES_COUNT,
    WC_SHARING_PLAYER_INFO,
    MSG_BOX_DISTRIBUTION_UNDER_WAY,
    MSG_BOX_DISTRIBUTION_SUCCESSFUL,
    NUM_WC_APP_WINDOWS,
};
enum WonderCardsAppState {
    WC_APP_STATE_LOAD_FROM_SAVE = 0,
    WC_APP_STATE_INIT_GRAPHICS,
    WC_APP_STATE_LOAD_GRAPHICS,
    WC_APP_STATE_SELECT_WONDERCARD,
    WC_APP_STATE_SHOW_WONDERCARD_ACTIONS,
    WC_APP_STATE_WAIT_FOR_MENU_CHOICE,
    WC_APP_STATE_START_FLIP_WC_TO_BACK,
    WC_APP_STATE_WAIT_WC_FLIP_TO_BACK_HALFWAY,
    WC_APP_STATE_SHOW_WONDERCARD_BACK,
    WC_APP_STATE_START_FLIP_WC_TO_FRONT,
    WC_APP_STATE_WAIT_WC_FLIP_TO_FRONT_HALFWAY,
    WC_APP_STATE_CLOSE_WINDOWS,
    WC_APP_STATE_WAIT_CONFIRM_DELETION,
    WC_APP_STATE_WAIT_CONFIRM_START_WIRELESS,
    WC_APP_STATE_ASK_START_WIRELESS_TO_SHARE_WC,
    WC_APP_STATE_START_TRANSITION_TO_WC_SHARE_SCREEN,
    WC_APP_STATE_SHOW_WC_SHARE_SCREEN,
    WC_APP_STATE_PREPARE_FOR_SHARING,
    WC_APP_STATE_WAIT_FOR_PLAYERS,
    WC_APP_STATE_RETURN_AFTER_COMM_MAN_EXIT,
    WC_APP_STATE_RETURN_TO_WC_ACTIONS,
    WC_APP_STATE_DISTRIBUTION_UNDERWAY,
    WC_APP_STATE_WAIT_FOR_COMM_MAN_EXIT,
    WC_APP_STATE_START_COMM_SYNC,
    WC_APP_STATE_WAIT_FOR_COMM_SYNC,
    WC_APP_STATE_DISTRIBUTION_SUCCESSFUL,
    WC_APP_STATE_NO_MORE_WONDERCARDS,
    WC_APP_STATE_WAIT_FOR_SCREEN_TRANSITION,
    WC_APP_STATE_EXIT,
    WC_APP_STATE_UNUSED,
    NUM_WC_APP_STATES,
};
enum WonderCardsAppScreen {
    WC_SCREEN_WONDERCARD_FRONT = 0,
    WC_SCREEN_WONDERCARD_BACK,
    WC_MESSAGE_BOX,
    WC_SCREEN_WONDERCARD_SHARING,
};
enum WonderCardAction {
    WC_ACTION_INFO = 0,
    WC_ACTION_SHARE,
    WC_ACTION_TRASH,
    WC_ACTION_EXIT,
    NUM_WC_ACTIONS,
};
enum WonderCardFlipStage {
    WC_FLIP_STAGE_GROWING = 0,
    WC_FLIP_STAGE_SHRINKING,
};
typedef struct WonderCardsAppData WonderCardsAppData;
typedef BOOL (*WcAppWindowSetupFuncPtr)(WonderCardsAppData *, Window *, TextColor);
typedef enum WonderCardsAppState (*StateTransitionFuncPtr)(ApplicationManager *);
typedef void (*WcAppMainCallbackFuncPtr)(WonderCardsAppData *);
typedef struct WonderCardFlipAnimManager {
    u16 scrollsBuffers[2][192 * 4];
    BufferManager *bufferManager;
    SysTask *sysTask;
    fx32 offsetStep;
    fx32 deltaOffsetStep;
    BOOL running;
    BOOL animStage;
} WonderCardFlipAnimManager;
typedef struct WonderCardsAppWindowTemplate {
    enum WonderCardsAppScreen screen;
    int tilemapLeft;
    int tilemapTop;
    int width;
    int height;
    int font;
    int textColor;
    union {
        enum FontAttribute fontAttribute;
        int colorIndex;
    } bgColor;
    u32 entryID;
    WcAppWindowSetupFuncPtr setup;
    int leftMarginSize;
    int topMarginSize;
} WonderCardsAppWindowTemplate;
typedef struct StateTransitionMenuOptionTemplate {
    int textEntryID;
    union {
        enum WonderCardsAppState targetState;
        StateTransitionFuncPtr transitionFunc;
        u32 asU32;
    } stateChange;
} StateTransitionMenuOptionTemplate;
struct WonderCardsAppData {
    enum HeapID heapID;
    MysteryGiftAppData mysteryGiftAppData;
    BgConfig *bgConfig;
    StringTemplate *strTemplate;
    MessageLoader *msgLoader;


    Window windows[NUM_WC_APP_WINDOWS + 1];
    int windowBasetiles[NUM_WC_APP_WINDOWS + 1];

    int messageBoxBaseTile;
    void *messageBoxTemplate;
    MysteryGift *mysteryGift;
    SaveData *saveData;
    Options *options;
    int msgBoxFrame;
    int unused_2C10;
    WonderCard *wonderCards[3 ];
    int selectedWondercardSlot;
    int currentScreen;
    ListMenu *listMenu;
    StringList *strList;
    Window messageBox;
    Window standardWindow;
    int nextWindowBasetile;
    int connectedPlayersCount;
    u32 unused_2C58[4];





    TrainerInfo *connTrainerInfos[1 + 4 ];
    int orderNumbers[1 + 4 ];

    int prevConnPlayersCount;
    int shouldSendWc;
    void (*unused_2C98)(void *);
    int numPlayerConnections;
    int queuedState;
    SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    SpriteResourceCollection *spriteResourceCollections[SPRITE_RESOURCE_MAX];
    SpriteResource *spriteResources[SPRITE_RESOURCE_MAX];
    SpriteResourcesHeader spriteResourceHeader;
    Sprite *shareScreenBtnSprites[2];
    Sprite *selectedWcSprites[3 ];
    WonderCard currentWonderCard;
    WonderCardFlipAnimManager flipAnimMan;
    WcAppMainCallbackFuncPtr mainCallback;
    int shareScreenSelectedBtn;
    WaitDial *waitDial;
};
enum WonderCardsAppState ov74_0222D308(ApplicationManager *appMan);
enum WonderCardsAppState AskConfirmShareWc(ApplicationManager *appMan);
extern StateTransitionMenuOptionTemplate ov74_0223C2D0[]; /* HGSS: the wonder card action entries are ov74_0223C2D0[6..9] */
void ov74_0222A89C(WonderCardsAppData *appData, StateTransitionMenuOptionTemplate *entries, int numEntries, Window *window, int startCursorPos);
enum WonderCardsAppState ov74_0222D358(ApplicationManager *appMan);
enum WonderCardsAppState ReturnToWcActionsMenu(ApplicationManager *appMan);
void ShowWindowsForScreen(WonderCardsAppData *appData, BOOL unused, enum WonderCardsAppScreen screen);
void LoadWondercardGraphics(WonderCardsAppData *appData, enum WonderCardsAppScreen screen);
void StartFlipAnim(WonderCardsAppData *appData, enum WonderCardFlipStage stage, fx32 startOffsetStep, fx32 startDOfsStep);
int WonderCardsApp_Main(ApplicationManager *appMan, enum WonderCardsAppState *state);
void ov74_0222EC08(BgConfig *bgConfig, WonderCard *wonderCard, enum HeapID heapID);

BOOL ov74_0222D690(WonderCardsAppData *appData, Window *window, TextColor unused);
void ov74_0222D11C(WonderCardsAppData *appData, StateTransitionMenuOptionTemplate *entries, int numEntries, Window *window, int startCursorPos);
BOOL ov74_0222D690(WonderCardsAppData *appData, Window *window, TextColor unused)
{
  StateTransitionMenuOptionTemplate entries[NUM_WC_ACTIONS];
  WonderCardsAppWindowTemplate *windowTemplate;
  int numOptions = 0;
  entries[numOptions++] = ov74_0223C2D0[6];
  if (appData->wonderCards[appData->selectedWondercardSlot]->sharesLeft)
  {
    entries[numOptions++] = ov74_0223C2D0[7];
  }
  entries[numOptions++] = ov74_0223C2D0[8];
  entries[numOptions++] = ov74_0223C2D0[9];
  windowTemplate = appData->messageBoxTemplate;
  ClearWindowTilemapAndCopyToVram(window);
  RemoveWindow(window);
  AddWindowParameterized(appData->bgConfig, window, BG_LAYER_MAIN_2, windowTemplate->tilemapLeft, windowTemplate->tilemapTop + ((NUM_WC_ACTIONS - numOptions) * ((16 + 7) / 8)), windowTemplate->width, windowTemplate->height - ((NUM_WC_ACTIONS - numOptions) * ((16 + 7) / 8)), 15, appData->messageBoxBaseTile);
  ov74_0222D11C(appData, entries, numOptions, window, 0);
  return 1;
}

