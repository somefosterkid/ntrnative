#include "global.h"

#include "unk_02096910.h"
#include "overlay_80_battle_factory.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "overlay_80_0222FD08.h"

struct BattleFactorySave;

struct BattleFrontier;

struct BgConfig;

struct FieldBattleDTO;

struct FrontierMenuManager;

struct FrontierParticleSystem;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct ListMenu2D;

struct MsgData;

struct PaletteData;

struct Party;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_ov63_0222AE60_t;

struct UnkStruct_ov63_0222BB38_t;

struct UnkStruct_ov63_0222BCE8_t;

struct UnkStruct_ov63_0222BE18_t;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov63_0222D1C0_t;

struct UnkStruct_ov63_0222D77C_t;

struct UnkStruct_ov63_0222D848_t;

typedef struct BattleFrontier BattleFrontier;

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

typedef struct BgConfig BgConfig;

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

void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette);

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

typedef u16 charcode_t;

typedef struct String String;

typedef struct MsgData MsgData;

typedef struct ListMenu2D ListMenu2D;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct BallCapsule {
    BallSeal seals[8];
} BallCapsule;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

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

enum PokemonType {
    TYPE_NORMAL = 0,
    TYPE_FIGHTING = 1,
    TYPE_FLYING = 2,
    TYPE_POISON = 3,
    TYPE_GROUND = 4,
    TYPE_ROCK = 5,
    TYPE_BUG = 6,
    TYPE_GHOST = 7,
    TYPE_STEEL = 8,
    TYPE_MYSTERY = 9,
    TYPE_FIRE = 10,
    TYPE_WATER = 11,
    TYPE_GRASS = 12,
    TYPE_ELECTRIC = 13,
    TYPE_PSYCHIC = 14,
    TYPE_ICE = 15,
    TYPE_DRAGON = 16,
    TYPE_DARK = 17,
    NUM_POKEMON_TYPES = 18,
};

typedef struct PaletteData PaletteData;

typedef struct Party Party;

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

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

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

Pokemon *AllocMonZeroed(enum HeapID heapID);

u32 GetMonData(Pokemon *mon, enum PokemonDataParam param, void *dest);

BoxPokemon *Mon_GetBoxMon(Pokemon *mon);

typedef struct MessageFormat MessageFormat;

void BufferBoxMonSpeciesName(MessageFormat *template, u32 idx, BoxPokemon *boxMon);

typedef struct FrontierScriptManager FrontierScriptManager;

typedef struct FrontierScriptContext FrontierScriptContext;

typedef BOOL (*FrontierScrCmdFunc)(FrontierScriptContext *);

typedef BOOL (*FrontierShouldResumeScriptFunc)(FrontierScriptContext *);

struct FrontierScriptContext {
    FrontierScriptManager *frontierSystem;
    u8 padding[4];
    u16 unk_08[8];
    u8 stackPointer;
    u8 state;
    const u8 *scriptPtr;
    const u8 *stack[20];
    const FrontierScrCmdFunc *cmdTable;
    u32 cmdTableSize;
    u16 data[4];
    MsgData *msgLoader;
    u8 *scripts;
    FrontierShouldResumeScriptFunc shouldResume;
    u8 unk_8C;
};

typedef struct FrontierMenuManager FrontierMenuManager;

struct FrontierScriptManager {
    BattleFrontier *frontier;
    FrontierScriptContext *ctx[8];
    UnkStruct_ov104_0222E8C8 unk_24;
    enum HeapID heapID;
    u8 unk_38;
    MsgData *msgLoader;
    u8 *script;
    MessageFormat *strTemplate;
    String *string;
    String *fmtString;
    u8 printerID;
    s8 numSetContexts;
    u16 scene;
    u8 unused[5];
    u8 movementCount;
    u8 isMsgBoxOpen;
    int unused2;
    FrontierMenuManager *menuMan;
    Window msgWindow;
    ListMenu2D *menu;
    u16 *unused3;
    void *savingIcon;
    int unused4[10];
    Window *castleHostInfoWin;
    Window *castlePlayer2InfoWin;
    u16 unk_B0;
    u16 unk_B2;
    u16 unk_B4;
    u16 unk_B6;
    u16 *unk_B8;
};

