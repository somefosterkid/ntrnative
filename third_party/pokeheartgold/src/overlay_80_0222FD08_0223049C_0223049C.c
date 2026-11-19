#include "global.h"
#include "overlay_80_battle_factory.h"
#include "seal_case.h"
#include "bag_types_def.h"
#include "options.h"
#include "overlay_12_field_battle_dto.h"
#include "mail_message_types.h"
#include "sav_chatot.h"
#include "player_data.h"
#include "save_palpad.h"
#include "battle_castle_save.h"
#include "save_frontier.h"

struct Bag;

struct BagCursor;

struct BagItem;

struct BallCapsule;

struct BallSeal;

struct BattleArcadeSave;

struct BattleArcadeStreakFlags;

struct BattleBagCursor;

struct BattleCastlePersistentSave;

struct BattleCastleSave;

struct BattleFactory;

struct BattleFactorySave;

struct BattleFactoryStreakFlags;

struct FrontierSave;

struct BattleFrontierStats;

struct BattleHallSave;

struct BattleHallStreakFlags;

struct BattleRecords;

struct BattleTowerPartnerData;

struct BattleTowerSave;

struct BoxPokemon;

struct CaptureAttempt;

struct SOUND_CHATOT;

struct MailMessage;

struct EncodingSeed;

struct FieldBagCursor;

struct FieldBattleDTO;

struct FrontierEasyChatMessages;

struct FrontierOpponent;

struct FrontierPokemon;

struct FrontierTrainer;

struct GameRecords;

struct JournalEntry;

struct JournalEntryMon;

struct JournalEntryTitle;

struct JournalEntryTrainer;

struct Mail;

struct Options;

struct PCBoxes;

struct SavePalPad;

struct Party;

struct PartyPokemon;

struct Pokedex;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct Poketch;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct SysTask;

struct SysTaskQueue;

struct Trainer;

struct TrainerHeader;

struct PlayerProfile;

struct WiFiHistory;

struct WifiBattleTowerDownloadData;

struct WifiBattleTowerMatchCandidate;

struct WifiBattleTowerRecord;

struct WifiTrainerRecord;

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

enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef u16 charcode_t;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

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

typedef struct Party {
    struct { int maxCount; int curCount; Pokemon mons[6]; } core;
    struct { struct { u8 unk_00[5]; } aprijuiceModifiers[6]; } extra;
} Party;

int Party_GetCount(const Party *party);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

enum BattleBackground {
    BACKGROUND_PLAIN = 0,
    BACKGROUND_WATER,
    BACKGROUND_CITY,
    BACKGROUND_FOREST,
    BACKGROUND_MOUNTAIN,
    BACKGROUND_SNOW,
    BACKGROUND_INDOORS_1,
    BACKGROUND_INDOORS_2,
    BACKGROUND_INDOORS_3,
    BACKGROUND_CAVE_1,
    BACKGROUND_CAVE_2,
    BACKGROUND_CAVE_3,
    BACKGROUND_AARON,
    BACKGROUND_BERTHA,
    BACKGROUND_FLINT,
    BACKGROUND_LUCIAN,
    BACKGROUND_CYNTHIA,
    BACKGROUND_DISTORTION_WORLD,
    BACKGROUND_BATTLE_TOWER,
    BACKGROUND_BATTLE_FACTORY,
    BACKGROUND_BATTLE_ARCADE,
    BACKGROUND_BATTLE_CASTLE,
    BACKGROUND_BATTLE_HALL,
};

