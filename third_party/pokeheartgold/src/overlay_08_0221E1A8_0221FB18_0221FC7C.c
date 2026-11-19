#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "msgdata.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"
#include "message_printer.h"
#include "item.h"

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSubMenuCursor;

struct BattleSystem;

struct BgConfig;


struct MessagePrinter;

struct GridMenuCursorPosition;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct PaletteData;

struct SelectedPaletteData;

struct Party;

struct Pokemon;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct Window;

enum MoveAttribute {
    MOVEATTRIBUTE_EFFECT = 0,
    MOVEATTRIBUTE_CLASS = 1,
    MOVEATTRIBUTE_POWER = 2,
    MOVEATTRIBUTE_TYPE = 3,
    MOVEATTRIBUTE_ACCURACY = 4,
    MOVEATTRIBUTE_PP = 5,
    MOVEATTRIBUTE_EFFECT_CHANCE = 6,
    MOVEATTRIBUTE_RANGE = 7,
    MOVEATTRIBUTE_PRIORTY = 8,
    MOVEATTRIBUTE_FLAGS = 9,
    MOVEATTRIBUTE_CONTEST_EFFECT = 10,
    MOVEATTRIBUTE_CONTEST_TYPE = 11,
};

enum Move {
    MOVE_NONE = 0,
    MOVE_POUND = 1,
    MOVE_KARATE_CHOP = 2,
    MOVE_DOUBLE_SLAP = 3,
    MOVE_COMET_PUNCH = 4,
    MOVE_MEGA_PUNCH = 5,
    MOVE_PAY_DAY = 6,
    MOVE_FIRE_PUNCH = 7,
    MOVE_ICE_PUNCH = 8,
    MOVE_THUNDER_PUNCH = 9,
    MOVE_SCRATCH = 10,
    MOVE_VICE_GRIP = 11,
    MOVE_GUILLOTINE = 12,
    MOVE_RAZOR_WIND = 13,
    MOVE_SWORDS_DANCE = 14,
    MOVE_CUT = 15,
    MOVE_GUST = 16,
    MOVE_WING_ATTACK = 17,
    MOVE_WHIRLWIND = 18,
    MOVE_FLY = 19,
    MOVE_BIND = 20,
    MOVE_SLAM = 21,
    MOVE_VINE_WHIP = 22,
    MOVE_STOMP = 23,
    MOVE_DOUBLE_KICK = 24,
    MOVE_MEGA_KICK = 25,
    MOVE_JUMP_KICK = 26,
    MOVE_ROLLING_KICK = 27,
    MOVE_SAND_ATTACK = 28,
    MOVE_HEADBUTT = 29,
    MOVE_HORN_ATTACK = 30,
    MOVE_FURY_ATTACK = 31,
    MOVE_HORN_DRILL = 32,
    MOVE_TACKLE = 33,
    MOVE_BODY_SLAM = 34,
    MOVE_WRAP = 35,
    MOVE_TAKE_DOWN = 36,
    MOVE_THRASH = 37,
    MOVE_DOUBLE_EDGE = 38,
    MOVE_TAIL_WHIP = 39,
    MOVE_POISON_STING = 40,
    MOVE_TWINEEDLE = 41,
    MOVE_PIN_MISSILE = 42,
    MOVE_LEER = 43,
    MOVE_BITE = 44,
    MOVE_GROWL = 45,
    MOVE_ROAR = 46,
    MOVE_SING = 47,
    MOVE_SUPERSONIC = 48,
    MOVE_SONIC_BOOM = 49,
    MOVE_DISABLE = 50,
    MOVE_ACID = 51,
    MOVE_EMBER = 52,
    MOVE_FLAMETHROWER = 53,
    MOVE_MIST = 54,
    MOVE_WATER_GUN = 55,
    MOVE_HYDRO_PUMP = 56,
    MOVE_SURF = 57,
    MOVE_ICE_BEAM = 58,
    MOVE_BLIZZARD = 59,
    MOVE_PSYBEAM = 60,
    MOVE_BUBBLE_BEAM = 61,
    MOVE_AURORA_BEAM = 62,
    MOVE_HYPER_BEAM = 63,
    MOVE_PECK = 64,
    MOVE_DRILL_PECK = 65,
    MOVE_SUBMISSION = 66,
    MOVE_LOW_KICK = 67,
    MOVE_COUNTER = 68,
    MOVE_SEISMIC_TOSS = 69,
    MOVE_STRENGTH = 70,
    MOVE_ABSORB = 71,
    MOVE_MEGA_DRAIN = 72,
    MOVE_LEECH_SEED = 73,
    MOVE_GROWTH = 74,
    MOVE_RAZOR_LEAF = 75,
    MOVE_SOLAR_BEAM = 76,
    MOVE_POISON_POWDER = 77,
    MOVE_STUN_SPORE = 78,
    MOVE_SLEEP_POWDER = 79,
    MOVE_PETAL_DANCE = 80,
    MOVE_STRING_SHOT = 81,
    MOVE_DRAGON_RAGE = 82,
    MOVE_FIRE_SPIN = 83,
    MOVE_THUNDER_SHOCK = 84,
    MOVE_THUNDERBOLT = 85,
    MOVE_THUNDER_WAVE = 86,
    MOVE_THUNDER = 87,
    MOVE_ROCK_THROW = 88,
    MOVE_EARTHQUAKE = 89,
    MOVE_FISSURE = 90,
    MOVE_DIG = 91,
    MOVE_TOXIC = 92,
    MOVE_CONFUSION = 93,
    MOVE_PSYCHIC = 94,
    MOVE_HYPNOSIS = 95,
    MOVE_MEDITATE = 96,
    MOVE_AGILITY = 97,
    MOVE_QUICK_ATTACK = 98,
    MOVE_RAGE = 99,
    MOVE_TELEPORT = 100,
    MOVE_NIGHT_SHADE = 101,
    MOVE_MIMIC = 102,
    MOVE_SCREECH = 103,
    MOVE_DOUBLE_TEAM = 104,
    MOVE_RECOVER = 105,
    MOVE_HARDEN = 106,
    MOVE_MINIMIZE = 107,
    MOVE_SMOKE_SCREEN = 108,
    MOVE_CONFUSE_RAY = 109,
    MOVE_WITHDRAW = 110,
    MOVE_DEFENSE_CURL = 111,
    MOVE_BARRIER = 112,
    MOVE_LIGHT_SCREEN = 113,
    MOVE_HAZE = 114,
    MOVE_REFLECT = 115,
    MOVE_FOCUS_ENERGY = 116,
    MOVE_BIDE = 117,
    MOVE_METRONOME = 118,
    MOVE_MIRROR_MOVE = 119,
    MOVE_SELFDESTRUCT = 120,
    MOVE_EGG_BOMB = 121,
    MOVE_LICK = 122,
    MOVE_SMOG = 123,
    MOVE_SLUDGE = 124,
    MOVE_BONE_CLUB = 125,
    MOVE_FIRE_BLAST = 126,
    MOVE_WATERFALL = 127,
    MOVE_CLAMP = 128,
    MOVE_SWIFT = 129,
    MOVE_SKULL_BASH = 130,
    MOVE_SPIKE_CANNON = 131,
    MOVE_CONSTRICT = 132,
    MOVE_AMNESIA = 133,
    MOVE_KINESIS = 134,
    MOVE_SOFTBOILED = 135,
    MOVE_HI_JUMP_KICK = 136,
    MOVE_GLARE = 137,
    MOVE_DREAM_EATER = 138,
    MOVE_POISON_GAS = 139,
    MOVE_BARRAGE = 140,
    MOVE_LEECH_LIFE = 141,
    MOVE_LOVELY_KISS = 142,
    MOVE_SKY_ATTACK = 143,
    MOVE_TRANSFORM = 144,
    MOVE_BUBBLE = 145,
    MOVE_DIZZY_PUNCH = 146,
    MOVE_SPORE = 147,
    MOVE_FLASH = 148,
    MOVE_PSYWAVE = 149,
    MOVE_SPLASH = 150,
    MOVE_ACID_ARMOR = 151,
    MOVE_CRABHAMMER = 152,
    MOVE_EXPLOSION = 153,
    MOVE_FURY_SWIPES = 154,
    MOVE_BONEMERANG = 155,
    MOVE_REST = 156,
    MOVE_ROCK_SLIDE = 157,
    MOVE_HYPER_FANG = 158,
    MOVE_SHARPEN = 159,
    MOVE_CONVERSION = 160,
    MOVE_TRI_ATTACK = 161,
    MOVE_SUPER_FANG = 162,
    MOVE_SLASH = 163,
    MOVE_SUBSTITUTE = 164,
    MOVE_STRUGGLE = 165,
    MOVE_SKETCH = 166,
    MOVE_TRIPLE_KICK = 167,
    MOVE_THIEF = 168,
    MOVE_SPIDER_WEB = 169,
    MOVE_MIND_READER = 170,
    MOVE_NIGHTMARE = 171,
    MOVE_FLAME_WHEEL = 172,
    MOVE_SNORE = 173,
    MOVE_CURSE = 174,
    MOVE_FLAIL = 175,
    MOVE_CONVERSION_2 = 176,
    MOVE_AEROBLAST = 177,
    MOVE_COTTON_SPORE = 178,
    MOVE_REVERSAL = 179,
    MOVE_SPITE = 180,
    MOVE_POWDER_SNOW = 181,
    MOVE_PROTECT = 182,
    MOVE_MACH_PUNCH = 183,
    MOVE_SCARY_FACE = 184,
    MOVE_FAINT_ATTACK = 185,
    MOVE_SWEET_KISS = 186,
    MOVE_BELLY_DRUM = 187,
    MOVE_SLUDGE_BOMB = 188,
    MOVE_MUD_SLAP = 189,
    MOVE_OCTAZOOKA = 190,
    MOVE_SPIKES = 191,
    MOVE_ZAP_CANNON = 192,
    MOVE_FORESIGHT = 193,
    MOVE_DESTINY_BOND = 194,
    MOVE_PERISH_SONG = 195,
    MOVE_ICY_WIND = 196,
    MOVE_DETECT = 197,
    MOVE_BONE_RUSH = 198,
    MOVE_LOCK_ON = 199,
    MOVE_OUTRAGE = 200,
    MOVE_SANDSTORM = 201,
    MOVE_GIGA_DRAIN = 202,
    MOVE_ENDURE = 203,
    MOVE_CHARM = 204,
    MOVE_ROLLOUT = 205,
    MOVE_FALSE_SWIPE = 206,
    MOVE_SWAGGER = 207,
    MOVE_MILK_DRINK = 208,
    MOVE_SPARK = 209,
    MOVE_FURY_CUTTER = 210,
    MOVE_STEEL_WING = 211,
    MOVE_MEAN_LOOK = 212,
    MOVE_ATTRACT = 213,
    MOVE_SLEEP_TALK = 214,
    MOVE_HEAL_BELL = 215,
    MOVE_RETURN = 216,
    MOVE_PRESENT = 217,
    MOVE_FRUSTRATION = 218,
    MOVE_SAFEGUARD = 219,
    MOVE_PAIN_SPLIT = 220,
    MOVE_SACRED_FIRE = 221,
    MOVE_MAGNITUDE = 222,
    MOVE_DYNAMIC_PUNCH = 223,
    MOVE_MEGAHORN = 224,
    MOVE_DRAGON_BREATH = 225,
    MOVE_BATON_PASS = 226,
    MOVE_ENCORE = 227,
    MOVE_PURSUIT = 228,
    MOVE_RAPID_SPIN = 229,
    MOVE_SWEET_SCENT = 230,
    MOVE_IRON_TAIL = 231,
    MOVE_METAL_CLAW = 232,
    MOVE_VITAL_THROW = 233,
    MOVE_MORNING_SUN = 234,
    MOVE_SYNTHESIS = 235,
    MOVE_MOONLIGHT = 236,
    MOVE_HIDDEN_POWER = 237,
    MOVE_CROSS_CHOP = 238,
    MOVE_TWISTER = 239,
    MOVE_RAIN_DANCE = 240,
    MOVE_SUNNY_DAY = 241,
    MOVE_CRUNCH = 242,
    MOVE_MIRROR_COAT = 243,
    MOVE_PSYCH_UP = 244,
    MOVE_EXTREME_SPEED = 245,
    MOVE_ANCIENT_POWER = 246,
    MOVE_SHADOW_BALL = 247,
    MOVE_FUTURE_SIGHT = 248,
    MOVE_ROCK_SMASH = 249,
    MOVE_WHIRLPOOL = 250,
    MOVE_BEAT_UP = 251,
    MOVE_FAKE_OUT = 252,
    MOVE_UPROAR = 253,
    MOVE_STOCKPILE = 254,
    MOVE_SPIT_UP = 255,
    MOVE_SWALLOW = 256,
    MOVE_HEAT_WAVE = 257,
    MOVE_HAIL = 258,
    MOVE_TORMENT = 259,
    MOVE_FLATTER = 260,
    MOVE_WILL_O_WISP = 261,
    MOVE_MEMENTO = 262,
    MOVE_FACADE = 263,
    MOVE_FOCUS_PUNCH = 264,
    MOVE_SMELLING_SALT = 265,
    MOVE_FOLLOW_ME = 266,
    MOVE_NATURE_POWER = 267,
    MOVE_CHARGE = 268,
    MOVE_TAUNT = 269,
    MOVE_HELPING_HAND = 270,
    MOVE_TRICK = 271,
    MOVE_ROLE_PLAY = 272,
    MOVE_WISH = 273,
    MOVE_ASSIST = 274,
    MOVE_INGRAIN = 275,
    MOVE_SUPERPOWER = 276,
    MOVE_MAGIC_COAT = 277,
    MOVE_RECYCLE = 278,
    MOVE_REVENGE = 279,
    MOVE_BRICK_BREAK = 280,
    MOVE_YAWN = 281,
    MOVE_KNOCK_OFF = 282,
    MOVE_ENDEAVOR = 283,
    MOVE_ERUPTION = 284,
    MOVE_SKILL_SWAP = 285,
    MOVE_IMPRISON = 286,
    MOVE_REFRESH = 287,
    MOVE_GRUDGE = 288,
    MOVE_SNATCH = 289,
    MOVE_SECRET_POWER = 290,
    MOVE_DIVE = 291,
    MOVE_ARM_THRUST = 292,
    MOVE_CAMOUFLAGE = 293,
    MOVE_TAIL_GLOW = 294,
    MOVE_LUSTER_PURGE = 295,
    MOVE_MIST_BALL = 296,
    MOVE_FEATHER_DANCE = 297,
    MOVE_TEETER_DANCE = 298,
    MOVE_BLAZE_KICK = 299,
    MOVE_MUD_SPORT = 300,
    MOVE_ICE_BALL = 301,
    MOVE_NEEDLE_ARM = 302,
    MOVE_SLACK_OFF = 303,
    MOVE_HYPER_VOICE = 304,
    MOVE_POISON_FANG = 305,
    MOVE_CRUSH_CLAW = 306,
    MOVE_BLAST_BURN = 307,
    MOVE_HYDRO_CANNON = 308,
    MOVE_METEOR_MASH = 309,
    MOVE_ASTONISH = 310,
    MOVE_WEATHER_BALL = 311,
    MOVE_AROMATHERAPY = 312,
    MOVE_FAKE_TEARS = 313,
    MOVE_AIR_CUTTER = 314,
    MOVE_OVERHEAT = 315,
    MOVE_ODOR_SLEUTH = 316,
    MOVE_ROCK_TOMB = 317,
    MOVE_SILVER_WIND = 318,
    MOVE_METAL_SOUND = 319,
    MOVE_GRASS_WHISTLE = 320,
    MOVE_TICKLE = 321,
    MOVE_COSMIC_POWER = 322,
    MOVE_WATER_SPOUT = 323,
    MOVE_SIGNAL_BEAM = 324,
    MOVE_SHADOW_PUNCH = 325,
    MOVE_EXTRASENSORY = 326,
    MOVE_SKY_UPPERCUT = 327,
    MOVE_SAND_TOMB = 328,
    MOVE_SHEER_COLD = 329,
    MOVE_MUDDY_WATER = 330,
    MOVE_BULLET_SEED = 331,
    MOVE_AERIAL_ACE = 332,
    MOVE_ICICLE_SPEAR = 333,
    MOVE_IRON_DEFENSE = 334,
    MOVE_BLOCK = 335,
    MOVE_HOWL = 336,
    MOVE_DRAGON_CLAW = 337,
    MOVE_FRENZY_PLANT = 338,
    MOVE_BULK_UP = 339,
    MOVE_BOUNCE = 340,
    MOVE_MUD_SHOT = 341,
    MOVE_POISON_TAIL = 342,
    MOVE_COVET = 343,
    MOVE_VOLT_TACKLE = 344,
    MOVE_MAGICAL_LEAF = 345,
    MOVE_WATER_SPORT = 346,
    MOVE_CALM_MIND = 347,
    MOVE_LEAF_BLADE = 348,
    MOVE_DRAGON_DANCE = 349,
    MOVE_ROCK_BLAST = 350,
    MOVE_SHOCK_WAVE = 351,
    MOVE_WATER_PULSE = 352,
    MOVE_DOOM_DESIRE = 353,
    MOVE_PSYCHO_BOOST = 354,
    MOVE_ROOST = 355,
    MOVE_GRAVITY = 356,
    MOVE_MIRACLE_EYE = 357,
    MOVE_WAKE_UP_SLAP = 358,
    MOVE_HAMMER_ARM = 359,
    MOVE_GYRO_BALL = 360,
    MOVE_HEALING_WISH = 361,
    MOVE_BRINE = 362,
    MOVE_NATURAL_GIFT = 363,
    MOVE_FEINT = 364,
    MOVE_PLUCK = 365,
    MOVE_TAILWIND = 366,
    MOVE_ACUPRESSURE = 367,
    MOVE_METAL_BURST = 368,
    MOVE_U_TURN = 369,
    MOVE_CLOSE_COMBAT = 370,
    MOVE_PAYBACK = 371,
    MOVE_ASSURANCE = 372,
    MOVE_EMBARGO = 373,
    MOVE_FLING = 374,
    MOVE_PSYCHO_SHIFT = 375,
    MOVE_TRUMP_CARD = 376,
    MOVE_HEAL_BLOCK = 377,
    MOVE_WRING_OUT = 378,
    MOVE_POWER_TRICK = 379,
    MOVE_GASTRO_ACID = 380,
    MOVE_LUCKY_CHANT = 381,
    MOVE_ME_FIRST = 382,
    MOVE_COPYCAT = 383,
    MOVE_POWER_SWAP = 384,
    MOVE_GUARD_SWAP = 385,
    MOVE_PUNISHMENT = 386,
    MOVE_LAST_RESORT = 387,
    MOVE_WORRY_SEED = 388,
    MOVE_SUCKER_PUNCH = 389,
    MOVE_TOXIC_SPIKES = 390,
    MOVE_HEART_SWAP = 391,
    MOVE_AQUA_RING = 392,
    MOVE_MAGNET_RISE = 393,
    MOVE_FLARE_BLITZ = 394,
    MOVE_FORCE_PALM = 395,
    MOVE_AURA_SPHERE = 396,
    MOVE_ROCK_POLISH = 397,
    MOVE_POISON_JAB = 398,
    MOVE_DARK_PULSE = 399,
    MOVE_NIGHT_SLASH = 400,
    MOVE_AQUA_TAIL = 401,
    MOVE_SEED_BOMB = 402,
    MOVE_AIR_SLASH = 403,
    MOVE_X_SCISSOR = 404,
    MOVE_BUG_BUZZ = 405,
    MOVE_DRAGON_PULSE = 406,
    MOVE_DRAGON_RUSH = 407,
    MOVE_POWER_GEM = 408,
    MOVE_DRAIN_PUNCH = 409,
    MOVE_VACUUM_WAVE = 410,
    MOVE_FOCUS_BLAST = 411,
    MOVE_ENERGY_BALL = 412,
    MOVE_BRAVE_BIRD = 413,
    MOVE_EARTH_POWER = 414,
    MOVE_SWITCHEROO = 415,
    MOVE_GIGA_IMPACT = 416,
    MOVE_NASTY_PLOT = 417,
    MOVE_BULLET_PUNCH = 418,
    MOVE_AVALANCHE = 419,
    MOVE_ICE_SHARD = 420,
    MOVE_SHADOW_CLAW = 421,
    MOVE_THUNDER_FANG = 422,
    MOVE_ICE_FANG = 423,
    MOVE_FIRE_FANG = 424,
    MOVE_SHADOW_SNEAK = 425,
    MOVE_MUD_BOMB = 426,
    MOVE_PSYCHO_CUT = 427,
    MOVE_ZEN_HEADBUTT = 428,
    MOVE_MIRROR_SHOT = 429,
    MOVE_FLASH_CANNON = 430,
    MOVE_ROCK_CLIMB = 431,
    MOVE_DEFOG = 432,
    MOVE_TRICK_ROOM = 433,
    MOVE_DRACO_METEOR = 434,
    MOVE_DISCHARGE = 435,
    MOVE_LAVA_PLUME = 436,
    MOVE_LEAF_STORM = 437,
    MOVE_POWER_WHIP = 438,
    MOVE_ROCK_WRECKER = 439,
    MOVE_CROSS_POISON = 440,
    MOVE_GUNK_SHOT = 441,
    MOVE_IRON_HEAD = 442,
    MOVE_MAGNET_BOMB = 443,
    MOVE_STONE_EDGE = 444,
    MOVE_CAPTIVATE = 445,
    MOVE_STEALTH_ROCK = 446,
    MOVE_GRASS_KNOT = 447,
    MOVE_CHATTER = 448,
    MOVE_JUDGMENT = 449,
    MOVE_BUG_BITE = 450,
    MOVE_CHARGE_BEAM = 451,
    MOVE_WOOD_HAMMER = 452,
    MOVE_AQUA_JET = 453,
    MOVE_ATTACK_ORDER = 454,
    MOVE_DEFEND_ORDER = 455,
    MOVE_HEAL_ORDER = 456,
    MOVE_HEAD_SMASH = 457,
    MOVE_DOUBLE_HIT = 458,
    MOVE_ROAR_OF_TIME = 459,
    MOVE_SPACIAL_REND = 460,
    MOVE_LUNAR_DANCE = 461,
    MOVE_CRUSH_GRIP = 462,
    MOVE_MAGMA_STORM = 463,
    MOVE_DARK_VOID = 464,
    MOVE_SEED_FLARE = 465,
    MOVE_OMINOUS_WIND = 466,
    MOVE_SHADOW_FORCE = 467,
    MAX_MOVES = 468,
};