enum BattleFactoryFunction {
    BF_FUNC_UNK_0 = 0,
    BF_FUNC_SET_IS_OPEN_LEVEL,
    BF_FUNC_SET_CHALLENGE_TYPE,
    BF_FUNC_GET_SELECTED_APP_SLOT,
    BF_FUNC_GET_CURRENT_STREAK,
    BF_FUNC_INCREMENT_CURRENT_STREAK,
    BF_FUNC_UNK_6,
    BF_FUNC_RESET_SYSTEM,
    BF_FUNC_UNK_8,
    BF_FUNC_HAS_SAVED,
    BF_FUNC_QUICKSAVE,
    BF_FUNC_UNK_11,
    BF_FUNC_UNK_12,
    BF_FUNC_UNK_13,
    BF_FUNC_INCREMENT_CURRENT_BATTLE,
    BF_FUNC_GET_MONS_SPECIES,
    BF_FUNC_GET_MONS_MOVE,
    BF_FUNC_GET_MONS_TYPE,
    BF_FUNC_GET_COMMON_TYPE,
    BF_FUNC_GET_CURRENT_ROUND,
    BF_FUNC_GET_OPPONENT_OBJECT_ID,
    BF_FUNC_SAVE_ON_LOSS,
    BF_FUNC_SAVE_ON_COMPLETING_ROUND,
    BF_FUNC_GET_CURRENT_BATTLE,
    BF_FUNC_UNK_24,
    BF_FUNC_UNK_25,
    BF_FUNC_UNK_26,
    BF_FUNC_INIT_COMM_MANAGER,
    BF_FUNC_IS_MULTIPLAYER_CHALLENGE,
    BF_FUNC_GET_CHALLENGE_TYPE,
    BF_FUNC_UPDATE_BACKGROUND,
    BF_FUNC_START_CORRIDOR_ANIMATION,
    BF_FUNC_STOP_CORRIDOR_ANIMATION,
    BF_FUNC_LOAD_TRAINERS,
    BF_FUNC_SET_RENTED_MONS_IN_STRING,
    BF_FUNC_GET_EARNED_BP,
    BF_FUNC_INCREMENT_TRADE_COUNT,
    BF_FUNC_GET_NEXT_BATTLE_TYPE,
    BF_FUNC_UPDATE_PARTNERS_PARTY,
    BF_FUNC_CREATE_OPPONENT_MONS,
    BF_FUNC_CREATE_INITAL_PARTY,
    BF_FUNC_CHECK_SEEN_HEAD_INTRO,
};

void Heap_Free(void *ptr);

typedef struct FieldBattleDTO FieldBattleDTO;

void ov80_0222A140(const FrontierPokemon *frontierMon, Pokemon *mon, s32 setLevel);

void ov80_0222A474(FrontierOpponent *opponent, u16 trainerID, enum HeapID heapID, enum NarcId narcID);

extern void ov80_0222A52C(FrontierPokemon *mons, u16 narcIdx[], u8 ivList[], u32 personalities[], u32 resultPersonalities[], int numMons, enum HeapID heapID, enum NarcId narcID);

typedef struct BattleFactorySave BattleFactorySave;

BOOL sub_0203095C(BattleFactorySave *save);

void ov80_0223049C(BattleFactory *factory, u8 saveType);


BOOL ov80_02230790(BattleFactory *factory);

u16 ov80_02230794(BattleFactory *factory, u8 trainerSlot);

void ov80_022307C8(BattleFactory *factory);

void ov80_022307D4(BattleFactory *factory);

void ov80_02230AE4(BattleFactory *factory);

u16 ov80_02230B4C(BattleFactory *factory);

u8 ov80_02236DD4(u8 challengeType);

u8 ov80_02236DF8(u8 challengeType, BOOL param1);

s32 ov80_02237120(BattleFactory *battleFactory);

void ov80_02237130(BattleFactory *battleFactory);

void ov80_022371B0(BattleFactory *battleFactory);

BOOL ov80_02237254(u8 challengeType);

u16 ov80_022372B4(BattleFactory *battleFactory);

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;

typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct {
    const UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222BB38 *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u8 unk_12;
    u8 unk_13;
} UnkStruct_ov63_0222CE44;

typedef struct {
    UnkStruct_ov63_0222AE60 *unk_00;
    UnkStruct_ov63_0222CE44 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov63_0222CD2C;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222D160;

typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct FrontierParticleSystem FrontierParticleSystem;

typedef struct {
    SysTask *unk_00;
    s16 unk_04;
    s16 unk_06;
    s8 unk_08;
    s8 unk_09;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_0223319C;

typedef struct {
    SysTask *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s16 unk_08;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_022331E8;

typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct FrontierGraphics {
    BgConfig *bgConfig;
    PaletteData *plttData;
    BattleFrontier *frontier;
    G3DPipelineBuffers *g3dPipeline;
    FrontierParticleSystem *particleSys;
    UnkStruct_ov63_0222BE18 *unk_14;
    UnkStruct_ov63_0222BCE8 *unk_18;
    UnkStruct_ov63_0222D160 unk_1C;
    UnkStruct_ov63_0222CD2C *unk_20;
    UnkStruct_ov63_0222D1C0 *unk_24;
    UnkStruct_ov63_0222D1C0 *unk_28;
    UnkStruct_ov63_0222D848 *unk_2C;
    UnkStruct_ov63_0222D77C *unk_30;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteMan;
    UnkStruct_ov104_0223D3B0_1 unk_3C;
    ManagedSprite *unk_80[4];
    void *unk_90;
    SysTask *unk_94;
    SysTask *unk_98;
    SysTask *unk_9C;
    SysTask *dummyExecuteOnVBlank;
    UnkStruct_ov104_0223319C unk_A4;
    UnkStruct_ov104_022331E8 unk_B0;
    u16 unk_BC;
    u16 unk_BE;
    u8 unk_C0;
    u8 sceneID;
    u8 unk_C2;
    u8 pad_C3[1];
} FrontierGraphics;

FrontierGraphics *FrontierSystem_GetFrontierMap(FrontierScriptManager *scriptMan);

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);

void *Frontier_GetData(BattleFrontier *frontier);

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

void SysTask_Destroy(SysTask *task);

void ov80_0222FC08(SysTask *task, void *data);

BOOL FrtCmd_103(FrontierScriptContext *ctx);

struct BattleArcade;
enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};
int Bg_GetYpos(BgConfig *bgConfig, enum GFBgLayer bgLayer);
void ScheduleSetBgPosText(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);
typedef struct BattleArcade BattleArcade;
BOOL ov80_02230AF8(BattleArcade *arcade, u16 command, u16 arg);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);
BOOL ov80_0222FCA0(FrontierScriptContext *ctx);
BOOL FrtCmd_104(FrontierScriptContext *ctx);
BOOL FrtCmd_105(FrontierScriptContext *ctx);
BOOL FrtCmd_106(FrontierScriptContext *ctx);