enum BattleTerrain {
    TERRAIN_PLAIN = 0,
    TERRAIN_SAND = 1,
    TERRAIN_GRASS = 2,
    TERRAIN_PUDDLE = 3,
    TERRAIN_MOUNTAIN = 4,
    TERRAIN_CAVE = 5,
    TERRAIN_SNOW = 6,
    TERRAIN_WATER = 7,
    TERRAIN_ICE = 8,
    TERRAIN_BUILDING = 9,
    TERRAIN_GREAT_MARSH = 10,
    TERRAIN_BRIDGE = 11,
    TERRAIN_AARON = 12,
    TERRAIN_BERTHA = 13,
    TERRAIN_FLINT = 14,
    TERRAIN_LUCIAN = 15,
    TERRAIN_CYNTHIA = 16,
    TERRAIN_DISTORTION_WORLD = 17,
    TERRAIN_BATTLE_TOWER = 18,
    TERRAIN_BATTLE_FACTORY = 19,
    TERRAIN_BATTLE_ARCADE = 20,
    TERRAIN_BATTLE_CASTLE = 21,
    TERRAIN_BATTLE_HALL = 22,
    TERRAIN_GIRATINA = 23,
    TERRAIN_MAX = 24,
};

enum EvolutionMethod {
    EVO_NONE = 0,
    EVO_LEVEL_HAPPINESS = 1,
    EVO_LEVEL_HAPPINESS_DAY = 2,
    EVO_LEVEL_HAPPINESS_NIGHT = 3,
    EVO_LEVEL = 4,
    EVO_TRADE = 5,
    EVO_TRADE_WITH_HELD_ITEM = 6,
    EVO_USE_ITEM = 7,
    EVO_LEVEL_ATK_GT_DEF = 8,
    EVO_LEVEL_ATK_EQ_DEF = 9,
    EVO_LEVEL_ATK_LT_DEF = 10,
    EVO_LEVEL_PID_LOW = 11,
    EVO_LEVEL_PID_HIGH = 12,
    EVO_LEVEL_NINJASK = 13,
    EVO_LEVEL_SHEDINJA = 14,
    EVO_LEVEL_BEAUTY = 15,
    EVO_USE_ITEM_MALE = 16,
    EVO_USE_ITEM_FEMALE = 17,
    EVO_LEVEL_WITH_HELD_ITEM_DAY = 18,
    EVO_LEVEL_WITH_HELD_ITEM_NIGHT = 19,
    EVO_LEVEL_KNOW_MOVE = 20,
    EVO_LEVEL_SPECIES_IN_PARTY = 21,
    EVO_LEVEL_MALE = 22,
    EVO_LEVEL_FEMALE = 23,
    EVO_LEVEL_MAGNETIC_FIELD = 24,
    EVO_LEVEL_MOSS_ROCK = 25,
    EVO_LEVEL_ICE_ROCK = 26,
};

typedef struct TrainerHeader {
    u8 monDataType;
    u8 trainerType;
    u8 sprite;
    u8 partySize;
    u16 items[4];
    u32 aiMask;
    u32 battleType;
} TrainerHeader;

typedef struct Trainer {
    TrainerHeader header;
    charcode_t name[7 + 1];
    MailMessage winMsg;
    MailMessage loseMsg;
} Trainer;

typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

enum ItemBattleCategory {
    ITEM_BATTLE_CATEGORY_RECOVER_HP = 0,
    ITEM_BATTLE_CATEGORY_RECOVER_STATUS = 1,
    ITEM_BATTLE_CATEGORY_POKE_BALLS = 2,
    ITEM_BATTLE_CATEGORY_BATTLE_ITEMS = 3,
    ITEM_BATTLE_CATEGORY_MAX = 4,
};

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

typedef struct FieldBagCursor {
    u8 scroll[8];
    u8 index[8];
    u16 pocket;
    u16 dummy12;
} FieldBagCursor;

typedef struct BattleBagCursor {
    u8 scroll[ITEM_BATTLE_CATEGORY_MAX + 1];
    u8 index[ITEM_BATTLE_CATEGORY_MAX + 1];
    u16 lastUsedItemID;
    u16 lastUsedCategory;
    u16 currentCategory;
} BattleBagCursor;

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

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

u32 GetMonData(Pokemon *mon, enum PokemonDataParam param, void *dest);