typedef struct BattleSystem BattleSystem;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;



typedef struct GridMenuCursorPosition {
    u8 xCoord1;
    u8 yCoord1;
    u8 xCoord2;
    u8 yCoord2;
    u8 upIndex;
    u8 downIndex;
    u8 leftIndex;
    u8 rightIndex;
} GridMenuCursorPosition;

void ScheduleWindowCopyToVram(Window *window);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef u16 charcode_t;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

enum BattleLearnMoveScreenWindow {
    BATTLE_LEARN_MOVE_SCREEN_WINDOW_POKEMON_NAME = 0,
    BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_1,
    BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_2,
    BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_3,
    BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_4,
    BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_TO_LEARN,
};

typedef u32 TextColor;

u32 GetMoveAttr(u32 moveId, int);

extern const u32 ov08_02224FE0[5];

void ov08_0221DDCC(BattleParty *battleParty, u32 windowIndex, enum Font font, u16 partyIndex, u8 xOffset, u8 yOffset);

void PrintMoveName(BattleParty *battleParty, enum Move move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void PrintMoveContestEffect(BattleParty *battleParty, u32 windowIndex, enum Move move);

void ov08_0221F284(BattleParty *battleParty, BattlePartyPokemonMove *move, u32 windowIndex);

void ov08_0221FC7C(BattleParty *battleParty);

void ov08_0221E244(BattleParty *battleParty, enum Move move, u32 windowIndex, u32 textID, u16 font, u16 yOffset, TextColor color);

void ov08_0221FC7C(BattleParty *battleParty) {
    BattlePartyPokemonMove moveToLearn;

    FillWindowPixelBuffer(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_POKEMON_NAME], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_1], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_2], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_3], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_4], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_TO_LEARN], 0);

    ov08_0221DDCC(battleParty, BATTLE_LEARN_MOVE_SCREEN_WINDOW_POKEMON_NAME, FONT_SYSTEM, battleParty->context->selectedPartyIndex, 0, 0);

    for (u32 i = 0; i < 4; i++) {
        BattlePartyPokemonMove *move = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[i];

        if (move->move == MOVE_NONE) {
            continue;
        }

        ov08_0221E244(battleParty, (enum Move)(move->move), BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_1 + i, ov08_02224FE0[i], 4, 7, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((9 & 0xFF) << 0))));
        ov08_0221F284(battleParty, move, BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_1 + i);
    }

    ov08_0221E244(battleParty, (enum Move)(battleParty->context->moveToLearn), BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_TO_LEARN, 73, 4, 7, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((9 & 0xFF) << 0))));

    moveToLearn.currentPP = GetMoveAttr(battleParty->context->moveToLearn, BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_TO_LEARN);
    moveToLearn.maxPP = moveToLearn.currentPP;

    ov08_0221F284(battleParty, &moveToLearn, BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_TO_LEARN);

    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_1]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_2]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_3]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_4]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_LEARN_MOVE_SCREEN_WINDOW_MOVE_TO_LEARN]);
}

