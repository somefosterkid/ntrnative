#include "global.h"
#include "sprite_resources_header.h"
#include "filesystem_files_def.h"
#include "unk_02013534.h"
#include "item.h"
#include "overlay_07_ball_rotation.h"
#include "seal_case.h"

struct AIContext;

struct BallCapsule;

struct BallRotation;

struct BallSeal;

struct BattleContext;

struct BattleMessage;

struct BattleMon;

struct BattleScriptTaskData;

struct BattleSystem;

struct BoxPokemon;

struct CharTransferAllocation;

struct MailMessage;

struct FieldConditions;

struct TextOBJ;

struct ItemData;

struct Mail;

struct ManagedSprite;

struct MoveContestData;

struct MoveEffectsData;

struct MoveFailFlags;

struct MoveTable;

struct PartyPokemon;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct SelfTurnFlags;

struct SideConditions;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct TurnFlags;

enum BattleAction {
    BATTLE_ACTION_PICK_COMMAND = 0,
    BATTLE_ACTION_CHOOSE_TARGET = 1,
    BATTLE_ACTION_TEMP_VALUE = 2,
    BATTLE_ACTION_SELECTED_COMMAND = 3,
    MAX_BATTLE_ACTIONS = 4,
};

enum BattleContextParam {
    BATTLECTX_SIDE_CONDITIONS_MASK = 0,
    BATTLECTX_SIDE_MIST_TURNS = 1,
    BATTLECTX_SELECTED_PARTY_SLOT = 2,
    BATTLECTX_TOTAL_TURNS = 3,
    BATTLECTX_LEVEL_UP_MONS = 4,
    BATTLECTX_SAFARI_ESCAPE_COUNT = 5,
    BATTLECTX_TOTAL_FAINTED_FOR = 6,
    BATTLECTX_TOTAL_DAMAGE_FOR = 7,
    BATTLECTX_ACTION_FOR = 8,
    BATTLECTX_AICTX_DEFENDER = 9,
    BATTLECTX_SWITCHING_MASK = 10,
    BATTLECTX_AICTX_SELECTED_TARGET = 11,
    BATTLECTX_ACTION_TEMP_FOR = 12,
    BATTLECTX_CONTROL_COMMAND = 13,
    BATTLECTX_NEXT_CONTROL_COMMAND = 14,
};

enum BattleMonParam {
    BATTLEMON_SPECIES = 0,
    BATTLEMON_ATTACK = 1,
    BATTLEMON_DEFENSE = 2,
    BATTLEMON_SPEED = 3,
    BATTLEMON_SP_ATTACK = 4,
    BATTLEMON_SP_DEFENSE = 5,
    BATTLEMON_MOVE_1 = 6,
    BATTLEMON_MOVE_2 = 7,
    BATTLEMON_MOVE_3 = 8,
    BATTLEMON_MOVE_4 = 9,
    BATTLEMON_HP_IV = 10,
    BATTLEMON_ATTACK_IV = 11,
    BATTLEMON_DEFENSE_IV = 12,
    BATTLEMON_SPEED_IV = 13,
    BATTLEMON_SP_ATTACK_IV = 14,
    BATTLEMON_SP_DEFENSE_IV = 15,
    BATTLEMON_IS_EGG = 16,
    BATTLEMON_HAS_NICKNAME = 17,
    BATTLEMON_HP_STAGE = 18,
    BATTLEMON_ATTACK_STAGE = 19,
    BATTLEMON_DEFENSE_STAGE = 20,
    BATTLEMON_SPEED_STAGE = 21,
    BATTLEMON_SP_ATTACK_STAGE = 22,
    BATTLEMON_SP_DEFENSE_STAGE = 23,
    BATTLEMON_ACCURACY_STAGE = 24,
    BATTLEMON_EVASION_STAGE = 25,
    BATTLEMON_ABILITY = 26,
    BATTLEMON_TYPE_1 = 27,
    BATTLEMON_TYPE_2 = 28,
    BATTLEMON_GENDER = 29,
    BATTLEMON_IS_SHINY = 30,
    BATTLEMON_CUR_PP_1 = 31,
    BATTLEMON_CUR_PP_2 = 32,
    BATTLEMON_CUR_PP_3 = 33,
    BATTLEMON_CUR_PP_4 = 34,
    BATTLEMON_PP_UPS_1 = 35,
    BATTLEMON_PP_UPS_2 = 36,
    BATTLEMON_PP_UPS_3 = 37,
    BATTLEMON_PP_UPS_4 = 38,
    BATTLEMON_MAX_PP_1 = 39,
    BATTLEMON_MAX_PP_2 = 40,
    BATTLEMON_MAX_PP_3 = 41,
    BATTLEMON_MAX_PP_4 = 42,
    BATTLEMON_LEVEL = 43,
    BATTLEMON_FRIENDSHIP = 44,
    BATTLEMON_NICKNAME = 45,
    BATTLEMON_NICKNAME_STRING = 46,
    BATTLEMON_CUR_HP = 47,
    BATTLEMON_MAX_HP = 48,
    BATTLEMON_OT_NAME = 49,
    BATTLEMON_EXP = 50,
    BATTLEMON_PERSONALITY = 51,
    BATTLEMON_STATUS = 52,
    BATTLEMON_VOLATILE_STATUS = 53,
    BATTLEMON_OT_ID = 54,
    BATTLEMON_HELD_ITEM = 55,
    BATTLEMON_TIMES_DAMAGED = 56,
    BATTLEMON_TRAINER_MESSAGE_FLAGS = 57,
    BATTLEMON_OT_GENDER = 58,
    BATTLEMON_MOVE_EFFECTS_MASK = 59,
    BATTLEMON_MOVE_EFFECTS_TEMP = 60,
    BATTLEMON_DISABLED_TURNS = 61,
    BATTLEMON_ENCORED_TURNS = 62,
    BATTLEMON_CHARGED_TURNS = 63,
    BATTLEMON_TAUNTED_TURNS = 64,
    BATTLEMON_SUCCESSFUL_PROTECT_TURNS = 65,
    BATTLEMON_PERISH_SONG_TURNS = 66,
    BATTLEMON_ROLLOUT_COUNT = 67,
    BATTLEMON_FURY_CUTTER_COUNT = 68,
    BATTLEMON_STOCKPILE_COUNT = 69,
    BATTLEMON_STOCKPILE_DEF_BOOSTS = 70,
    BATTLEMON_STOCKPILE_SPDEF_BOOSTS = 71,
    BATTLEMON_TRUANT = 72,
    BATTLEMON_FLASH_FIRE = 73,
    BATTLEMON_LOCK_ON_TARGET = 74,
    BATTLEMON_MIMICKED_MOVE_SLOT = 75,
    BATTLEMON_BIND_TARGET = 76,
    BATTLEMON_MEAN_LOOK_TARGET = 77,
    BATTLEMON_LAST_RESORT_COUNT = 78,
    BATTLEMON_MAGNET_RISE_TURNS = 79,
    BATTLEMON_HEAL_BLOCK_TURNS = 80,
    BATTLEMON_EMBARGO_TURNS = 81,
    BATTLEMON_CAN_UNBURDEN = 82,
    BATTLEMON_METRONOME_TURNS = 83,
    BATTLEMON_MICLE_BERRY = 84,
    BATTLEMON_CUSTAP_BERRY = 85,
    BATTLEMON_QUICK_CLAW = 86,
    BATTLEMON_RECHARGE_TURN_NUMBER = 87,
    BATTLEMON_FAKE_OUT_TURN_NUMBER = 88,
    BATTLEMON_SLOW_START_TURN_NUMBER = 89,
    BATTLEMON_SUBSTITUTE_HP = 90,
    BATTLEMON_TRANSFORMED_PERSONALITY = 91,
    BATTLEMON_DISABLED_MOVE = 92,
    BATTLEMON_ENCORED_MOVE = 93,
    BATTLEMON_BINDING_MOVE = 94,
    BATTLEMON_ITEM_HP_RECOVERY = 95,
    BATTLEMON_SLOW_START_ANNOUNCED = 96,
    BATTLEMON_SLOW_START_FINISHED = 97,
    BATTLEMON_FORM_NUM = 98,
    BATTLEMON_UNUSED_99 = 99,
    BATTLEMON_TEMP = 100,
};