BOOL FrtCmd_103(FrontierScriptContext *ctx) {
    int i;
    u8 command = (*(ctx->scriptPtr++));
    u8 arg1 = (*(ctx->scriptPtr++));
    u8 arg2 = (*(ctx->scriptPtr++));
    u16 *returnVar = FrontierScript_ReadVarPtr(ctx);

    BattleFactory *battleFactory = Frontier_GetData(ctx->frontierSystem->frontier);
    FrontierGraphics *graphics = FrontierSystem_GetFrontierMap(ctx->frontierSystem);

    switch (command) {
    case BF_FUNC_UNK_0:
        battleFactory->unused2 = arg1;
        break;
    case BF_FUNC_SET_IS_OPEN_LEVEL:
        battleFactory->isOpenLevel = arg1;
        break;
    case BF_FUNC_SET_CHALLENGE_TYPE:
        battleFactory->challengeType = arg1;
        break;
    case BF_FUNC_GET_SELECTED_APP_SLOT:
        *returnVar = battleFactory->selectedAppSlots[arg1];
        break;
    case BF_FUNC_GET_CURRENT_STREAK:
        *returnVar = battleFactory->currentStreak;
        break;
    case BF_FUNC_INCREMENT_CURRENT_STREAK:
        if (battleFactory->currentStreak < 9999) {
            battleFactory->currentStreak++;
        }
        break;
    case BF_FUNC_RESET_SYSTEM:
        OS_ResetSystem(0);
        break;
    case BF_FUNC_HAS_SAVED:
        *returnVar = sub_0203095C(battleFactory->factorySave);
        break;
    case BF_FUNC_QUICKSAVE:
        ov80_0223049C(battleFactory, 2);
        break;
    case BF_FUNC_INCREMENT_CURRENT_BATTLE:
        *returnVar = ov80_02230784(battleFactory);
        break;
    case BF_FUNC_GET_MONS_SPECIES:
        *returnVar = battleFactory->opponentMons[arg1].species;
        break;
    case BF_FUNC_GET_MONS_MOVE:
        *returnVar = battleFactory->opponentMons[arg1].moves[arg2];
        break;
    case BF_FUNC_GET_MONS_TYPE: {
        Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);
        ov80_0222A140(&battleFactory->opponentMons[arg1], mon, ov80_02237120(battleFactory));
        *returnVar = GetMonData(mon, MON_DATA_TYPE_1, 0L);
        Heap_Free(mon);
    } break;
    case BF_FUNC_GET_COMMON_TYPE: {
        int typeCounts[NUM_POKEMON_TYPES];
        u8 partySize = ov80_02236DF8(battleFactory->challengeType, 1);

        for (i = 0; i < NUM_POKEMON_TYPES; i++) {
            typeCounts[i] = 0;
        }

        Pokemon *mon = AllocMonZeroed(HEAP_ID_FIELD2);

        for (i = 0; i < partySize; i++) {
            ov80_0222A140(&battleFactory->opponentMons[i], mon, ov80_02237120(battleFactory));

            u32 type1 = GetMonData(mon, MON_DATA_TYPE_1, 0L);
            u32 type2 = GetMonData(mon, MON_DATA_TYPE_2, 0L);

            if (type1 == type2) {
                type2 = 0xff;
            }

            typeCounts[type1]++;

            if (type2 != 0xff) {
                typeCounts[type2]++;
            }
        }

        Heap_Free(mon);

        u32 commonType = 0;

        for (i = 0; i < NUM_POKEMON_TYPES; i++) {
            if (typeCounts[commonType] < typeCounts[i]) {
                commonType = i;
            }
        }

        if (typeCounts[commonType] <= 1) {
            *returnVar = 0xff;
        } else {
            *returnVar = commonType;
        }
    } break;
    case BF_FUNC_GET_CURRENT_ROUND:
        *returnVar = ov80_022372B4(battleFactory);
        break;
    case BF_FUNC_GET_OPPONENT_OBJECT_ID:
        *returnVar = ov80_02230794(battleFactory, arg1);
        break;
    case BF_FUNC_SAVE_ON_LOSS:
        ov80_022307C8(battleFactory);
        break;
    case BF_FUNC_SAVE_ON_COMPLETING_ROUND:
        ov80_022307D4(battleFactory);
        break;
    case BF_FUNC_GET_CURRENT_BATTLE:
        *returnVar = ov80_02230790(battleFactory);
        break;
    case BF_FUNC_UNK_24:
        *returnVar = battleFactory->unk_57C;
        break;
    case BF_FUNC_UNK_26:
        *returnVar = battleFactory->unk_57D;
        break;
    case BF_FUNC_INIT_COMM_MANAGER:
        sub_02096910(battleFactory);
        break;
    case BF_FUNC_IS_MULTIPLAYER_CHALLENGE:
        *returnVar = ov80_02237254(battleFactory->challengeType);
        break;
    case BF_FUNC_GET_CHALLENGE_TYPE:
        *returnVar = battleFactory->challengeType;
        break;
    case BF_FUNC_UPDATE_BACKGROUND:
        BgTilemapRectChangePalette(graphics->bgConfig, GF_BG_LYR_MAIN_3, 3, 10, 26, 11, arg1);
        ScheduleBgTilemapBufferTransfer(graphics->bgConfig, GF_BG_LYR_MAIN_3);
        break;
    case BF_FUNC_START_CORRIDOR_ANIMATION:
        battleFactory->corridorAnimation = SysTask_CreateOnMainQueue(ov80_0222FC08, FrontierSystem_GetFrontierMap(ctx->frontierSystem), 5);
        break;
    case BF_FUNC_STOP_CORRIDOR_ANIMATION:
        if (battleFactory->corridorAnimation != 0L) {
            SysTask_Destroy(battleFactory->corridorAnimation);
            battleFactory->corridorAnimation = 0L;
        }
        break;
    case BF_FUNC_LOAD_TRAINERS:
        ov80_0222A474(&battleFactory->opponents[0], battleFactory->trainerIDs[battleFactory->currentBattle], HEAP_ID_FIELD2, (enum NarcId)204);
        ov80_0222A474(&battleFactory->opponents[1], battleFactory->trainerIDs[battleFactory->currentBattle + 7], HEAP_ID_FIELD2, (enum NarcId)204);
        break;
    case BF_FUNC_SET_RENTED_MONS_IN_STRING: {
        u8 partySize = ov80_02236DD4(battleFactory->challengeType);

        for (i = 0; i < partySize; i++) {
            Pokemon *mon = Party_GetMonByIndex(battleFactory->playersParty, i);
            BufferBoxMonSpeciesName(ctx->frontierSystem->strTemplate, i, Mon_GetBoxMon(mon));
        }
    } break;
    case BF_FUNC_GET_EARNED_BP:
        *returnVar = ov80_02230B4C(battleFactory);
        break;
    case BF_FUNC_INCREMENT_TRADE_COUNT:
        ov80_02230AE4(battleFactory);
        break;
    case BF_FUNC_GET_NEXT_BATTLE_TYPE:
        *returnVar = 0;

        if (battleFactory->challengeType == 0) {
            if (battleFactory->currentStreak + 1 == 21) {
                *returnVar = 1;
            } else if (battleFactory->currentStreak + 1 == 49) {
                *returnVar = 2;
            }
        }
        break;
    case BF_FUNC_UPDATE_PARTNERS_PARTY:
        ov80_022371B0(battleFactory);
        break;
    case BF_FUNC_CREATE_OPPONENT_MONS:
        ov80_0222A52C(battleFactory->opponentMons, battleFactory->opponentMonSetIDs, battleFactory->opponentMonIVs, battleFactory->opponentMonPersonalities, 0L, (((3) >= (2 * 2)) ? (3) : (2 * 2)), HEAP_ID_FIELD2, (enum NarcId)205);
        break;
    case BF_FUNC_CREATE_INITAL_PARTY:
        ov80_02237130(battleFactory);
        break;
    case BF_FUNC_CHECK_SEEN_HEAD_INTRO:
        *returnVar = battleFactory->seenFactoryHeadIntro;
        battleFactory->seenFactoryHeadIntro = 1;
        break;
    }

    return 0;
}

