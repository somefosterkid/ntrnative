#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

struct Bag;
struct BagCursor;
struct CaptureAttempt;
struct ChatotCry;
struct FrontierPokemon;
struct UnkStruct_ov80_02229EE0_0222A480_3DE9;
struct GameStats;
struct JournalEntry;
struct Options;
struct PCBoxes;
struct PalPad;
struct Party;
struct Pokedex;
struct Poketch;
struct SaveData;
struct PlayerProfile;
struct WiFiHistory;
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
typedef struct ChatotCry ChatotCry;
typedef struct TrainerHeader {
    u8 monDataType;
    u8 trainerType;
    u8 sprite;
    u8 partySize;
    u16 items[4];
    u32 aiMask;
    u32 battleType;
} TrainerHeader;
typedef u16 charcode_t;
typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;
typedef struct Trainer {
    TrainerHeader header;
    charcode_t name[7 + 1];
    MailMessage winMsg;
    MailMessage loseMsg;
} Trainer;
typedef struct WiFiHistory WiFiHistory;
typedef struct SaveData SaveData;
typedef struct Bag Bag;
typedef struct BagCursor BagCursor;
typedef struct Options Options;
typedef struct Party Party;
typedef struct PlayerProfile PlayerProfile;
typedef struct Pokedex Pokedex;
typedef struct GameStats GameStats;
typedef struct JournalEntry JournalEntry;
typedef struct PalPad PalPad;
typedef struct PCBoxes PCBoxes;
typedef struct Poketch Poketch;
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
typedef struct CaptureAttempt CaptureAttempt;
typedef struct FieldBattleDTO {
    u32 battleType;
    Party *parties[4];
    int resultMask;
    int trainerIDs[4];
    Trainer trainer[4];
    PlayerProfile *trainerInfo[4];
    Bag *bag;
    BagCursor *bagCursor;
    Pokedex *pokedex;
    PCBoxes *pcBoxes;
    ChatotCry *chatotCries[4];
    Poketch *poketch;
    WiFiHistory *wiFiHistory;
    Options *options;
    CaptureAttempt *captureAttempt;
    BattleRecords battleRecords;
    GameStats *records;
    JournalEntry *journalEntry;
    PalPad *palPad;
    enum BattleBackground background;
    enum BattleTerrain terrain;
    int mapLabelTextID;
    int mapHeaderID;
    enum TimeOfDay timeOfDay;
    enum EvolutionMethod mapEvolutionMethod;
    BOOL visitedContestHall;
    BOOL metBebe;
    int caughtBattlerIdx;
    int fieldWeather;
    int leveledUpMonsMask;
    u32 systemVersion[4];
    u32 battleStatusMask;
    int countSafariBalls;
    u32 rulesetMask;
    void *postBattleData;
    u32 seed;
    int linkPlayerPositions[4];
    u16 networkID;
    u8 recordingType;
    u8 dummy18B;
    int totalTurnsElapsed;
    u8 *subscreenCursorOn;
    u8 recordedChatter[4];
    SaveData *saveData;
    u32 recordingStopped;
} FieldBattleDTO;
typedef struct UnkStruct_ov80_02229EE0_0222A480_3DE9 UnkStruct_ov80_02229EE0_0222A480_3DE9;
struct UnkStruct_ov80_02229EE0_0222A480_3DE9 {
    u32 trainerID;
    u16 trainerType;
    u16 objectID;
    u16 trainerName[7 + 1];
    u16 introMsg[4];
    u16 winMsg[4];
    u16 loseMsg[4];
};
typedef struct FrontierPokemon FrontierPokemon;
charcode_t *CopyU16StringArray(charcode_t *dst, const charcode_t *src);
u16 LCRandom(void);
u32 ov80_02229F6C(FrontierPokemon *mon, u16 narcIdx, u32 otID, u32 personality, u8 ivs, u8 itemIdx, BOOL alwaysFalse, enum HeapID heapID, enum NarcId narcID);
enum PokemonStat {
    STAT_HP = 0,
    STAT_ATTACK = 1,
    STAT_DEFENSE = 2,
    STAT_SPEED = 3,
    STAT_SPECIAL_ATTACK = 4,
    STAT_SPECIAL_DEFENSE = 5,
    STAT_MAX = 6,
};
struct FrontierPokemon {
    struct {
        u16 species : 11;
        u16 form : 5;
    };
    u16 item;
    u16 moves[4];
    u32 otID;
    u32 personality;
    union {
        struct {
            u32 hpIV : 5;
            u32 atkIV : 5;
            u32 defIV : 5;
            u32 speedIV : 5;
            u32 spAtkIV : 5;
            u32 spDefIV : 5;
            u32 setSpeciesAsNickname : 1;
            u32 : 1;
        };
        u32 combinedIVs;
    };
    union {
        struct {
            u8 hpEV;
            u8 atkEV;
            u8 defEV;
            u8 speedEV;
            u8 spAtkEV;
            u8 spDefEV;
        };
        u8 evList[STAT_MAX];
    };
    union {
        struct {
            u8 move0PPUps : 2;
            u8 move1PPUps : 2;
            u8 move2PPUps : 2;
            u8 move3PPUps : 2;
        };
        u8 combinedPPUps;
    };
    u8 language;
    u8 ability;
    u8 friendship;
    u16 nickname[10 + 1];
};
u32 ov80_0222A4EC(FrontierPokemon *frontierMon, u16 narcIdx, int unused, u8 ivs, u32 personality, enum HeapID heapID, enum NarcId narcID);