enum BattleSideEffectType {
    SIDE_EFFECT_TYPE_NONE = 0,
    SIDE_EFFECT_TYPE_DIRECT = 1,
    SIDE_EFFECT_TYPE_INDIRECT = 2,
    SIDE_EFFECT_TYPE_ABILITY = 3,
    SIDE_EFFECT_TYPE_MOVE_EFFECT = 4,
    SIDE_EFFECT_TYPE_HELD_ITEM = 5,
    SIDE_EFFECT_TYPE_TOXIC_SPIKES = 6,
    SIDE_EFFECT_TYPE_DISOBEDIENCE = 7,
};

typedef struct BattleSystem BattleSystem;

enum PokemonDataParam {
    MON_DATA_PERSONALITY = 0,
    MON_DATA_IS_PARTY_DECRYPTED = 1,
    MON_DATA_IS_BOX_DECRYPTED = 2,
    MON_DATA_CHECKSUM_FAILED = 3,
    MON_DATA_CHECKSUM = 4,
    MON_DATA_SPECIES = 5,
    MON_DATA_HELD_ITEM = 6,
    MON_DATA_OT_ID = 7,
    MON_DATA_EXPERIENCE = 8,
    MON_DATA_FRIENDSHIP = 9,
    MON_DATA_ABILITY = 10,
    MON_DATA_MARKINGS = 11,
    MON_DATA_LANGUAGE = 12,
    MON_DATA_HP_EV = 13,
    MON_DATA_ATK_EV = 14,
    MON_DATA_DEF_EV = 15,
    MON_DATA_SPEED_EV = 16,
    MON_DATA_SPATK_EV = 17,
    MON_DATA_SPDEF_EV = 18,
    MON_DATA_COOL = 19,
    MON_DATA_BEAUTY = 20,
    MON_DATA_CUTE = 21,
    MON_DATA_SMART = 22,
    MON_DATA_TOUGH = 23,
    MON_DATA_SHEEN = 24,
    MON_DATA_SINNOH_CHAMP_RIBBON = 25,
    MON_DATA_ABILITY_RIBBON = 26,
    MON_DATA_GREAT_ABILITY_RIBBON = 27,
    MON_DATA_DOUBLE_ABILITY_RIBBON = 28,
    MON_DATA_MULTI_ABILITY_RIBBON = 29,
    MON_DATA_PAIR_ABILITY_RIBBON = 30,
    MON_DATA_WORLD_ABILITY_RIBBON = 31,
    MON_DATA_ALERT_RIBBON = 32,
    MON_DATA_SHOCK_RIBBON = 33,
    MON_DATA_DOWNCAST_RIBBON = 34,
    MON_DATA_CARELESS_RIBBON = 35,
    MON_DATA_RELAX_RIBBON = 36,
    MON_DATA_SNOOZE_RIBBON = 37,
    MON_DATA_SMILE_RIBBON = 38,
    MON_DATA_GORGEOUS_RIBBON = 39,
    MON_DATA_ROYAL_RIBBON = 40,
    MON_DATA_GORGEOUS_ROYAL_RIBBON = 41,
    MON_DATA_FOOTPRINT_RIBBON = 42,
    MON_DATA_RECORD_RIBBON = 43,
    MON_DATA_HISTORY_RIBBON = 44,
    MON_DATA_LEGEND_RIBBON = 45,
    MON_DATA_RED_RIBBON = 46,
    MON_DATA_GREEN_RIBBON = 47,
    MON_DATA_BLUE_RIBBON = 48,
    MON_DATA_FESTIVAL_RIBBON = 49,
    MON_DATA_CARNIVAL_RIBBON = 50,
    MON_DATA_CLASSIC_RIBBON = 51,
    MON_DATA_PREMIER_RIBBON = 52,
    MON_DATA_UNUSED_RIBBON_53 = 53,
    MON_DATA_MOVE1 = 54,
    MON_DATA_MOVE2 = 55,
    MON_DATA_MOVE3 = 56,
    MON_DATA_MOVE4 = 57,
    MON_DATA_MOVE1_PP = 58,
    MON_DATA_MOVE2_PP = 59,
    MON_DATA_MOVE3_PP = 60,
    MON_DATA_MOVE4_PP = 61,
    MON_DATA_MOVE1_PP_UPS = 62,
    MON_DATA_MOVE2_PP_UPS = 63,
    MON_DATA_MOVE3_PP_UPS = 64,
    MON_DATA_MOVE4_PP_UPS = 65,
    MON_DATA_MOVE1_MAX_PP = 66,
    MON_DATA_MOVE2_MAX_PP = 67,
    MON_DATA_MOVE3_MAX_PP = 68,
    MON_DATA_MOVE4_MAX_PP = 69,
    MON_DATA_HP_IV = 70,
    MON_DATA_ATK_IV = 71,
    MON_DATA_DEF_IV = 72,
    MON_DATA_SPEED_IV = 73,
    MON_DATA_SPATK_IV = 74,
    MON_DATA_SPDEF_IV = 75,
    MON_DATA_IS_EGG = 76,
    MON_DATA_HAS_NICKNAME = 77,
    MON_DATA_COOL_RIBBON = 78,
    MON_DATA_COOL_RIBBON_SUPER = 79,
    MON_DATA_COOL_RIBBON_HYPER = 80,
    MON_DATA_COOL_RIBBON_MASTER = 81,
    MON_DATA_BEAUTY_RIBBON = 82,
    MON_DATA_BEAUTY_RIBBON_SUPER = 83,
    MON_DATA_BEAUTY_RIBBON_HYPER = 84,
    MON_DATA_BEAUTY_RIBBON_MASTER = 85,
    MON_DATA_CUTE_RIBBON = 86,
    MON_DATA_CUTE_RIBBON_SUPER = 87,
    MON_DATA_CUTE_RIBBON_HYPER = 88,
    MON_DATA_CUTE_RIBBON_MASTER = 89,
    MON_DATA_SMART_RIBBON = 90,
    MON_DATA_SMART_RIBBON_SUPER = 91,
    MON_DATA_SMART_RIBBON_HYPER = 92,
    MON_DATA_SMART_RIBBON_MASTER = 93,
    MON_DATA_TOUGH_RIBBON = 94,
    MON_DATA_TOUGH_RIBBON_SUPER = 95,
    MON_DATA_TOUGH_RIBBON_HYPER = 96,
    MON_DATA_TOUGH_RIBBON_MASTER = 97,
    MON_DATA_CHAMPION_RIBBON = 98,
    MON_DATA_WINNING_RIBBON = 99,
    MON_DATA_VICTORY_RIBBON = 100,
    MON_DATA_ARTIST_RIBBON = 101,
    MON_DATA_EFFORT_RIBBON = 102,
    MON_DATA_MARINE_RIBBON = 103,
    MON_DATA_LAND_RIBBON = 104,
    MON_DATA_SKY_RIBBON = 105,
    MON_DATA_COUNTRY_RIBBON = 106,
    MON_DATA_NATIONAL_RIBBON = 107,
    MON_DATA_EARTH_RIBBON = 108,
    MON_DATA_WORLD_RIBBON = 109,
    MON_DATA_FATEFUL_ENCOUNTER = 110,
    MON_DATA_GENDER = 111,
    MON_DATA_FORM = 112,
    MON_DATA_UNUSED_113 = 113,
    MON_DATA_UNUSED_114 = 114,
    MON_DATA_EGG_LOCATION_PTHGSS = 115,
    MON_DATA_MET_LOCATION_PTHGSS = 116,
    MON_DATA_NICKNAME = 117,
    MON_DATA_NICKNAME_AND_FLAG = 118,
    MON_DATA_NICKNAME_STRING = 119,
    MON_DATA_NICKNAME_STRING_AND_FLAG = 120,
    MON_DATA_UNUSED_121 = 121,
    MON_DATA_MET_GAME = 122,
    MON_DATA_SUPER_COOL_RIBBON = 123,
    MON_DATA_SUPER_COOL_RIBBON_GREAT = 124,
    MON_DATA_SUPER_COOL_RIBBON_ULTRA = 125,
    MON_DATA_SUPER_COOL_RIBBON_MASTER = 126,
    MON_DATA_SUPER_BEAUTY_RIBBON = 127,
    MON_DATA_SUPER_BEAUTY_RIBBON_GREAT = 128,
    MON_DATA_SUPER_BEAUTY_RIBBON_ULTRA = 129,
    MON_DATA_SUPER_BEAUTY_RIBBON_MASTER = 130,
    MON_DATA_SUPER_CUTE_RIBBON = 131,
    MON_DATA_SUPER_CUTE_RIBBON_GREAT = 132,
    MON_DATA_SUPER_CUTE_RIBBON_ULTRA = 133,
    MON_DATA_SUPER_CUTE_RIBBON_MASTER = 134,
    MON_DATA_SUPER_SMART_RIBBON = 135,
    MON_DATA_SUPER_SMART_RIBBON_GREAT = 136,
    MON_DATA_SUPER_SMART_RIBBON_ULTRA = 137,
    MON_DATA_SUPER_SMART_RIBBON_MASTER = 138,
    MON_DATA_SUPER_TOUGH_RIBBON = 139,
    MON_DATA_SUPER_TOUGH_RIBBON_GREAT = 140,
    MON_DATA_SUPER_TOUGH_RIBBON_ULTRA = 141,
    MON_DATA_SUPER_TOUGH_RIBBON_MASTER = 142,
    MON_DATA_UNUSED_RIBBON_143 = 143,
    MON_DATA_OT_NAME = 144,
    MON_DATA_OT_NAME_STRING = 145,
    MON_DATA_EGG_YEAR = 146,
    MON_DATA_EGG_MONTH = 147,
    MON_DATA_EGG_DAY = 148,
    MON_DATA_MET_YEAR = 149,
    MON_DATA_MET_MONTH = 150,
    MON_DATA_MET_DAY = 151,
    MON_DATA_EGG_LOCATION = 152,
    MON_DATA_MET_LOCATION = 153,
    MON_DATA_POKERUS = 154,
    MON_DATA_POKEBALL = 155,
    MON_DATA_MET_LEVEL = 156,
    MON_DATA_OT_GENDER = 157,
    MON_DATA_MET_TERRAIN = 158,
    MON_DATA_UNUSED_159 = 159,
    MON_DATA_STATUS = 160,
    MON_DATA_LEVEL = 161,
    MON_DATA_BALL_CAPSULE_ID = 162,
    MON_DATA_HP = 163,
    MON_DATA_MAX_HP = 164,
    MON_DATA_ATK = 165,
    MON_DATA_DEF = 166,
    MON_DATA_SPEED = 167,
    MON_DATA_SP_ATK = 168,
    MON_DATA_SP_DEF = 169,
    MON_DATA_MAIL = 170,
    MON_DATA_BALL_CAPSULE = 171,
    MON_DATA_SPECIES_EXISTS = 172,
    MON_DATA_SANITY_IS_EGG = 173,
    MON_DATA_SPECIES_OR_EGG = 174,
    MON_DATA_COMBINED_IVS = 175,
    MON_DATA_PRINT_GENDER = 176,
    MON_DATA_TYPE_1 = 177,
    MON_DATA_TYPE_2 = 178,
    MON_DATA_SPECIES_NAME = 179,
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

typedef u16 charcode_t;

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

typedef struct MoveContestData {
    u8 effect;
    u8 type;
    u8 padding02[2];
} MoveContestData;

typedef struct MoveTable {
    u16 effect;
    u8 class;
    u8 power;
    u8 type;
    u8 accuracy;
    u8 pp;
    u8 effectChance;
    u16 range;
    s8 priority;
    u8 flags;
    MoveContestData contest;
} MoveTable;

typedef struct AIContext {
    u8 evalStep;
    u8 moveSlot;
    u16 move;

    s8 moveScore[4];

    int calcTemp;
    u32 thinkingMask;

    u8 stateFlags;
    u8 thinkingBitShift;
    u8 padding0012;
    u8 padding0013;
    int padding0014;

    u8 moveDamageRolls[4];

    u16 battlerMoves[4][4];
    u8 battlerAbilities[4];
    u16 battlerHeldItems[4];

    u16 trainerItems[2][4];

    u32 scriptStackPointer[8];
    u8 scriptStackSize;

    u8 trainerItemCounts[2];
    u8 attacker;
    u8 defender;
    u8 usedItemType[2];
    u8 usedItemCondition[2];
    u16 usedItem[2];

    u8 selectedTarget[4];

    MoveTable moveTable[MAX_MOVES];
    ItemData *itemTable;

    u16 padding1DD0[4];
    u16 padding1DD8[4];
} AIContext;

typedef struct MoveEffectsData {
    u32 disabledTurns : 3;
    u32 encoredTurns : 3;
    u32 chargedTurns : 2;
    u32 tauntedTurns : 3;
    u32 protectSuccessTurns : 2;
    u32 perishSongTurns : 2;
    u32 rolloutCount : 3;
    u32 furyCutterCount : 3;
    u32 stockpileCount : 3;
    u32 stockpileDefBoosts : 3;
    u32 stockpileSpDefBoosts : 3;
    u32 truant : 1;
    u32 flashFire : 1;

    u32 lockOnTarget : 2;
    u32 mimickedMoveSlot : 4;
    u32 bindTarget : 2;
    u32 meanLookTarget : 2;
    u32 lastResortCount : 3;
    u32 magnetRiseTurns : 3;
    u32 healBlockTurns : 3;
    u32 embargoTurns : 3;
    u32 canUnburden : 1;
    u32 metronomeTurns : 4;
    u32 micleBerry : 1;
    u32 custapBerry : 1;
    u32 quickClaw : 1;
    u32 meFirst : 1;
    u32 padding0004_1F : 1;

    int rechargeTurnNumber;
    int fakeOutTurnNumber;
    int slowStartTurnNumber;
    int meFirstTurnNumber;
    int substituteHP;
    u32 transformedPID;

    u16 disabledMove;
    u16 bindingMove;
    u16 encoredMove;
    u16 encoredMoveSlot;
    u16 lastResortMoves[4];
    u16 choiceLockedMove;
    u16 transformedGender;

    int itemHPRecovery;
} MoveEffectsData;

typedef struct BattleMon {
    u16 species;
    u16 atk;
    u16 def;
    u16 speed;
    u16 spAtk;
    u16 spDef;
    u16 moves[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 hasNickname : 1;
    s8 statChanges[8];
    int weight;
    u8 type1;
    u8 type2;
    u8 form : 5;
    u8 shiny : 1;
    u8 unk26_6 : 2;
    u8 ability;
    u32 sendOutFlag : 1;
    u32 intimidateFlag : 1;
    u32 traceFlag : 1;
    u32 downloadFlag : 1;
    u32 anticipationFlag : 1;
    u32 forewarnFlag : 1;
    u32 slowStartFlag : 1;
    u32 slowStartEnded : 1;
    u32 friskFlag : 1;
    u32 moldBreakerFlag : 1;
    u32 pressureFlag : 1;
    u32 unk28_B : 21;
    u8 movePPCur[4];
    u8 movePP[4];
    u8 level;
    u8 friendship;
    u16 nickname[11];
    s32 hp;
    u32 maxHp;
    u16 otName[7 + 1];
    u32 exp;
    u32 personality;
    u32 status;
    u32 status2;
    u32 otid;
    u16 item;
    u16 unk76;
    u8 hitCount;
    u8 msgFlag;
    u8 gender : 4;
    u8 otGender : 4;
    u8 ball;
    u32 moveEffectFlags;
    u32 moveEffectFlagsTemp;
    MoveEffectsData unk88;
} BattleMon;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct BattleContext BattleContext;

typedef struct BattleMessage {
    u8 commandCode;
    u8 tags;
    u16 id;
    int params[6];
    int digits;
    int battler;
} BattleMessage;

typedef struct FieldConditions {
    u32 weatherTurns;
    u8 futureSightTurns[4];
    u8 wishTurns[4];
    u16 futureSightMove[4];
    int futureSightAttacker[4];
    s32 futureSightDamage[4];
    u8 wishTarget[4];
} FieldConditions;

typedef struct SideConditions {
    u32 reflectUser : 2;
    u32 reflectTurns : 3;
    u32 lightScreenUser : 2;
    u32 lightScreenTurns : 3;
    u32 mistUser : 2;
    u32 mistTurns : 3;
    u32 safeguardUser : 2;
    u32 safeguardTurns : 3;
    u32 followMe : 1;
    u32 followMeUser : 2;
    u32 knockedOffItemsMask : 6;
    u32 padding00_1D : 3;

    u32 spikesLayers : 2;
    u32 toxicSpikesLayers : 2;
    u32 padding04_04 : 28;
} SideConditions;

typedef struct TurnFlags {
    u32 struggling : 1;
    u32 ppDecremented : 1;
    u32 protecting : 1;
    u32 helpingHand : 1;
    u32 magicCoat : 1;
    u32 snatching : 1;
    u32 roosting : 1;
    u32 fleeing : 2;
    u32 enduring : 1;
    u32 padding00_0A : 22;

    int physicalDamageTakenFrom[4];
    int physicalDamageLastAttacker;
    int physicalDamageAttackerMask;

    int specialDamageTakenFrom[4];
    int specialDamageLastAttacker;
    int specialDamageAttackerMask;

    int lastDamageTaken;
    int lastAttacker;
    int assuranceDamageMask;
} TurnFlags;

typedef struct SelfTurnFlags {
    u32 skipPressureCheck : 1;
    u32 lightningRodActivated : 1;
    u32 stormDrainActivated : 1;
    u32 moldBreakerActivated : 1;
    u32 trickRoomActivated : 1;
    u32 focusItemActivated : 1;
    u32 repeatedMoveCount : 3;
    u32 padding00_09 : 23;

    int physicalDamageTaken;
    int physicalDamageLastAttacker;
    int specialDamageTaken;
    int specialDamageLastAttacker;
    int statusFlags;
    int shellBellDamageDealt;
} SelfTurnFlags;

typedef struct MoveFailFlags {
    u32 paralysis : 1;
    u32 noEffect : 1;
    u32 imprison : 1;
    u32 infatuation : 1;
    u32 disabled : 1;
    u32 unk0_5 : 1;
    u32 flinch : 1;
    u32 confusion : 1;
    u32 gravity : 1;
    u32 healBlock : 1;
    u32 unused : 21;
} MoveFailFlags;

typedef struct BattleScriptTaskData {
    BattleSystem *battleSys;
    BattleContext *battleCtx;
    BallRotation *ballRotation;
    ManagedSprite *sprites[2];
    TextOBJ *fontOAM;
    CharTransferAllocation charTransferAllocation;
    int flag;
    int seqNum;
    int ball;
    int tmpData[8];
    void *tmpPtr[2];
} BattleScriptTaskData;

struct BattleContext {
    u8 curCommandState[4];
    u8 nextCommandState[4];

    int command;
    int commandNext;

    int fieldConditionCheckState;
    int fieldConditionCheckTemp;
    int monConditionCheckState;
    int monConditionCheckTemp;
    int sideConditionCheckState;
    int sideConditionCheckTemp;
    int turnStartCheckState;
    int turnStartCheckTemp;
    int afterMoveHitCheckState;
    int afterMoveHitCheckTemp;
    int afterMoveMessageType;
    int afterMoveMessageState;
    int afterMoveEffectState;

    int afterMoveEffectTemp;
    int beforeMoveCheckState;
    int tryMoveCheckState;
    int statusCheckState;
    int abilityCheckState;
    int switchInCheckState;
    int vanishedCheckTemp;

    int padding0060;

    int attacker;
    int attackerTemp;
    int defender;
    int defenderTemp;
    int faintedMon;
    int switchedMon;
    int switchedMonTemp;
    int abilityMon;
    int magicCoatMon;
    int sideEffectType;
    int sideEffectParam;
    int sideEffectFlags;
    int sideEffectMon;
    int lastBattlerId;

    int gainedExp;
    int sharedExp;
    u32 sideGetExpMask[2];

    enum NarcId scriptNarc;
    int scriptFile;
    int scriptCursor;
    int scriptStackPointer;
    enum NarcId scriptStackNarc[4];
    int scriptStackFile[4];
    int scriptStackCursor[4];

    int turnOrderCounter;
    int waitCounter;

    BattleMessage msgBuffer;
    int msgBattlerTemp;
    int msgAttacker;
    int msgDefender;
    int msgMoveTemp;
    int msgItemTemp;
    int msgAbilityTemp;
    int msgTemp;
    int calcTemp;
    int scriptTemp;

    u32 battlerStatusFlags[4];
    u32 payDayCount;
    int totalTurns;
    int totalFainted[4];
    int totalDamage[4];
    int meFirstTurnOrder;

    BattleScriptTaskData *taskData;
    void *tmpData;

    u32 fieldConditionsMask;
    FieldConditions fieldConditions;

    u32 sideConditionsMask[2];
    SideConditions sideConditions[2];

    TurnFlags turnFlags[4];
    SelfTurnFlags selfTurnFlags[4];
    MoveFailFlags moveFailFlags[4];

    AIContext aiContext;
    u32 *aiScriptTemp;
    u32 aiScriptCursor;

    u32 battleStatusMask;
    u32 battleStatusMask2;

    int damage;
    int hitDamage;
    int criticalBoosts;
    int criticalMul;
    int movePower;
    int powerMul;
    int hpCalcTemp;
    int moveType;
    int moveEffectChance;
    int prizeMoneyMul;
    u32 moveStatusFlags;
    u32 sideEffectDirectFlags;
    u32 sideEffectIndirectFlags;
    u32 sideEffectAbilityFlags;
    u8 multiHitCounter;
    u8 multiHitNumHits;
    u8 battlerCounter;
    u8 beatUpCounter;

    BOOL multiHitLoop;
    u32 multiHitCheckFlags;
    u32 multiHitAccuracyCheck;

    u32 clearVolatileStatus[4];

    u8 selectedPartySlot[4];
    u8 switchedPartySlot[4];
    u8 aiSwitchedPartySlot[4];

    u32 battlerActions[4][MAX_BATTLE_ACTIONS];
    u8 battlerActionOrder[4];
    u8 monSpeedOrder[4];
    u32 monSpeedValues[4];

    u8 ioQueue[4][4][16];
    u8 ioBuffer[4][256];
    int battleScript[400];

    BattleMon battleMons[4];

    u32 moveTemp;
    u32 moveCur;
    u32 movePrev;
    u32 moveLockedInto[4];
    u16 moveProtect[4];
    u16 moveHit[4];
    u16 moveHitBattler[4];
    u16 moveHitType[4];
    u16 movePrevByBattler[4];
    u16 moveCopied[4];
    u16 moveCopiedHit[4][4];
    u16 moveSketched[4];
    u16 moveSelected[4];
    u16 moveSlot[4];
    u16 conversion2Move[4];
    u16 conversion2Battler[4];
    u16 conversion2Type[4];
    u16 metronomeMove[4];

    int storedDamage[4];
    int lastHitByBattler[4];
    int battlerSpeedTemp;
    u8 battlersSwitchingMask;
    u8 levelUpMons;
    u16 padding310A;

    u16 speedRand[4];

    int flingTemp;
    int flingScript;

    u8 safariCatchStage;
    u8 safariEscapeCount;
    u8 runAttempts;
    u8 battleEndFlag;

    u8 magnitude;
    u8 fieldWeatherChecked;
    s16 hpTemp;

    u16 recycleItem[4];

    u8 partyOrder[4][6];

    BOOL expJinglePlayed;

    int linkBattleTimeout;

    u8 recordedCommandFlags[4];

    int waitingBattlers;

    u32 battleProgressFlag : 1;
    u32 padding3154_01 : 31;
};

enum Species {
    SPECIES_NONE = 0,
    SPECIES_BULBASAUR = 1,
    SPECIES_IVYSAUR = 2,
    SPECIES_VENUSAUR = 3,
    SPECIES_CHARMANDER = 4,
    SPECIES_CHARMELEON = 5,
    SPECIES_CHARIZARD = 6,
    SPECIES_SQUIRTLE = 7,
    SPECIES_WARTORTLE = 8,
    SPECIES_BLASTOISE = 9,
    SPECIES_CATERPIE = 10,
    SPECIES_METAPOD = 11,
    SPECIES_BUTTERFREE = 12,
    SPECIES_WEEDLE = 13,
    SPECIES_KAKUNA = 14,
    SPECIES_BEEDRILL = 15,
    SPECIES_PIDGEY = 16,
    SPECIES_PIDGEOTTO = 17,
    SPECIES_PIDGEOT = 18,
    SPECIES_RATTATA = 19,
    SPECIES_RATICATE = 20,
    SPECIES_SPEAROW = 21,
    SPECIES_FEAROW = 22,
    SPECIES_EKANS = 23,
    SPECIES_ARBOK = 24,
    SPECIES_PIKACHU = 25,
    SPECIES_RAICHU = 26,
    SPECIES_SANDSHREW = 27,
    SPECIES_SANDSLASH = 28,
    SPECIES_NIDORAN_F = 29,
    SPECIES_NIDORINA = 30,
    SPECIES_NIDOQUEEN = 31,
    SPECIES_NIDORAN_M = 32,
    SPECIES_NIDORINO = 33,
    SPECIES_NIDOKING = 34,
    SPECIES_CLEFAIRY = 35,
    SPECIES_CLEFABLE = 36,
    SPECIES_VULPIX = 37,
    SPECIES_NINETALES = 38,
    SPECIES_JIGGLYPUFF = 39,
    SPECIES_WIGGLYTUFF = 40,
    SPECIES_ZUBAT = 41,
    SPECIES_GOLBAT = 42,
    SPECIES_ODDISH = 43,
    SPECIES_GLOOM = 44,
    SPECIES_VILEPLUME = 45,
    SPECIES_PARAS = 46,
    SPECIES_PARASECT = 47,
    SPECIES_VENONAT = 48,
    SPECIES_VENOMOTH = 49,
    SPECIES_DIGLETT = 50,
    SPECIES_DUGTRIO = 51,
    SPECIES_MEOWTH = 52,
    SPECIES_PERSIAN = 53,
    SPECIES_PSYDUCK = 54,
    SPECIES_GOLDUCK = 55,
    SPECIES_MANKEY = 56,
    SPECIES_PRIMEAPE = 57,
    SPECIES_GROWLITHE = 58,
    SPECIES_ARCANINE = 59,
    SPECIES_POLIWAG = 60,
    SPECIES_POLIWHIRL = 61,
    SPECIES_POLIWRATH = 62,
    SPECIES_ABRA = 63,
    SPECIES_KADABRA = 64,
    SPECIES_ALAKAZAM = 65,
    SPECIES_MACHOP = 66,
    SPECIES_MACHOKE = 67,
    SPECIES_MACHAMP = 68,
    SPECIES_BELLSPROUT = 69,
    SPECIES_WEEPINBELL = 70,
    SPECIES_VICTREEBEL = 71,
    SPECIES_TENTACOOL = 72,
    SPECIES_TENTACRUEL = 73,
    SPECIES_GEODUDE = 74,
    SPECIES_GRAVELER = 75,
    SPECIES_GOLEM = 76,
    SPECIES_PONYTA = 77,
    SPECIES_RAPIDASH = 78,
    SPECIES_SLOWPOKE = 79,
    SPECIES_SLOWBRO = 80,
    SPECIES_MAGNEMITE = 81,
    SPECIES_MAGNETON = 82,
    SPECIES_FARFETCHD = 83,
    SPECIES_DODUO = 84,
    SPECIES_DODRIO = 85,
    SPECIES_SEEL = 86,
    SPECIES_DEWGONG = 87,
    SPECIES_GRIMER = 88,
    SPECIES_MUK = 89,
    SPECIES_SHELLDER = 90,
    SPECIES_CLOYSTER = 91,
    SPECIES_GASTLY = 92,
    SPECIES_HAUNTER = 93,
    SPECIES_GENGAR = 94,
    SPECIES_ONIX = 95,
    SPECIES_DROWZEE = 96,
    SPECIES_HYPNO = 97,
    SPECIES_KRABBY = 98,
    SPECIES_KINGLER = 99,
    SPECIES_VOLTORB = 100,
    SPECIES_ELECTRODE = 101,
    SPECIES_EXEGGCUTE = 102,
    SPECIES_EXEGGUTOR = 103,
    SPECIES_CUBONE = 104,
    SPECIES_MAROWAK = 105,
    SPECIES_HITMONLEE = 106,
    SPECIES_HITMONCHAN = 107,
    SPECIES_LICKITUNG = 108,
    SPECIES_KOFFING = 109,
    SPECIES_WEEZING = 110,
    SPECIES_RHYHORN = 111,
    SPECIES_RHYDON = 112,
    SPECIES_CHANSEY = 113,
    SPECIES_TANGELA = 114,
    SPECIES_KANGASKHAN = 115,
    SPECIES_HORSEA = 116,
    SPECIES_SEADRA = 117,
    SPECIES_GOLDEEN = 118,
    SPECIES_SEAKING = 119,
    SPECIES_STARYU = 120,
    SPECIES_STARMIE = 121,
    SPECIES_MR_MIME = 122,
    SPECIES_SCYTHER = 123,
    SPECIES_JYNX = 124,
    SPECIES_ELECTABUZZ = 125,
    SPECIES_MAGMAR = 126,
    SPECIES_PINSIR = 127,
    SPECIES_TAUROS = 128,
    SPECIES_MAGIKARP = 129,
    SPECIES_GYARADOS = 130,
    SPECIES_LAPRAS = 131,
    SPECIES_DITTO = 132,
    SPECIES_EEVEE = 133,
    SPECIES_VAPOREON = 134,
    SPECIES_JOLTEON = 135,
    SPECIES_FLAREON = 136,
    SPECIES_PORYGON = 137,
    SPECIES_OMANYTE = 138,
    SPECIES_OMASTAR = 139,
    SPECIES_KABUTO = 140,
    SPECIES_KABUTOPS = 141,
    SPECIES_AERODACTYL = 142,
    SPECIES_SNORLAX = 143,
    SPECIES_ARTICUNO = 144,
    SPECIES_ZAPDOS = 145,
    SPECIES_MOLTRES = 146,
    SPECIES_DRATINI = 147,
    SPECIES_DRAGONAIR = 148,
    SPECIES_DRAGONITE = 149,
    SPECIES_MEWTWO = 150,
    SPECIES_MEW = 151,
    SPECIES_CHIKORITA = 152,
    SPECIES_BAYLEEF = 153,
    SPECIES_MEGANIUM = 154,
    SPECIES_CYNDAQUIL = 155,
    SPECIES_QUILAVA = 156,
    SPECIES_TYPHLOSION = 157,
    SPECIES_TOTODILE = 158,
    SPECIES_CROCONAW = 159,
    SPECIES_FERALIGATR = 160,
    SPECIES_SENTRET = 161,
    SPECIES_FURRET = 162,
    SPECIES_HOOTHOOT = 163,
    SPECIES_NOCTOWL = 164,
    SPECIES_LEDYBA = 165,
    SPECIES_LEDIAN = 166,
    SPECIES_SPINARAK = 167,
    SPECIES_ARIADOS = 168,
    SPECIES_CROBAT = 169,
    SPECIES_CHINCHOU = 170,
    SPECIES_LANTURN = 171,
    SPECIES_PICHU = 172,
    SPECIES_CLEFFA = 173,
    SPECIES_IGGLYBUFF = 174,
    SPECIES_TOGEPI = 175,
    SPECIES_TOGETIC = 176,
    SPECIES_NATU = 177,
    SPECIES_XATU = 178,
    SPECIES_MAREEP = 179,
    SPECIES_FLAAFFY = 180,
    SPECIES_AMPHAROS = 181,
    SPECIES_BELLOSSOM = 182,
    SPECIES_MARILL = 183,
    SPECIES_AZUMARILL = 184,
    SPECIES_SUDOWOODO = 185,
    SPECIES_POLITOED = 186,
    SPECIES_HOPPIP = 187,
    SPECIES_SKIPLOOM = 188,
    SPECIES_JUMPLUFF = 189,
    SPECIES_AIPOM = 190,
    SPECIES_SUNKERN = 191,
    SPECIES_SUNFLORA = 192,
    SPECIES_YANMA = 193,
    SPECIES_WOOPER = 194,
    SPECIES_QUAGSIRE = 195,
    SPECIES_ESPEON = 196,
    SPECIES_UMBREON = 197,
    SPECIES_MURKROW = 198,
    SPECIES_SLOWKING = 199,
    SPECIES_MISDREAVUS = 200,
    SPECIES_UNOWN = 201,
    SPECIES_WOBBUFFET = 202,
    SPECIES_GIRAFARIG = 203,
    SPECIES_PINECO = 204,
    SPECIES_FORRETRESS = 205,
    SPECIES_DUNSPARCE = 206,
    SPECIES_GLIGAR = 207,
    SPECIES_STEELIX = 208,
    SPECIES_SNUBBULL = 209,
    SPECIES_GRANBULL = 210,
    SPECIES_QWILFISH = 211,
    SPECIES_SCIZOR = 212,
    SPECIES_SHUCKLE = 213,
    SPECIES_HERACROSS = 214,
    SPECIES_SNEASEL = 215,
    SPECIES_TEDDIURSA = 216,
    SPECIES_URSARING = 217,
    SPECIES_SLUGMA = 218,
    SPECIES_MAGCARGO = 219,
    SPECIES_SWINUB = 220,
    SPECIES_PILOSWINE = 221,
    SPECIES_CORSOLA = 222,
    SPECIES_REMORAID = 223,
    SPECIES_OCTILLERY = 224,
    SPECIES_DELIBIRD = 225,
    SPECIES_MANTINE = 226,
    SPECIES_SKARMORY = 227,
    SPECIES_HOUNDOUR = 228,
    SPECIES_HOUNDOOM = 229,
    SPECIES_KINGDRA = 230,
    SPECIES_PHANPY = 231,
    SPECIES_DONPHAN = 232,
    SPECIES_PORYGON2 = 233,
    SPECIES_STANTLER = 234,
    SPECIES_SMEARGLE = 235,
    SPECIES_TYROGUE = 236,
    SPECIES_HITMONTOP = 237,
    SPECIES_SMOOCHUM = 238,
    SPECIES_ELEKID = 239,
    SPECIES_MAGBY = 240,
    SPECIES_MILTANK = 241,
    SPECIES_BLISSEY = 242,
    SPECIES_RAIKOU = 243,
    SPECIES_ENTEI = 244,
    SPECIES_SUICUNE = 245,
    SPECIES_LARVITAR = 246,
    SPECIES_PUPITAR = 247,
    SPECIES_TYRANITAR = 248,
    SPECIES_LUGIA = 249,
    SPECIES_HO_OH = 250,
    SPECIES_CELEBI = 251,
    SPECIES_TREECKO = 252,
    SPECIES_GROVYLE = 253,
    SPECIES_SCEPTILE = 254,
    SPECIES_TORCHIC = 255,
    SPECIES_COMBUSKEN = 256,
    SPECIES_BLAZIKEN = 257,
    SPECIES_MUDKIP = 258,
    SPECIES_MARSHTOMP = 259,
    SPECIES_SWAMPERT = 260,
    SPECIES_POOCHYENA = 261,
    SPECIES_MIGHTYENA = 262,
    SPECIES_ZIGZAGOON = 263,
    SPECIES_LINOONE = 264,
    SPECIES_WURMPLE = 265,
    SPECIES_SILCOON = 266,
    SPECIES_BEAUTIFLY = 267,
    SPECIES_CASCOON = 268,
    SPECIES_DUSTOX = 269,
    SPECIES_LOTAD = 270,
    SPECIES_LOMBRE = 271,
    SPECIES_LUDICOLO = 272,
    SPECIES_SEEDOT = 273,
    SPECIES_NUZLEAF = 274,
    SPECIES_SHIFTRY = 275,
    SPECIES_TAILLOW = 276,
    SPECIES_SWELLOW = 277,
    SPECIES_WINGULL = 278,
    SPECIES_PELIPPER = 279,
    SPECIES_RALTS = 280,
    SPECIES_KIRLIA = 281,
    SPECIES_GARDEVOIR = 282,
    SPECIES_SURSKIT = 283,
    SPECIES_MASQUERAIN = 284,
    SPECIES_SHROOMISH = 285,
    SPECIES_BRELOOM = 286,
    SPECIES_SLAKOTH = 287,
    SPECIES_VIGOROTH = 288,
    SPECIES_SLAKING = 289,
    SPECIES_NINCADA = 290,
    SPECIES_NINJASK = 291,
    SPECIES_SHEDINJA = 292,
    SPECIES_WHISMUR = 293,
    SPECIES_LOUDRED = 294,
    SPECIES_EXPLOUD = 295,
    SPECIES_MAKUHITA = 296,
    SPECIES_HARIYAMA = 297,
    SPECIES_AZURILL = 298,
    SPECIES_NOSEPASS = 299,
    SPECIES_SKITTY = 300,
    SPECIES_DELCATTY = 301,
    SPECIES_SABLEYE = 302,
    SPECIES_MAWILE = 303,
    SPECIES_ARON = 304,
    SPECIES_LAIRON = 305,
    SPECIES_AGGRON = 306,
    SPECIES_MEDITITE = 307,
    SPECIES_MEDICHAM = 308,
    SPECIES_ELECTRIKE = 309,
    SPECIES_MANECTRIC = 310,
    SPECIES_PLUSLE = 311,
    SPECIES_MINUN = 312,
    SPECIES_VOLBEAT = 313,
    SPECIES_ILLUMISE = 314,
    SPECIES_ROSELIA = 315,
    SPECIES_GULPIN = 316,
    SPECIES_SWALOT = 317,
    SPECIES_CARVANHA = 318,
    SPECIES_SHARPEDO = 319,
    SPECIES_WAILMER = 320,
    SPECIES_WAILORD = 321,
    SPECIES_NUMEL = 322,
    SPECIES_CAMERUPT = 323,
    SPECIES_TORKOAL = 324,
    SPECIES_SPOINK = 325,
    SPECIES_GRUMPIG = 326,
    SPECIES_SPINDA = 327,
    SPECIES_TRAPINCH = 328,
    SPECIES_VIBRAVA = 329,
    SPECIES_FLYGON = 330,
    SPECIES_CACNEA = 331,
    SPECIES_CACTURNE = 332,
    SPECIES_SWABLU = 333,
    SPECIES_ALTARIA = 334,
    SPECIES_ZANGOOSE = 335,
    SPECIES_SEVIPER = 336,
    SPECIES_LUNATONE = 337,
    SPECIES_SOLROCK = 338,
    SPECIES_BARBOACH = 339,
    SPECIES_WHISCASH = 340,
    SPECIES_CORPHISH = 341,
    SPECIES_CRAWDAUNT = 342,
    SPECIES_BALTOY = 343,
    SPECIES_CLAYDOL = 344,
    SPECIES_LILEEP = 345,
    SPECIES_CRADILY = 346,
    SPECIES_ANORITH = 347,
    SPECIES_ARMALDO = 348,
    SPECIES_FEEBAS = 349,
    SPECIES_MILOTIC = 350,
    SPECIES_CASTFORM = 351,
    SPECIES_KECLEON = 352,
    SPECIES_SHUPPET = 353,
    SPECIES_BANETTE = 354,
    SPECIES_DUSKULL = 355,
    SPECIES_DUSCLOPS = 356,
    SPECIES_TROPIUS = 357,
    SPECIES_CHIMECHO = 358,
    SPECIES_ABSOL = 359,
    SPECIES_WYNAUT = 360,
    SPECIES_SNORUNT = 361,
    SPECIES_GLALIE = 362,
    SPECIES_SPHEAL = 363,
    SPECIES_SEALEO = 364,
    SPECIES_WALREIN = 365,
    SPECIES_CLAMPERL = 366,
    SPECIES_HUNTAIL = 367,
    SPECIES_GOREBYSS = 368,
    SPECIES_RELICANTH = 369,
    SPECIES_LUVDISC = 370,
    SPECIES_BAGON = 371,
    SPECIES_SHELGON = 372,
    SPECIES_SALAMENCE = 373,
    SPECIES_BELDUM = 374,
    SPECIES_METANG = 375,
    SPECIES_METAGROSS = 376,
    SPECIES_REGIROCK = 377,
    SPECIES_REGICE = 378,
    SPECIES_REGISTEEL = 379,
    SPECIES_LATIAS = 380,
    SPECIES_LATIOS = 381,
    SPECIES_KYOGRE = 382,
    SPECIES_GROUDON = 383,
    SPECIES_RAYQUAZA = 384,
    SPECIES_JIRACHI = 385,
    SPECIES_DEOXYS = 386,
    SPECIES_TURTWIG = 387,
    SPECIES_GROTLE = 388,
    SPECIES_TORTERRA = 389,
    SPECIES_CHIMCHAR = 390,
    SPECIES_MONFERNO = 391,
    SPECIES_INFERNAPE = 392,
    SPECIES_PIPLUP = 393,
    SPECIES_PRINPLUP = 394,
    SPECIES_EMPOLEON = 395,
    SPECIES_STARLY = 396,
    SPECIES_STARAVIA = 397,
    SPECIES_STARAPTOR = 398,
    SPECIES_BIDOOF = 399,
    SPECIES_BIBAREL = 400,
    SPECIES_KRICKETOT = 401,
    SPECIES_KRICKETUNE = 402,
    SPECIES_SHINX = 403,
    SPECIES_LUXIO = 404,
    SPECIES_LUXRAY = 405,
    SPECIES_BUDEW = 406,
    SPECIES_ROSERADE = 407,
    SPECIES_CRANIDOS = 408,
    SPECIES_RAMPARDOS = 409,
    SPECIES_SHIELDON = 410,
    SPECIES_BASTIODON = 411,
    SPECIES_BURMY = 412,
    SPECIES_WORMADAM = 413,
    SPECIES_MOTHIM = 414,
    SPECIES_COMBEE = 415,
    SPECIES_VESPIQUEN = 416,
    SPECIES_PACHIRISU = 417,
    SPECIES_BUIZEL = 418,
    SPECIES_FLOATZEL = 419,
    SPECIES_CHERUBI = 420,
    SPECIES_CHERRIM = 421,
    SPECIES_SHELLOS = 422,
    SPECIES_GASTRODON = 423,
    SPECIES_AMBIPOM = 424,
    SPECIES_DRIFLOON = 425,
    SPECIES_DRIFBLIM = 426,
    SPECIES_BUNEARY = 427,
    SPECIES_LOPUNNY = 428,
    SPECIES_MISMAGIUS = 429,
    SPECIES_HONCHKROW = 430,
    SPECIES_GLAMEOW = 431,
    SPECIES_PURUGLY = 432,
    SPECIES_CHINGLING = 433,
    SPECIES_STUNKY = 434,
    SPECIES_SKUNTANK = 435,
    SPECIES_BRONZOR = 436,
    SPECIES_BRONZONG = 437,
    SPECIES_BONSLY = 438,
    SPECIES_MIME_JR = 439,
    SPECIES_HAPPINY = 440,
    SPECIES_CHATOT = 441,
    SPECIES_SPIRITOMB = 442,
    SPECIES_GIBLE = 443,
    SPECIES_GABITE = 444,
    SPECIES_GARCHOMP = 445,
    SPECIES_MUNCHLAX = 446,
    SPECIES_RIOLU = 447,
    SPECIES_LUCARIO = 448,
    SPECIES_HIPPOPOTAS = 449,
    SPECIES_HIPPOWDON = 450,
    SPECIES_SKORUPI = 451,
    SPECIES_DRAPION = 452,
    SPECIES_CROAGUNK = 453,
    SPECIES_TOXICROAK = 454,
    SPECIES_CARNIVINE = 455,
    SPECIES_FINNEON = 456,
    SPECIES_LUMINEON = 457,
    SPECIES_MANTYKE = 458,
    SPECIES_SNOVER = 459,
    SPECIES_ABOMASNOW = 460,
    SPECIES_WEAVILE = 461,
    SPECIES_MAGNEZONE = 462,
    SPECIES_LICKILICKY = 463,
    SPECIES_RHYPERIOR = 464,
    SPECIES_TANGROWTH = 465,
    SPECIES_ELECTIVIRE = 466,
    SPECIES_MAGMORTAR = 467,
    SPECIES_TOGEKISS = 468,
    SPECIES_YANMEGA = 469,
    SPECIES_LEAFEON = 470,
    SPECIES_GLACEON = 471,
    SPECIES_GLISCOR = 472,
    SPECIES_MAMOSWINE = 473,
    SPECIES_PORYGON_Z = 474,
    SPECIES_GALLADE = 475,
    SPECIES_PROBOPASS = 476,
    SPECIES_DUSKNOIR = 477,
    SPECIES_FROSLASS = 478,
    SPECIES_ROTOM = 479,
    SPECIES_UXIE = 480,
    SPECIES_MESPRIT = 481,
    SPECIES_AZELF = 482,
    SPECIES_DIALGA = 483,
    SPECIES_PALKIA = 484,
    SPECIES_HEATRAN = 485,
    SPECIES_REGIGIGAS = 486,
    SPECIES_GIRATINA = 487,
    SPECIES_CRESSELIA = 488,
    SPECIES_PHIONE = 489,
    SPECIES_MANAPHY = 490,
    SPECIES_DARKRAI = 491,
    SPECIES_SHAYMIN = 492,
    SPECIES_ARCEUS = 493,
    SPECIES_EGG = 494,
    SPECIES_BAD_EGG = 495,
};

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

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
    MailMessage sentences[3];
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

u32 GetMonData(Pokemon *mon, enum PokemonDataParam param, void *dest);

enum CountAbilityMode {
    COUNT_ALL_BATTLERS_OUR_SIDE = 0,
    COUNT_ALIVE_BATTLERS_OUR_SIDE,
    COUNT_ALL_BATTLERS_THEIR_SIDE,
    COUNT_ALIVE_BATTLERS_THEIR_SIDE,
    COUNT_ALIVE_BATTLERS_THEIR_SIDE_FLAG,
    COUNT_ALL_BATTLERS,
    COUNT_ALL_BATTLERS_EXCEPT_ME,
    COUNT_ALL_BATTLERS_EXCEPT_ME_RETURN_BATTLER,
    COUNT_ALIVE_BATTLERS,
    COUNT_ALIVE_BATTLERS_EXCEPT_ME,
};

int BattleMon_Get(BattleContext *battleCtx, int battler, enum BattleMonParam paramID, void *buf);

int ov12_02251D28(BattleSystem *battleSys, BattleContext *battleCtx, int move, int inType, int attacker, int defender, int damage, u32 *moveStatusMask);

void ov12_02252054(BattleContext *battleCtx, int move, int inType, int attackerAbility, int defenderAbility, int defenderItemEffect, int defenderType1, int defenderType2, u32 *moveStatusMask);

int CalculateTypeEffectiveness(u8 attackingType, u8 defendingType1, u8 defendingType2);

u8 GetBattlerAbility(BattleContext *battleCtx, int battler);

int Battler_GetRandomOpposingBattlerId(BattleSystem *battleSys, BattleContext *battleCtx, int attacker);

s32 GetBattlerHeldItemEffect(BattleContext *battleCtx, int battler);

enum HeldItemPowerOp {
    ITEM_POWER_CHECK_ALL = 0,
    ITEM_POWER_CHECK_NONE,
    ITEM_POWER_CHECK_EMBARGO,
};

int CalcMoveDamage(BattleSystem *battleSys,
    BattleContext *battleCtx,
    int move,
    u32 sideConditions,
    u32 fieldConditions,
    u16 inPower,
    u8 inType,
    u8 attacker,
    u8 defender,
    u8 criticalMul);

int ov12_02258BB4(BattleSystem *battleSys, BattleContext *battleCtx, Pokemon *mon, int move);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

BattleContext *BattleSystem_GetBattleContext(BattleSystem *battleSys);

int BattleSystem_GetPartySize(BattleSystem *battleSys, int battler);

Pokemon *BattleSystem_GetPartyMon(BattleSystem *battleSys, int battler, int slot);

u8 BattleSystem_GetFieldSide(BattleSystem *battleSys, int battler);

int BattleSystem_GetBattlerIdPartner(BattleSystem *battleSys, int battler);

u32 MaskOfFlagNo(int index);

void SetBattlerVar(BattleContext *battleCtx, int battler, enum BattleMonParam paramID, const void *buf);

void AddBattlerVar(BattleContext *battleCtx, int battler, enum BattleMonParam paramID, int val);

int CheckAbilityActive(BattleSystem *battleSys, BattleContext *battleCtx, enum CountAbilityMode mode, int battler, int ability);

s32 GetHeldItemModifier(BattleContext *battleCtx, int battler, enum HeldItemPowerOp opcode);

s32 GetItemVar(BattleContext *battleCtx, u16 item, u16 paramID);

int ov12_022581D4(BattleSystem *battleSys, BattleContext *battleCtx, enum BattleContextParam paramID, int battler);

void ov12_022582B8(BattleSystem *battleSys, BattleContext *battleCtx, enum BattleContextParam paramID, int battler, int val);

int GetMoveStatusChangeScript(BattleContext *battleCtx, enum BattleSideEffectType type, u32 effect);

int ov12_02258800(BattleSystem *battleSys, int battler);

int ov12_02258800(BattleSystem *battleSys, int battler) {

    int i, j;
    u8 defender, defenderType1, defenderType2;
    u8 monType1, monType2;
    u16 monSpecies;
    u16 move;
    int moveType;
    u8 battlersDisregarded;
    u8 score, maxScore;
    u8 picked = 6;
    u8 slot1, slot2;
    u32 moveStatusFlags;
    int partySize;
    Pokemon *mon;
    BattleContext *battleCtx = BattleSystem_GetBattleContext(battleSys);

    slot1 = battler;
    if ((BattleSystem_GetBattleType(battleSys) & (1 << 4))
        || (BattleSystem_GetBattleType(battleSys) & (1 << 3))) {
        slot2 = slot1;
    } else {
        slot2 = BattleSystem_GetBattlerIdPartner(battleSys, battler);
    }

    defender = Battler_GetRandomOpposingBattlerId(battleSys, battleCtx, battler);
    partySize = BattleSystem_GetPartySize(battleSys, battler);
    battlersDisregarded = 0;

    while (battlersDisregarded != 0x3F) {
        maxScore = 0;
        picked = 6;

        for (i = 0; i < partySize; i++) {
            mon = BattleSystem_GetPartyMon(battleSys, battler, i);
            monSpecies = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, 0L);

            if (monSpecies != SPECIES_NONE
                && monSpecies != SPECIES_EGG
                && GetMonData(mon, MON_DATA_HP, 0L)
                && (battlersDisregarded & MaskOfFlagNo(i)) == 0
                && battleCtx->selectedPartySlot[slot1] != i
                && battleCtx->selectedPartySlot[slot2] != i
                && i != battleCtx->aiSwitchedPartySlot[slot1]
                && i != battleCtx->aiSwitchedPartySlot[slot2]) {
                defenderType1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, 0L);
                defenderType2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, 0L);
                monType1 = GetMonData(mon, MON_DATA_TYPE_1, 0L);
                monType2 = GetMonData(mon, MON_DATA_TYPE_2, 0L);

                score = CalculateTypeEffectiveness(monType1, defenderType1, defenderType2);
                score += CalculateTypeEffectiveness(monType2, defenderType1, defenderType2);

                if (maxScore < score) {
                    maxScore = score;
                    picked = i;
                }
            } else {
                battlersDisregarded |= MaskOfFlagNo(i);
            }
        }