typedef struct Pokedex {
    u32 magic;
    u32 caughtSpecies[((int)(((SPECIES_BAD_EGG - 2) - 1) / 32) + 1)];
    u32 seenSpecies[((int)(((SPECIES_BAD_EGG - 2) - 1) / 32) + 1)];
    u32 seenGenders[2][((int)(((SPECIES_BAD_EGG - 2) - 1) / 32) + 1)];
    u32 spindaPersonality;
    u8 shellosFormOrder;
    u8 gastrodonFormOrder;
    u8 burmyFormOrder;
    u8 wormadamFormOrder;
    u8 unownSeenOrder[28];
    u8 unownCaughtOrder[28];
    u8 caughtLanguages[496];
    u8 canDetectForms;
    u8 enabledInternational;
    u8 dexEnabled;
    u8 nationalDex;
    u32 rotomFormOrder;
    u8 shayminFormOrder;
    u8 giratinaFormOrder;
    u8 pichuFormOrder;
    u8 dummy;
} Pokedex;

typedef struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
} EncodingSeed;

typedef struct GameRecords {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
} GameRecords;

typedef struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
} JournalEntryTitle;

typedef struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
} JournalEntryMon;

typedef struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
} JournalEntryTrainer;

typedef struct JournalEntry {
    JournalEntryTitle title;
    u32 locationEvents[4];
    JournalEntryMon mon;
    JournalEntryTrainer trainer;
    u8 onlineEvents[2][42];
} JournalEntry;

typedef struct PCBoxes {
    u32 currentBoxID;
    BoxPokemon boxMons[18][(5 * 6)];
    u16 names[18][20];
    u8 wallpapers[18];
    u8 unlockedWallpapers;
} PCBoxes;

typedef struct Poketch {
    u8 poketchEnabled : 1;
    u8 pedometerEnabled : 1;
    u8 dotArtModifiedByPlayer : 1;
    u8 screenColor : 3;

    s8 appCount;
    s8 appIndex;
    u8 appRegistry[32];

    u32 stepCount;

    u16 alarmSet : 1;
    u16 alarmHour : 5;
    u16 alarmMinute : 6;

    u8 dotArtData[120];

    u32 calendarMarkBitmap;
    u8 calendarMonth;

    struct {
        u8 x;
        u8 y;
    } markMapPositions[6];

    struct {
        u16 species;
        u16 icon;
        u32 form;
    } pokemonHistoryQueue[12];
} Poketch;

enum TimeOfDay {
    TIMEOFDAY_MORNING = 0,
    TIMEOFDAY_DAY = 1,
    TIMEOFDAY_TWILIGHT = 2,
    TIMEOFDAY_NIGHT = 3,
    TIMEOFDAY_LATE_NIGHT = 4,
};

typedef struct BattleRecords {
    int totalTurns;
    int totalFainted;
    int totalDamage;
} BattleRecords;

typedef struct CaptureAttempt {
    int resultMask;
    u16 ballsThrown;
    u16 species;
    u8 gender;
    u8 language;
    u8 metGame;
    u8 hasNickname;
    u16 nickname[11];
    u16 pokeballItemID;
} CaptureAttempt;



void sub_02030964(BattleFactorySave *save, BOOL didSave);

void sub_02030978(BattleFactorySave *save, u8 field, u8 index, const void *value);

void sub_02030AA4(BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, u8 *value);

BattleFactoryStreakFlags *sub_02030AE8(SaveData *saveData);














u8 ov80_02236DD4(u8 challengeType);

u8 ov80_02236DF8(u8 challengeType, BOOL param1);

enum BattleFrontierStatsIndex sub_0205BFF0(u8 isOpenLevel, u8 challengeType);

enum BattleFrontierStatsIndex sub_0205C01C(s32 isOpenLevel, u32 challengeType);

enum BattleFrontierStatsIndex sub_0205C048(u8 isOpenLevel, u8 challengeType);

enum BattleFrontierStatsIndex sub_0205C074(s32 isOpenLevel, u32 challengeType);

int sub_0205C268(u32 statIndex);

FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);

u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);

u16 sub_02031108(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);

u16 sub_0203126C(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);

void ov80_0223049C(BattleFactory *factory, u8 saveType);

