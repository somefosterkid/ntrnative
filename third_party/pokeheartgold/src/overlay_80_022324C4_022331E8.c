#include "global.h"
#include "overlay_80_battle_castle.h"
#include "constants/heap.h"
#include "save.h"
#include "game_stats.h"
#include "pm_string.h"
#include "bg_window.h"
#include "msgdata.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_12_field_battle_dto.h"
#include "frontier_graphics.h"
#include "battle_castle_save.h"

struct BattleFrontierSave;

struct GameStats;

struct SaveData;

GameStats *Save_GameStats_Get(SaveData *saveData);

u32 GameStats_Add(GameStats *records, int id, u32 toAdd);

enum GameRecord {
    RECORD_STEPS = 0,
    RECORD_TRAINER_SCORE = 1,
    RECORD_UNK_002 = 2,
    RECORD_UNK_003 = 3,
    RECORD_BERRIES_PLANTED = 4,
    RECORD_UNK_005 = 5,
    RECORD_UNK_006 = 6,
    RECORD_WILD_BATTLES_FOUGHT = 7,
    RECORD_TRAINER_BATTLES_FOUGHT = 8,
    RECORD_CAUGHT_POKEMON = 9,
    RECORD_CAUGHT_FISH = 10,
    RECORD_EGGS_HATCHED = 11,
    RECORD_POKEMON_EVOLVED = 12,
    RECORD_UNK_013 = 13,
    RECORD_UNK_014 = 14,
    RECORD_BATTLE_TOWER_CHALLENGES = 15,
    RECORD_UNK_016 = 16,
    RECORD_UNK_017 = 17,
    RECORD_UNK_018 = 18,
    RECORD_LOCAL_LINK_TRADES = 19,
    RECORD_UNK_020 = 20,
    RECORD_LOCAL_LINK_BATTLE_WINS = 21,
    RECORD_LOCAL_LINK_BATTLE_LOSSES = 22,
    RECORD_UNK_023 = 23,
    RECORD_WIFI_TRADES = 24,
    RECORD_UNK_025 = 25,
    RECORD_WIFI_BATTLE_WINS = 26,
    RECORD_WIFI_BATTLE_LOSSES = 27,
    RECORD_UNK_028 = 28,
    RECORD_BATTLE_TOWER_VICTORIES = 29,
    RECORD_UNK_030 = 30,
    RECORD_UNK_031 = 31,
    RECORD_UNK_032 = 32,
    RECORD_UNK_033 = 33,
    RECORD_UNK_034 = 34,
    RECORD_MONEY_SPENT = 35,
    RECORD_UNK_036 = 36,
    RECORD_UNK_037 = 37,
    RECORD_UNK_038 = 38,
    RECORD_UNK_039 = 39,
    RECORD_DEPOSITED_IN_DAYCARE = 40,
    RECORD_FAINTED_IN_BATTLE = 41,
    RECORD_UNK_042 = 42,
    RECORD_UNK_043 = 43,
    RECORD_UNK_044 = 44,
    RECORD_TIMES_MAIL_WRITTEN = 45,
    RECORD_UNK_046 = 46,
    RECORD_SPHERES_BURIED = 47,
    RECORD_WATCHED_TV = 48,
    RECORD_POKEMON_NICKNAMED = 49,
    RECORD_UNK_050 = 50,
    RECORD_UNK_051 = 51,
    RECORD_UNK_052 = 52,
    RECORD_UNK_053 = 53,
    RECORD_TIMES_DRESSED_UP_POKEMON = 54,
    RECORD_UNK_055 = 55,
    RECORD_TIMES_STUCK_IN_DEEP_MUD = 56,
    RECORD_TIMES_BATTLED_AT_BATTLEGROUND = 57,
    RECORD_TIMES_STARTED_BATTLE_FRONTIER_CHALLENGE = 58,
    RECORD_UNK_059 = 59,
    RECORD_BATTLE_FACTORY_VICTORIES = 60,
    RECORD_BATTLE_CASTLE_VICTORIES = 61,
    RECORD_BATTLE_HALL_VICTORIES = 62,
    RECORD_BATTLE_ARCADE_VICTORIES = 63,
    RECORD_BATTLE_FACTORY_TRADES = 64,
    RECORD_CASTLE_POINTS_RECEIVED = 65,
    RECORD_UNK_066 = 66,
    RECORD_UNK_067 = 67,
    RECORD_BATTLE_POINTS_RECEIVED = 68,
    RECORD_BATTLE_POINTS_SPENT = 69,
    RECORD_STAR_PIECES_TRADED = 70,
    RECORD_UNK_071 = 71,
    RECORD_UNK_072 = 72,
    RECORD_TIMES_ENTERED_HALL_OF_FAME = 73,
    RECORD_UNK_074 = 74,
    RECORD_UNK_075 = 75,
    RECORD_USED_SPLASH = 76,
    RECORD_UNK_077 = 77,
    RECORD_USED_SELFDESTRUCT = 78,
    RECORD_USED_EXPLOSION = 79,
    RECORD_UNK_080 = 80,
    RECORD_UNK_081 = 81,
    RECORD_UNK_082 = 82,
    RECORD_UNK_083 = 83,
    RECORD_UNK_084 = 84,
    RECORD_UNK_085 = 85,
    RECORD_UNK_086 = 86,
    RECORD_UNK_087 = 87,
    RECORD_UNK_088 = 88,
    RECORD_UNK_089 = 89,
    RECORD_SUPER_CONTEST_PARTICIPATIONS = 90,
    RECORD_LINK_CONTEST_PARTICIPATIONS = 91,
    RECORD_OFFICIAL_CONTEST_WINS = 92,
    RECORD_LINK_CONTEST_WINS = 93,
    RECORD_RIBBONS_WON = 94,
    RECORD_USED_INEFFECTIVE_MOVE = 95,
    RECORD_FAINTED_ENEMY_MON = 96,
    RECORD_ATTACKED_ALLY = 97,
    RECORD_FAILED_ESCAPE = 98,
    RECORD_WILD_MON_FLED = 99,
    RECORD_FISH_GOT_AWAY = 100,
    RECORD_UNK_101 = 101,
    RECORD_UNK_102 = 102,
    RECORD_UNK_103 = 103,
    RECORD_UNK_104 = 104,
    RECORD_UNK_105 = 105,
    RECORD_UNK_106 = 106,
    RECORD_UNK_107 = 107,
    RECORD_UNK_108 = 108,
    RECORD_UNK_109 = 109,
    RECORD_UNK_110 = 110,
    RECORD_UNK_111 = 111,
    RECORD_UNK_112 = 112,
    RECORD_UNK_113 = 113,
    RECORD_TIMES_SIGNED_TRAINER_CARD = 114,
    RECORD_POKEMON_RECEIVED_FROM_FOSSIL_REVIVAL = 115,
    RECORD_CHECKED_FOOTPRINTS = 116,
    RECORD_HONEY_TREE_POKEMON_ENCOUNTERED = 117,
    RECORD_UNK_118 = 118,
    RECORD_UNK_119 = 119,
    RECORD_UNK_120 = 120,
    RECORD_UNK_121 = 121,
    RECORD_UNK_122 = 122,
    RECORD_UNK_123 = 123,
    RECORD_UNK_124 = 124,
    RECORD_UNK_125 = 125,
    RECORD_UNK_126 = 126,
    RECORD_UNK_127 = 127,
    RECORD_UNK_128 = 128,
    RECORD_UNK_129 = 129,
    RECORD_UNK_130 = 130,
    RECORD_UNK_131 = 131,
    RECORD_UNK_132 = 132,
    RECORD_UNK_133 = 133,
    RECORD_UNK_134 = 134,
    RECORD_UNK_135 = 135,
    RECORD_UNK_136 = 136,
    RECORD_UNK_137 = 137,
    RECORD_UNK_138 = 138,
    RECORD_UNK_139 = 139,
    RECORD_UNK_140 = 140,
    RECORD_UNK_141 = 141,
    RECORD_UNK_142 = 142,
    RECORD_UNK_143 = 143,
    RECORD_UNK_144 = 144,
    RECORD_UNK_145 = 145,
    RECORD_UNK_146 = 146,
    RECORD_UNK_147 = 147,
    MAX_RECORDS = 148,
};