void ov80_0222A480(FieldBattleDTO *battleDTO, UnkStruct_ov80_02229EE0_0222A480_3DE9 *trainer, int unused, int battlerId, enum HeapID heapID);
u32 ov80_0222A4EC(FrontierPokemon *frontierMon, u16 narcIdx, int unused, u8 ivs, u32 personality, enum HeapID heapID, enum NarcId narcID);
extern void ov80_0222A52C(FrontierPokemon *mons, u16 narcIdx[], u8 ivList[], u32 personalities[], u32 resultPersonalities[], int numMons, enum HeapID heapID, enum NarcId narcID);
u8 ov80_0222A5A4(u16 battleTowerID);

void ov80_0222A480(FieldBattleDTO *battleDTO, UnkStruct_ov80_02229EE0_0222A480_3DE9 *trainer, int unused, int battlerId, enum HeapID heapID) {
    MailMessage *sentence;

    battleDTO->trainerIDs[battlerId] = trainer->trainerID;
    battleDTO->trainer[battlerId].header.trainerType = trainer->trainerType;

    CopyU16StringArray(&battleDTO->trainer[battlerId].name[0], &trainer->trainerName[0]);

    sentence = (MailMessage *)&trainer->winMsg[0];
    battleDTO->trainer[battlerId].winMsg = *sentence;

    sentence = (MailMessage *)&trainer->loseMsg[0];
    battleDTO->trainer[battlerId].loseMsg = *sentence;
}

u32 ov80_0222A4EC(FrontierPokemon *frontierMon, u16 narcIdx, int unused, u8 ivs, u32 personality, enum HeapID heapID, enum NarcId narcID) {
    u32 otID = LCRandom();
    otID |= LCRandom() << 16;
    return ov80_02229F6C(frontierMon, narcIdx, otID, personality, ivs, unused, 0, heapID, narcID);
}

void ov80_0222A52C(FrontierPokemon *mons, u16 narcIdx[], u8 ivList[], u32 personalities[], u32 resultPersonalities[], int numMons, enum HeapID heapID, enum NarcId narcID) {
    for (int i = 0; i < numMons; i++) {
        u8 ivs = ivList == 0L ? 0 : ivList[i];
        u32 personality = (personalities == 0L) ? 0 : personalities[i];
        personality = ov80_0222A4EC(&mons[i], narcIdx[i], i, ivs, personality, heapID, narcID);

        if (resultPersonalities != 0L) {
            resultPersonalities[i] = personality;
        }
    }
}

u8 ov80_0222A5A4(u16 battleTowerID) {
    u8 ivs;

    if (battleTowerID < 100) {
        ivs = 31 / 8 * 1;
    } else if (battleTowerID < 120) {
        ivs = 31 / 8 * 2;
    } else if (battleTowerID < 140) {
        ivs = 31 / 8 * 3;
    } else if (battleTowerID < 160) {
        ivs = 31 / 8 * 4;
    } else if (battleTowerID < 180) {
        ivs = 31 / 8 * 5;
    } else if (battleTowerID < 200) {
        ivs = 31 / 8 * 6;
    } else if (battleTowerID < 220) {
        ivs = 31 / 8 * 7;
    } else {
        ivs = 31;
    }

    return ivs;
}