void ov80_0222FC08(SysTask *task, void *data) {
    FrontierGraphics *graphics = data;

    int offset = Bg_GetYpos(graphics->bgConfig, GF_BG_LYR_MAIN_2);

    if (offset >= 255) {
        ScheduleSetBgPosText(graphics->bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_SET_Y, 0);
    } else {
        ScheduleSetBgPosText(graphics->bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_ADD_Y, 1);
    }
}

BOOL FrtCmd_104(FrontierScriptContext *ctx) {
    u16 *destVar = FrontierScript_ReadVarPtr(ctx);

    BattleFactory *battleFactory = Frontier_GetData(ctx->frontierSystem->frontier);
    *destVar = battleFactory->wonBattle;

    return 0;
}

BOOL FrtCmd_105(FrontierScriptContext *ctx) {
    u16 command = FrontierScript_ReadVar(ctx);
    u16 arg = FrontierScript_ReadVar(ctx);
    u16 *success = FrontierScript_ReadVarPtr(ctx);

    BattleArcade *arcade = Frontier_GetData(ctx->frontierSystem->frontier);
    *success = ov80_02230AF8(arcade, command, arg);

    return 1;
}

BOOL FrtCmd_106(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_Pause(ctx, ov80_0222FCA0);

    return 1;
}