enum BattleFrontierStatsIndex {
    STAT_TOWER_RECORD_STREAK_SINGLE = 0,
    STAT_TOWER_LATEST_STREAK_SINGLE,
    STAT_TOWER_RECORD_STREAK_DOUBLE,
    STAT_TOWER_LATEST_STREAK_DOUBLE,
    STAT_TOWER_RECORD_STREAK_MULTI,
    STAT_TOWER_LATEST_STREAK_MULTI,
    STAT_TOWER_RECORD_STREAK_LINK_MULTI,
    STAT_TOWER_LATEST_STREAK_LINK_MULTI,
    STAT_TOWER_RECORD_STREAK_WIFI,
    STAT_TOWER_LATEST_STREAK_WIFI,
    STAT_FACTORY_RECORD_STREAK_50_SINGLE,
    STAT_FACTORY_LATEST_STREAK_50_SINGLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_SINGLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_SINGLE,
    STAT_FACTORY_RECORD_STREAK_OPEN_SINGLE,
    STAT_FACTORY_LATEST_STREAK_OPEN_SINGLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_SINGLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_SINGLE,
    STAT_FACTORY_RECORD_STREAK_50_DOUBLE,
    STAT_FACTORY_LATEST_STREAK_50_DOUBLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_DOUBLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_DOUBLE,
    STAT_FACTORY_RECORD_STREAK_OPEN_DOUBLE,
    STAT_FACTORY_LATEST_STREAK_OPEN_DOUBLE,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_DOUBLE,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_DOUBLE,
    STAT_FACTORY_RECORD_STREAK_50_MULTI,
    STAT_FACTORY_LATEST_STREAK_50_MULTI,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_MULTI,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_MULTI,
    STAT_FACTORY_RECORD_STREAK_OPEN_MULTI,
    STAT_FACTORY_LATEST_STREAK_OPEN_MULTI,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_MULTI,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_MULTI,
    STAT_HALL_CURRENT_STREAK_SINGLE,
    STAT_HALL_LATEST_STREAK_SINGLE,
    STAT_HALL_LATEST_SPECIES_SINGLE,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_SINGLE,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_SINGLE,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_SINGLE,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_SINGLE,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_SINGLE,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_SINGLE,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_SINGLE,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_SINGLE,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_SINGLE,
    STAT_HALL_CURRENT_STREAK_DOUBLE,
    STAT_HALL_LATEST_STREAK_DOUBLE,
    STAT_HALL_LATEST_SPECIES_DOUBLE,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_DOUBLE,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_DOUBLE,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_DOUBLE,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_DOUBLE,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_DOUBLE,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_DOUBLE,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_DOUBLE,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_DOUBLE,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_DOUBLE,
    STAT_HALL_CURRENT_STREAK_MULTI,
    STAT_HALL_LATEST_STREAK_MULTI,
    STAT_HALL_LATEST_SPECIES_MULTI,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_MULTI,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_MULTI,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_MULTI,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_MULTI,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_MULTI,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_MULTI,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_MULTI,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_MULTI,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_MULTI,
    STAT_CASTLE_RECORD_STREAK_SINGLE,
    STAT_CASTLE_LATEST_STREAK_SINGLE,
    STAT_CASTLE_LATEST_CP_SINGLE,
    STAT_CASTLE_SPENT_CP_SINGLE,
    STAT_CASTLE_RECORD_CP_SINGLE,
    STAT_CASTLE_RANK_HEALING_SINGLE,
    STAT_CASTLE_RANK_RENTALS_SINGLE,
    STAT_CASTLE_RANK_OPP_SUMMARY_SINGLE,
    STAT_CASTLE_RECORD_STREAK_DOUBLE,
    STAT_CASTLE_LATEST_STREAK_DOUBLE,
    STAT_CASTLE_LATEST_CP_DOUBLE,
    STAT_CASTLE_SPENT_CP_DOUBLE,
    STAT_CASTLE_RECORD_CP_DOUBLE,
    STAT_CASTLE_RANK_HEALING_DOUBLE,
    STAT_CASTLE_RANK_RENTALS_DOUBLE,
    STAT_CASTLE_RANK_OPP_SUMMARY_DOUBLE,
    STAT_CASTLE_RECORD_STREAK_MULTI,
    STAT_CASTLE_LATEST_STREAK_MULTI,
    STAT_CASTLE_LATEST_CP_MULTI,
    STAT_CASTLE_SPENT_CP_MULTI,
    STAT_CASTLE_RECORD_CP_MULTI,
    STAT_CASTLE_RANK_HEALING_MULTI,
    STAT_CASTLE_RANK_RENTALS_MULTI,
    STAT_CASTLE_RANK_OPP_SUMMARY_MULTI,
    STAT_ARCADE_RECORD_STREAK_SINGLE,
    STAT_ARCADE_LATEST_STREAK_SINGLE,
    STAT_ARCADE_RECORD_STREAK_DOUBLE,
    STAT_ARCADE_LATEST_STREAK_DOUBLE,
    STAT_ARCADE_RECORD_STREAK_MULTI,
    STAT_ARCADE_LATEST_STREAK_MULTI,