enum BattleConfirmLearnMoveScreenWindow {
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POKEMON_NAME = 0,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_MOVE_NAME,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_PP_LABEL,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_PP,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_ACCURACY_LABEL,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POWER_LABEL,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_ACCURACY,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POWER,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_DESCRIPTION,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CATEGORY_LABEL,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CATEGORY,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CONFIRM,
};

void ov08_0221E2E8(BattleParty *battleParty, u16 windowIndex, u8 xOffset, u8 yOffset);

void ov08_0221ECE8(BattleParty *battleParty, u32 windowIndex);

void ov08_0221ED30(BattleParty *battleParty, u32 windowIndex, u32 moveAccuracyStat);

void ov08_0221EE18(BattleParty *battleParty, u32 windowIndex);

void ov08_0221EE60(BattleParty *battleParty, u32 windowIndex, u32 movePowerStat);

void PrintMoveDescription(BattleParty *battleParty, u32 windowIndex, u32 textID);

void ov08_0221EFA8(BattleParty *battleParty, u32 windowIndex);

void PrintMoveCategory(BattleParty *battleParty, u32 windowIndex, u32 moveCategory);

void ov08_0221F07C(BattleParty *battleParty, u32 windowIndex, u32 currentPP, u32 maxPP);

void PrintConfirmMoveButton(BattleParty *battleParty, u32 windowIndex);