        if (picked != 6) {

            mon = BattleSystem_GetPartyMon(battleSys, battler, picked);

            for (i = 0; i < 4; i++) {
                move = GetMonData((struct Pokemon *)(mon), (enum PokemonDataParam)(MON_DATA_MOVE1 + i), 0L);
                moveType = ov12_02258BB4(battleSys, battleCtx, mon, move);

                if (move) {
                    moveStatusFlags = 0;
                    ov12_02252054(battleCtx,
                        move,
                        moveType,
                        GetMonData(mon, MON_DATA_ABILITY, 0L),
                        GetBattlerAbility(battleCtx, defender),
                        GetBattlerHeldItemEffect(battleCtx, defender),
                        BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, 0L),
                        BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, 0L),
                        &moveStatusFlags);

                    if (moveStatusFlags & (1 << 1)) {
                        break;
                    }
                }
            }

            if (i == 4) {
                battlersDisregarded |= MaskOfFlagNo(picked);
            } else {
                return picked;
            }
        } else {

            battlersDisregarded = 0x3F;
        }
    }

    maxScore = 0;
    picked = 6;

    for (i = 0; i < partySize; i++) {
        mon = BattleSystem_GetPartyMon(battleSys, battler, i);
        monSpecies = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, 0L);

        if (monSpecies != SPECIES_NONE
            && monSpecies != SPECIES_EGG
            && GetMonData(mon, MON_DATA_HP, 0L)
            && battleCtx->selectedPartySlot[slot1] != i
            && battleCtx->selectedPartySlot[slot2] != i
            && i != battleCtx->aiSwitchedPartySlot[slot1]
            && i != battleCtx->aiSwitchedPartySlot[slot2]) {
            for (j = 0; j < 4; j++) {
                move = GetMonData((struct Pokemon *)(mon), (enum PokemonDataParam)(MON_DATA_MOVE1 + j), 0L);
                moveType = ov12_02258BB4(battleSys, battleCtx, mon, move);

                if (move && (battleCtx->aiContext.moveTable[move]).power != 1) {
                    score = CalcMoveDamage(battleSys,
                        battleCtx,
                        move,
                        battleCtx->sideConditionsMask[BattleSystem_GetFieldSide(battleSys, defender)],
                        battleCtx->fieldConditionsMask,
                        0,
                        0,
                        battler,
                        defender,
                        1);

                    moveStatusFlags = 0;
                    score = ov12_02251D28(battleSys,
                        battleCtx,
                        move,
                        moveType,
                        battler,
                        defender,
                        score,
                        &moveStatusFlags);

                    if (moveStatusFlags & (((1 << 3) | (1 << 18)) | (1 << 11) | (1 << 20))) {
                        score = 0;
                    }
                }

                if (maxScore < score) {
                    maxScore = score;
                    picked = i;
                }
            }
        }
    }

    return picked;
}