    STATS_NUM_SOLO_STATS,

    STAT_TOWER_WFC_STREAK_ACTIVE = STATS_NUM_SOLO_STATS,
    STAT_TOWER_WFC_STREAK_ACTIVE_2,
    STAT_FACTORY_50_WFC_STREAK_ACTIVE,
    STAT_FACTORY_50_WFC_STREAK_ACTIVE_2,
    STAT_FACTORY_OPEN_WFC_STREAK_ACTIVE,
    STAT_FACTORY_OPEN_WFC_STREAK_ACTIVE_2,
    STAT_HALL_WFC_STREAK_ACTIVE,
    STAT_HALL_WFC_STREAK_ACTIVE_2,
    STAT_CASTLE_WFC_STREAK_ACTIVE,
    STAT_CASTLE_WFC_STREAK_ACTIVE_2,
    STAT_ARCADE_WFC_STREAK_ACTIVE,
    STAT_ARCADE_WFC_STREAK_ACTIVE_2,

    STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS,

    STAT_TOWER_RECORD_STREAK_MODE_6 = STATS_NUM_SOLO_STATS_AND_ACTIVE_FLAGS,
    STAT_TOWER_LATEST_STREAK_MODE_6,
    STAT_FACTORY_RECORD_STREAK_50_MULTI_WFC,
    STAT_FACTORY_LATEST_STREAK_50_MULTI_WFC,
    STAT_FACTORY_RECORD_TRADE_COUNT_50_MULTI_WFC,
    STAT_FACTORY_LATEST_TRADE_COUNT_50_MULTI_WFC,
    STAT_FACTORY_RECORD_STREAK_OPEN_MULTI_WFC,
    STAT_FACTORY_LATEST_STREAK_OPEN_MULTI_WFC,
    STAT_FACTORY_RECORD_TRADE_COUNT_OPEN_MULTI_WFC,
    STAT_FACTORY_LATEST_TRADE_COUNT_OPEN_MULTI_WFC,
    STAT_HALL_CURRENT_STREAK_MULTI_WFC,
    STAT_HALL_LATEST_STREAK_MULTI_WFC,
    STAT_HALL_LATEST_SPECIES_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_NORMAL_FIGHTING_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_FLYING_POISON_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_GROUND_ROCK_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_BUG_GHOST_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_STEEL_MYSTERY_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_FIRE_WATER_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_GRASS_ELECTRIC_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_PSYCHIC_ICE_MULTI_WFC,
    STAT_HALL_CURRENT_RANKS_DRAGON_DARK_MULTI_WFC,
    STAT_CASTLE_RECORD_STREAK_MULTI_WFC,
    STAT_CASTLE_LATEST_STREAK_MULTI_WFC,
    STAT_CASTLE_LATEST_CP_MULTI_WFC,
    STAT_CASTLE_SPENT_CP_MULTI_WFC,
    STAT_CASTLE_RECORD_CP_MULTI_WFC,
    STAT_CASTLE_RANK_HEALING_MULTI_WFC,
    STAT_CASTLE_RANK_RENTALS_MULTI_WFC,
    STAT_CASTLE_RANK_OPP_SUMMARY_MULTI_WFC,
    STAT_ARCADE_RECORD_STREAK_MULTI_WFC,
    STAT_ARCADE_LATEST_STREAK_MULTI_WFC,