void ov08_0221FDA4(BattleParty *battleParty);

void ov08_0221EF48(BattleParty *battleParty, u32 windowIndex, u32 textID);

void ov08_0221F00C(BattleParty *battleParty, u32 windowIndex, u32 moveCategory);

void ov08_0221F1B0(BattleParty *battleParty, u32 windowIndex);

void ov08_0221FDA4(BattleParty *battleParty) {
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POKEMON_NAME], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_PP_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_PP], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_MOVE_NAME], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_ACCURACY_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_ACCURACY], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POWER_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POWER], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CATEGORY_LABEL], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CATEGORY], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_DESCRIPTION], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CONFIRM], 0);

    ov08_0221DDCC(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POKEMON_NAME, FONT_SYSTEM, battleParty->context->selectedPartyIndex, 0, 0);
    ov08_0221E2E8(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_PP_LABEL, 0, 0);
    ov08_0221ECE8(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_ACCURACY_LABEL);
    ov08_0221EE18(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POWER_LABEL);
    ov08_0221EFA8(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CATEGORY_LABEL);

    if (battleParty->context->selectedMoveSlot < 4) {
        BattlePartyPokemonMove *move = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[battleParty->context->selectedMoveSlot];

        ov08_0221E244(battleParty, (enum Move)(move->move), BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_MOVE_NAME, ov08_02224FE0[battleParty->context->selectedMoveSlot], FONT_SYSTEM, 0, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        ov08_0221ED30(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_ACCURACY, move->accuracy);
        ov08_0221EE60(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POWER, move->power);
        ov08_0221EF48(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_DESCRIPTION, move->move);
        ov08_0221F00C(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CATEGORY, move->class);
        ov08_0221F07C(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_PP, move->currentPP, move->maxPP);
    } else {
        u32 pp = GetMoveAttr(battleParty->context->moveToLearn, MOVEATTRIBUTE_PP);

        ov08_0221E244(battleParty, (enum Move)(battleParty->context->moveToLearn), BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_MOVE_NAME, 73, FONT_SYSTEM, 0, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
        ov08_0221EF48(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_DESCRIPTION, battleParty->context->moveToLearn);
        ov08_0221ED30(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_ACCURACY, GetMoveAttr(battleParty->context->moveToLearn, MOVEATTRIBUTE_ACCURACY));
        ov08_0221EE60(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_POWER, GetMoveAttr(battleParty->context->moveToLearn, MOVEATTRIBUTE_POWER));
        ov08_0221F00C(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CATEGORY, GetMoveAttr(battleParty->context->moveToLearn, MOVEATTRIBUTE_CLASS));
        ov08_0221F07C(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_PP, pp, pp);
    }

    ov08_0221F1B0(battleParty, BATTLE_CONFIRM_LEARN_MOVE_SCREEN_WINDOW_CONFIRM);
}

enum BattleRestoreMovePPScreenWindow {
    BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_POKEMON_NAME = 0,
    BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_1,
    BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_2,
    BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_3,
    BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_4,
};

enum ItemDataParam {
    ITEM_PARAM_PRICE,
    ITEM_PARAM_HOLD_EFFECT,
    ITEM_PARAM_EFFECT_PARAM,
    ITEM_PARAM_PREVENT_TOSS,
    ITEM_PARAM_CAN_REGISTER,
    ITEM_PARAM_FIELD_POCKET,
    ITEM_PARAM_FIELD_USE_FUNC,
    ITEM_PARAM_BATTLE_USE_CATEGORY,
    ITEM_PARAM_PLUCK_EFFECT,
    ITEM_PARAM_FLING_EFFECT,
    ITEM_PARAM_FLING_POWER,
    ITEM_PARAM_NATURAL_GIFT_POWER,
    ITEM_PARAM_NATURAL_GIFT_TYPE,
    ITEM_PARAM_BATTLE_POCKET,
    ITEM_PARAM_PARTY_USE,
    ITEM_PARAM_HEAL_SLEEP,
    ITEM_PARAM_HEAL_POISON,
    ITEM_PARAM_HEAL_BURN,
    ITEM_PARAM_HEAL_FREEZE,
    ITEM_PARAM_HEAL_PARALYSIS,
    ITEM_PARAM_HEAL_CONFUSION,
    ITEM_PARAM_HEAL_ATTRACT,
    ITEM_PARAM_GUARD_SPEC,
    ITEM_PARAM_REVIVE,
    ITEM_PARAM_REVIVE_ALL,
    ITEM_PARAM_LEVEL_UP,
    ITEM_PARAM_EVOLVE,
    ITEM_PARAM_ATK_STAGES,
    ITEM_PARAM_DEF_STAGES,
    ITEM_PARAM_SPATK_STAGES,
    ITEM_PARAM_SPDEF_STAGES,
    ITEM_PARAM_SPEED_STAGES,
    ITEM_PARAM_ACC_STAGES,
    ITEM_PARAM_CRIT_STAGES,
    ITEM_PARAM_PP_UP,
    ITEM_PARAM_PP_MAX,
    ITEM_PARAM_PP_RESTORE,
    ITEM_PARAM_PP_RESTORE_ALL,
    ITEM_PARAM_HP_RESTORE,
    ITEM_PARAM_GIVE_HP_EVS,
    ITEM_PARAM_GIVE_ATK_EVS,
    ITEM_PARAM_GIVE_DEF_EVS,
    ITEM_PARAM_GIVE_SPEED_EVS,
    ITEM_PARAM_GIVE_SPATK_EVS,
    ITEM_PARAM_GIVE_SPDEF_EVS,
    ITEM_PARAM_GIVE_FRIENDSHIP_LOW,
    ITEM_PARAM_GIVE_FRIENDSHIP_MED,
    ITEM_PARAM_GIVE_FRIENDSHIP_HIGH,
    ITEM_PARAM_HP_EVS,
    ITEM_PARAM_ATK_EVS,
    ITEM_PARAM_DEF_EVS,
    ITEM_PARAM_SPEED_EVS,
    ITEM_PARAM_SPATK_EVS,
    ITEM_PARAM_SPDEF_EVS,
    ITEM_PARAM_HP_RESTORED,
    ITEM_PARAM_PP_RESTORED,
    ITEM_PARAM_FRIENDSHIP_LOW,
    ITEM_PARAM_FRIENDSHIP_MED,
    ITEM_PARAM_FRIENDSHIP_HIGH,
};


void ov08_0221E340(BattleParty *battleParty, u32 textID);

void ov08_0221FF70(BattleParty *battleParty);

void ov08_0221FF70(BattleParty *battleParty) {
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_POKEMON_NAME], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_1], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_2], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_3], 0);
    FillWindowPixelBuffer(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_4], 0);

    ov08_0221DDCC(battleParty, BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_POKEMON_NAME, FONT_SYSTEM, battleParty->context->selectedPartyIndex, 0, 0);

    for (u32 i = 0; i < 4; i++) {
        BattlePartyPokemonMove *move = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex].moves[i];

        if (move->move == MOVE_NONE) {
            continue;
        }

        ov08_0221E244(battleParty, (enum Move)(move->move), BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_1 + i, ov08_02224FE0[i], 4, 7, ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((9 & 0xFF) << 0))));
        ov08_0221F284(battleParty, move, BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_1 + i);
    }

    if (GetItemAttr(battleParty->context->selectedBattleBagItem, ITEM_PARAM_PP_RESTORE_ALL, battleParty->context->heapID) == 0) {
        ov08_0221E340(battleParty, 94);
    }

    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_1]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_2]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_3]);
    ScheduleWindowCopyToVram(&battleParty->windows[BATTLE_RESTORE_MOVE_PP_SCREEN_WINDOW_MOVE_4]);
}