void ov80_0223049C(BattleFactory *factory, u8 saveType) {
    u8 u8Ptr[4];
    u16 u16Ptr[4];
    u32 u32Ptr[4];
    BattleFactorySave *factorySave = factory->factorySave;
    BattleFactoryStreakFlags *streakFlags = sub_02030AE8(factory->saveData);

    FrontierSave *frontier = Save_Frontier_GetStatic(factory->saveData);
    (void)(ov80_02236DD4(factory->challengeType));
    (void)(ov80_02236DF8(factory->challengeType, 1));

    u8Ptr[0] = factory->isOpenLevel;
    sub_02030978(factory->factorySave, 0, 0, u8Ptr);

    u8Ptr[0] = factory->challengeType;
    sub_02030978(factory->factorySave, 1, 0, u8Ptr);
    sub_02030964(factory->factorySave, 1);

    u8Ptr[0] = factory->currentBattle;
    sub_02030978(factory->factorySave, 2, 0, u8Ptr);
    sub_02031108(frontier, sub_0205C048(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C048(factory->isOpenLevel, factory->challengeType)), factory->tradeCount);
    sub_02031108(frontier, sub_0205BFF0(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205BFF0(factory->isOpenLevel, factory->challengeType)), factory->currentStreak);

    if (saveType != 2) {
        u16 recordStreak = FrontierSave_GetStat(frontier, sub_0205C01C(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C01C(factory->isOpenLevel, factory->challengeType)));
        u32 newRecord = sub_0203126C(frontier, sub_0205C01C(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C01C(factory->isOpenLevel, factory->challengeType)), factory->currentStreak);
        u16 latestStreak = FrontierSave_GetStat(frontier, sub_0205C01C(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C01C(factory->isOpenLevel, factory->challengeType)));

        if (factory->currentStreak == recordStreak) {
            sub_0203126C(frontier, sub_0205C074(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C074(factory->isOpenLevel, factory->challengeType)), factory->tradeCount);
        } else if (recordStreak < latestStreak) {
            sub_02031108(frontier, sub_0205C074(factory->isOpenLevel, factory->challengeType), sub_0205C268(sub_0205C074(factory->isOpenLevel, factory->challengeType)), factory->tradeCount);
        }

        u8Ptr[0] = factory->saveStreak;
        sub_02030AA4(streakFlags, 10, (factory->isOpenLevel * 4) + factory->challengeType, u8Ptr);

        if (factory->challengeType == 3) {
            u32 statIndex = !factory->isOpenLevel ? STAT_FACTORY_50_WFC_STREAK_ACTIVE : STAT_FACTORY_OPEN_WFC_STREAK_ACTIVE;
            sub_02031108((struct FrontierSave *)((frontier)), (enum BattleFrontierStatsIndex)(statIndex), sub_0205C268(statIndex), factory->saveStreak);
        }
    }

    for (u16 i = 0; i < 7 * 2; i++) {
        u16Ptr[0] = factory->trainerIDs[i];
        sub_02030978(factory->factorySave, 3, i, u16Ptr);
    }

    u32 partySize = Party_GetCount(factory->playersParty);

    for (u16 i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(factory->playersParty, i);

        u16Ptr[0] = factory->playerMonSetIDs[i];
        sub_02030978(factory->factorySave, 4, i, u16Ptr);

        u8Ptr[0] = GetMonData(mon, MON_DATA_ATK_IV, 0L);
        sub_02030978(factory->factorySave, 5, i, u8Ptr);

        u32Ptr[0] = GetMonData(mon, MON_DATA_PERSONALITY, 0L);
        sub_02030978(factory->factorySave, 6, i, u32Ptr);
    }

    partySize = Party_GetCount(factory->opponentsParty);

    for (u16 i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(factory->opponentsParty, i);

        u16Ptr[0] = factory->opponentMonSetIDs[i];
        sub_02030978(factory->factorySave, 7, i, u16Ptr);

        u8Ptr[0] = GetMonData(mon, MON_DATA_ATK_IV, 0L);
        sub_02030978(factory->factorySave, 8, i, u8Ptr);

        u32Ptr[0] = GetMonData(mon, MON_DATA_PERSONALITY, 0L);
        sub_02030978(factory->factorySave, 9, i, u32Ptr);
    }
}