    STATS_NUM_STATS_TOTAL,
};

typedef struct BattleFrontierSave BattleFrontierSave;

enum BattleFrontierStatsIndex sub_0205C1F0(u8 challengeType);

int sub_0205C268(u32 statIndex);

BattleFrontierSave *Save_Frontier_GetStatic(SaveData *saveData);

u16 FrontierSave_GetStat(BattleFrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);

u16 sub_02031108(BattleFrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);

#if defined(SDK_PORT) || defined(NONMATCHING)
u16 sub_02031228(BattleFrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, s16 addValue);
#else
u16 sub_02031228(BattleFrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, int addValue);
#endif

void ov80_022331E8(SaveData *saveData, u8 challengeType, int newCP);

struct BattleCastle;
struct BattleCastleSave;
struct BattleFrontier;
struct BgConfig;
struct FrontierGraphics;
struct FrontierMenuManager;
struct FrontierParticleSystem;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct G3DPipelineBuffers;
struct ListMenu2D;
struct MsgData;
struct String;
struct MessageFormat;
struct SysTask;
struct SysTaskManager;
struct PlayerProfile;
struct Window;
BOOL ov80_0222B5C8(BattleCastle *battleCastle);
BOOL ov80_0222B65C(BattleCastle *battleCastle);
BOOL ov80_0222B6C8(BattleCastle *battleCastle);
BOOL ov80_0222B7E4(BattleCastle *battleCastle);
BOOL ov80_0222B8D8(BattleCastle *battleCastle, u16 param1);
BOOL ov80_0222B920(BattleCastle *battleCastle, u16 param1);
BOOL ov80_0222B968(BattleCastle *battleCastle);
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);
typedef struct ListMenu2D ListMenu2D;
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
void ov80_02233490(FrontierScriptManager *scriptMan, BattleCastle *castle);
BOOL ov80_02237D8C(u8 challengeType);
typedef struct FrontierParticleSystem FrontierParticleSystem;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
FrontierGraphics *FrontierSystem_GetFrontierMap(FrontierScriptManager *scriptMan);
static void ov80_022333D0(BgConfig *bgConfig, Window *window);
void FillWindowPixelBuffer(Window *window, u8 val);
void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);
u16 sub_0203769C(void);
void ClearWindowPixelBuffer(Window *window, u8 skipTransfer);
PlayerProfile *sub_02034818(int netId);
PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);
void ov80_02233594(FrontierScriptManager *scriptMan, Window *window, PlayerProfile *trainer, u16 castlePoints);
BOOL ov80_02233280(BattleCastle *castle, u16 command, u16 arg);
void ov80_022332D0(FrontierScriptManager *scriptMan, BattleCastle *castle);
static void ov80_022333D0(BgConfig *bgConfig, Window *window);
void ov80_022333F0(FrontierScriptManager *scriptMan, BattleCastle *castle);
void ov80_02233490(FrontierScriptManager *scriptMan, BattleCastle *castle);

void ov80_022331E8(SaveData *saveData, u8 challengeType, int newCP) {
    u16 currentCP = FrontierSave_GetStat(Save_Frontier_GetStatic(saveData), sub_0205C1F0(challengeType), sub_0205C268(sub_0205C1F0(challengeType)));

    if (currentCP + newCP > 9999) {
        sub_02031108(Save_Frontier_GetStatic(saveData), sub_0205C1F0(challengeType), sub_0205C268(sub_0205C1F0(challengeType)), 9999);
    } else {
        sub_02031228(Save_Frontier_GetStatic(saveData), sub_0205C1F0(challengeType), sub_0205C268(sub_0205C1F0(challengeType)), newCP);
    }

    GameStats_Add(Save_GameStats_Get(saveData), RECORD_UNK_066, newCP);
}

BOOL ov80_02233280(BattleCastle *castle, u16 command, u16 arg) {
    int success;

    switch (command) {
    case 0:
        success = ov80_0222B5C8(castle);
        break;
    case 1:
        success = ov80_0222B65C(castle);
        break;
    case 2:
        success = ov80_0222B6C8(castle);
        break;
    case 3:
        success = ov80_0222B7E4(castle);
        break;
    case 4:
        success = ov80_0222B8D8(castle, arg);
        break;
    case 5:
        success = ov80_0222B920(castle, arg);
        break;
    case 6:
        success = ov80_0222B968(castle);
        break;
    }

    return success;
}

void ov80_022332D0(FrontierScriptManager *scriptMan, BattleCastle *castle) {
    FrontierGraphics *graphics = FrontierSystem_GetFrontierMap(scriptMan);

    ((scriptMan->castleHostInfoWin == 0L) ? (void)0 : GF_AssertFail());
    ((scriptMan->castlePlayer2InfoWin == 0L) ? (void)0 : GF_AssertFail());

    if (!ov80_02237D8C(castle->challengeType)) {
        scriptMan->castleHostInfoWin = AllocWindows(HEAP_ID_FIELD2, 1);
        AddWindowParameterized(graphics->bgConfig, scriptMan->castleHostInfoWin, 1, 1, 1, 10, 4, 14, ((((1024 - (18 + 12)) - 9) - (27 * 4)) - (11 * 22)) - (10 * 4));
        ov80_022333D0(graphics->bgConfig, scriptMan->castleHostInfoWin);
    } else {
        scriptMan->castleHostInfoWin = AllocWindows(HEAP_ID_FIELD2, 1);
        scriptMan->castlePlayer2InfoWin = AllocWindows(HEAP_ID_FIELD2, 1);

        AddWindowParameterized(graphics->bgConfig, scriptMan->castleHostInfoWin, 1, 1, 1, 10, 4, 14, ((((1024 - (18 + 12)) - 9) - (27 * 4)) - (11 * 22)) - (10 * 4));
        AddWindowParameterized(graphics->bgConfig, scriptMan->castlePlayer2InfoWin, 1, 21, 1, 10, 4, 14, (((((1024 - (18 + 12)) - 9) - (27 * 4)) - (11 * 22)) - (10 * 4)) - (10 * 4));

        ov80_022333D0(graphics->bgConfig, scriptMan->castleHostInfoWin);
        ov80_022333D0(graphics->bgConfig, scriptMan->castlePlayer2InfoWin);
    }

    ov80_02233490(scriptMan, castle);
}

static void ov80_022333D0(BgConfig *bgConfig, Window *window) {
    DrawFrameAndWindow1(window, 1, (1024 - (18 + 12)) - 9, 12);
    FillWindowPixelBuffer(window, 15);
}

void ov80_022333F0(FrontierScriptManager *scriptMan, BattleCastle *castle) {
    Window *window1, *window2;

    if (!ov80_02237D8C(castle->challengeType)) {
        ((scriptMan->castleHostInfoWin != 0L) ? (void)0 : GF_AssertFail());
        window1 = scriptMan->castleHostInfoWin;
        ClearWindowPixelBuffer(window1, 0);
        WindowArray_Delete(window1, 1);
    } else {
        ((scriptMan->castleHostInfoWin != 0L) ? (void)0 : GF_AssertFail());
        ((scriptMan->castlePlayer2InfoWin != 0L) ? (void)0 : GF_AssertFail());

        if (sub_0203769C() == 0) {
            window1 = scriptMan->castleHostInfoWin;
            window2 = scriptMan->castlePlayer2InfoWin;
        } else {
            window1 = scriptMan->castlePlayer2InfoWin;
            window2 = scriptMan->castleHostInfoWin;
        }

        ClearWindowPixelBuffer(window1, 0);
        WindowArray_Delete(window1, 1);
        ClearWindowPixelBuffer(window2, 0);
        WindowArray_Delete(window2, 1);
    }

    scriptMan->castleHostInfoWin = 0L;
    scriptMan->castlePlayer2InfoWin = 0L;
}

void ov80_02233490(FrontierScriptManager *scriptMan, BattleCastle *castle) {
    u16 hostsCP, player2CP;

    if (!ov80_02237D8C(castle->challengeType)) {
        ((scriptMan->castleHostInfoWin != 0L) ? (void)0 : GF_AssertFail());
        hostsCP = FrontierSave_GetStat(Save_Frontier_GetStatic(castle->saveData), sub_0205C1F0(castle->challengeType), sub_0205C268(sub_0205C1F0(castle->challengeType)));
        ov80_02233594(scriptMan, scriptMan->castleHostInfoWin, Save_PlayerData_GetProfile(castle->saveData), hostsCP);
    } else {
        ((scriptMan->castleHostInfoWin != 0L) ? (void)0 : GF_AssertFail());
        ((scriptMan->castlePlayer2InfoWin != 0L) ? (void)0 : GF_AssertFail());

        if (sub_0203769C() == 0) {
            hostsCP = FrontierSave_GetStat(Save_Frontier_GetStatic(castle->saveData), sub_0205C1F0(castle->challengeType), sub_0205C268(sub_0205C1F0(castle->challengeType)));
            player2CP = castle->partnersCP;
        } else {
            hostsCP = castle->partnersCP;
            player2CP = FrontierSave_GetStat(Save_Frontier_GetStatic(castle->saveData), sub_0205C1F0(castle->challengeType), sub_0205C268(sub_0205C1F0(castle->challengeType)));
        }

        ov80_02233594(scriptMan, scriptMan->castleHostInfoWin, sub_02034818(0), hostsCP);
        ov80_02233594(scriptMan, scriptMan->castlePlayer2InfoWin, sub_02034818(1), player2CP);
    }
}
