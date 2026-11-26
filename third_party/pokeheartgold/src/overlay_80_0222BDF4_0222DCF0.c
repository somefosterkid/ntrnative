#include "global.h"

#include "frontier_launch.h"
#include "overlay_80_field_frontier_dto.h"
#include "constants/heap.h"
#include "bg_window.h"

struct BagCursor;
struct BattleFrontier;
struct FrontierGraphics;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct JournalEntry;
struct ListMenu2D;
struct MsgData;
struct Options;
struct SaveData;
struct ScreenScrollManager;
struct String;
struct MessageFormat;
struct SysTask;
struct SysTaskManager;
struct PlayerProfile;
struct UnkStruct_ov104_0223EBD0_t;
struct UnkStruct_ov104_0223F174_t;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct String String;
typedef struct MsgData MsgData;
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
typedef struct MessageFormat MessageFormat;
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
typedef struct FrontierGraphics FrontierGraphics;
void Heap_Free(void *ptr);
typedef struct UnkStruct_ov104_0223EBD0_t UnkStruct_ov104_0223EBD0;
typedef struct ScreenScrollManager ScreenScrollManager;
typedef struct {
    ScreenScrollManager *screenScrollMgr;
    SysTask *unk_04;
    u32 unk_08;
} UnkStruct_ov104_0223F094;
typedef struct UnkStruct_ov104_0223F174_t UnkStruct_ov104_0223F174;
void *Frontier_GetData(BattleFrontier *frontier);
void Frontier_SetData(BattleFrontier *frontier, void *facilityData);
void sub_0200FBF4(enum GFScreen screen, u16 color);
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    int unk_0C;
} UnkStruct_ov104_022313FC_sub1;
typedef struct {
    UnkStruct_ov104_022313FC_sub1 unk_00[96];
    UnkStruct_ov104_0223F174 *unk_600;
    int unk_604;
} UnkStruct_ov104_022313FC;
typedef struct {
    FrontierGraphics *unk_00;
    int unk_04;
    int unk_08;
    BOOL unk_0C;
    BOOL unk_10;
    void *unk_14;
    UnkStruct_ov104_0223F094 unk_18;
    UnkStruct_ov104_022313FC *unk_24;
    Window *unk_28;
    UnkStruct_ov104_0223EBD0 *unk_2C;
} UnkStruct_ov104_02231148;
extern BOOL (*const ov80_0223B9EC[6])(UnkStruct_ov104_02231148 *);
enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};
typedef struct SaveData SaveData;
typedef struct PlayerProfile PlayerProfile;
u32 PlayerProfile_GetTrainerGender(const PlayerProfile *info);
u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
enum ObjectEventGfx {
    OBJ_EVENT_GFX_PLAYER_M = 0,
    OBJ_EVENT_GFX_NINJA_BOY = 1,
    OBJ_EVENT_GFX_TWIN = 2,
    OBJ_EVENT_GFX_SCHOOL_KID_M = 3,
    OBJ_EVENT_GFX_YOUNGSTER = 4,
    OBJ_EVENT_GFX_BUG_CATCHER = 5,
    OBJ_EVENT_GFX_LASS = 6,
    OBJ_EVENT_GFX_BATTLE_GIRL = 7,
    OBJ_EVENT_GFX_SCHOOL_KID_F = 8,
    OBJ_EVENT_GFX_POKEMON_BREEDER_M = 9,
    OBJ_EVENT_GFX_GUITARIST = 10,
    OBJ_EVENT_GFX_ACE_TRAINER_M = 11,
    OBJ_EVENT_GFX_POKEMON_BREEDER_F = 12,
    OBJ_EVENT_GFX_BEAUTY = 13,
    OBJ_EVENT_GFX_ACE_TRAINER_F = 14,
    OBJ_EVENT_GFX_POKEFAN_M = 15,
    OBJ_EVENT_GFX_POKEFAN_F = 16,
    OBJ_EVENT_GFX_EXPERT_M = 17,
    OBJ_EVENT_GFX_EXPERT_F = 18,
    OBJ_EVENT_GFX_COLLECTOR = 19,
    OBJ_EVENT_GFX_HIKER = 20,
    OBJ_EVENT_GFX_PLAYER_M_BIKE = 21,
    OBJ_EVENT_GFX_REPORTER = 22,
    OBJ_EVENT_GFX_CAMERAMAN = 23,
    OBJ_EVENT_GFX_CASHIER_M = 24,
    OBJ_EVENT_GFX_CASHIER_F = 25,
    OBJ_EVENT_GFX_POKECENTER_NURSE = 26,
    OBJ_EVENT_GFX_TEALA = 27,
    OBJ_EVENT_GFX_UNUSED_RECEPTIONIST = 28,
    OBJ_EVENT_GFX_SCIENTIST_M = 29,
    OBJ_EVENT_GFX_SCIENTIST_F = 30,
    OBJ_EVENT_GFX_ROUGHNECK = 31,
    OBJ_EVENT_GFX_SKIER_M = 32,
    OBJ_EVENT_GFX_SKIER_F = 33,
    OBJ_EVENT_GFX_POLICEMAN = 34,
    OBJ_EVENT_GFX_IDOL = 35,
    OBJ_EVENT_GFX_GENTLEMAN = 36,
    OBJ_EVENT_GFX_SOCIALITE = 37,
    OBJ_EVENT_GFX_CYCLIST_M = 38,
    OBJ_EVENT_GFX_CYCLIST_F = 39,
    OBJ_EVENT_GFX_WORKER = 40,
    OBJ_EVENT_GFX_RANCHER = 41,
    OBJ_EVENT_GFX_COWGIRL = 42,
    OBJ_EVENT_GFX_CLOWN = 43,
    OBJ_EVENT_GFX_ARTIST = 44,
    OBJ_EVENT_GFX_JOGGER = 45,
    OBJ_EVENT_GFX_SWIMMER_M = 46,
    OBJ_EVENT_GFX_SWIMMER_F = 47,
    OBJ_EVENT_GFX_TUBER_F = 48,
    OBJ_EVENT_GFX_TUBER_M = 49,
    OBJ_EVENT_GFX_RUIN_MANIAC = 50,
    OBJ_EVENT_GFX_BLACK_BELT = 51,
    OBJ_EVENT_GFX_CAMPER = 52,
    OBJ_EVENT_GFX_PICNICKER = 53,
    OBJ_EVENT_GFX_FISHERMAN = 54,
    OBJ_EVENT_GFX_PARASOL_LADY = 55,
    OBJ_EVENT_GFX_SAILOR = 56,
    OBJ_EVENT_GFX_DUMMY_057 = 57,
    OBJ_EVENT_GFX_DUMMY_058 = 58,
    OBJ_EVENT_GFX_WAITER = 59,
    OBJ_EVENT_GFX_WAITRESS = 60,
    OBJ_EVENT_GFX_DUMMY_061 = 61,
    OBJ_EVENT_GFX_RICH_BOY = 62,
    OBJ_EVENT_GFX_LADY = 63,
    OBJ_EVENT_GFX_SNOWPOINT_NPC_M = 64,
    OBJ_EVENT_GFX_SNOWPOINT_NPC_F = 65,
    OBJ_EVENT_GFX_DUMMY_066 = 66,
    OBJ_EVENT_GFX_DUMMY_067 = 67,
    OBJ_EVENT_GFX_ACE_TRAINER_SNOW_M = 68,
    OBJ_EVENT_GFX_ACE_TRAINER_SNOW_F = 69,
    OBJ_EVENT_GFX_PSYCHIC = 70,
    OBJ_EVENT_GFX_PIKACHU = 71,
    OBJ_EVENT_GFX_CLEFAIRY = 72,
    OBJ_EVENT_GFX_JIGGLYPUFF = 73,
    OBJ_EVENT_GFX_PSYDUCK = 74,
    OBJ_EVENT_GFX_DUMMY_075 = 75,
    OBJ_EVENT_GFX_DUMMY_076 = 76,
    OBJ_EVENT_GFX_DUMMY_077 = 77,
    OBJ_EVENT_GFX_TORCHIC = 78,
    OBJ_EVENT_GFX_SKITTY = 79,
    OBJ_EVENT_GFX_DUMMY_080 = 80,
    OBJ_EVENT_GFX_BABY_IN_PRAM = 81,
    OBJ_EVENT_GFX_MIDDLE_AGED_MAN = 82,
    OBJ_EVENT_GFX_MIDDLE_AGED_WOMAN = 83,
    OBJ_EVENT_GFX_STRENGTH_BOULDER = 84,
    OBJ_EVENT_GFX_ROCK_SMASH = 85,
    OBJ_EVENT_GFX_CUT_TREE = 86,
    OBJ_EVENT_GFX_POKEBALL = 87,
    OBJ_EVENT_GFX_UNUSED_WOMAN_1 = 88,
    OBJ_EVENT_GFX_UNUSED_WOMAN_2 = 89,
    OBJ_EVENT_GFX_UNUSED_WOMAN_3 = 90,
    OBJ_EVENT_GFX_MAP_SIGNPOST = 91,
    OBJ_EVENT_GFX_MAILBOX = 92,
    OBJ_EVENT_GFX_SIGNBOARD = 93,
    OBJ_EVENT_GFX_ARROW_SIGNPOST = 94,
    OBJ_EVENT_GFX_GYM_SIGNPOST = 95,
    OBJ_EVENT_GFX_TRAINER_TIPS_SIGNPOST = 96,
    OBJ_EVENT_GFX_PLAYER_F = 97,
    OBJ_EVENT_GFX_PLAYER_F_BIKE = 98,
    OBJ_EVENT_GFX_PROF_ROWAN = 99,
    OBJ_EVENT_GFX_BERRY_SOIL = 100,
    OBJ_EVENT_GFX_VAR_0 = 101,
    OBJ_EVENT_GFX_VAR_1 = 102,
    OBJ_EVENT_GFX_VAR_2 = 103,
    OBJ_EVENT_GFX_VAR_3 = 104,
    OBJ_EVENT_GFX_VAR_4 = 105,
    OBJ_EVENT_GFX_VAR_5 = 106,
    OBJ_EVENT_GFX_VAR_6 = 107,
    OBJ_EVENT_GFX_VAR_7 = 108,
    OBJ_EVENT_GFX_VAR_8 = 109,
    OBJ_EVENT_GFX_VAR_9 = 110,
    OBJ_EVENT_GFX_VAR_A = 111,
    OBJ_EVENT_GFX_VAR_B = 112,
    OBJ_EVENT_GFX_VAR_C = 113,
    OBJ_EVENT_GFX_VAR_D = 114,
    OBJ_EVENT_GFX_VAR_E = 115,
    OBJ_EVENT_GFX_VAR_F = 116,
    OBJ_EVENT_GFX_UNK_117 = 117,
    OBJ_EVENT_GFX_SNOWBALL = 118,
    OBJ_EVENT_GFX_UNK_119 = 119,
    OBJ_EVENT_GFX_CYRUS = 120,
    OBJ_EVENT_GFX_MARS = 121,
    OBJ_EVENT_GFX_SATURN = 122,
    OBJ_EVENT_GFX_JUPITER = 123,
    OBJ_EVENT_GFX_GRUNT_M = 124,
    OBJ_EVENT_GFX_GRUNT_F = 125,
    OBJ_EVENT_GFX_ROARK = 126,
    OBJ_EVENT_GFX_GARDENIA = 127,
    OBJ_EVENT_GFX_CRASHER_WAKE = 128,
    OBJ_EVENT_GFX_MAYLENE = 129,
    OBJ_EVENT_GFX_FANTINA = 130,
    OBJ_EVENT_GFX_CANDICE = 131,
    OBJ_EVENT_GFX_BYRON = 132,
    OBJ_EVENT_GFX_VOLKNER = 133,
    OBJ_EVENT_GFX_AARON = 134,
    OBJ_EVENT_GFX_BERTHA = 135,
    OBJ_EVENT_GFX_FLINT = 136,
    OBJ_EVENT_GFX_LUCIAN = 137,
    OBJ_EVENT_GFX_CYNTHIA = 138,
    OBJ_EVENT_GFX_UNUSED_MAN = 139,
    OBJ_EVENT_GFX_MOM = 140,
    OBJ_EVENT_GFX_CHERYL = 141,
    OBJ_EVENT_GFX_RILEY = 142,
    OBJ_EVENT_GFX_MARLEY = 143,
    OBJ_EVENT_GFX_BUCK = 144,
    OBJ_EVENT_GFX_MIRA = 145,
    OBJ_EVENT_GFX_DUMMY_146 = 146,
    OBJ_EVENT_GFX_DUMMY_147 = 147,
    OBJ_EVENT_GFX_BARRY = 148,
    OBJ_EVENT_GFX_UNUSED_BIG_MAN_1 = 149,
    OBJ_EVENT_GFX_UNUSED_BIG_MAN_2 = 150,
    OBJ_EVENT_GFX_UXIE = 151,
    OBJ_EVENT_GFX_MESPRIT = 152,
    OBJ_EVENT_GFX_AZELF = 153,
    OBJ_EVENT_GFX_DIALGA = 154,
    OBJ_EVENT_GFX_PALKIA = 155,
    OBJ_EVENT_GFX_ARCEUS = 156,
    OBJ_EVENT_GFX_DARKRAI = 157,
    OBJ_EVENT_GFX_SHAYMIN = 158,
    OBJ_EVENT_GFX_CRESSELIA = 159,
    OBJ_EVENT_GFX_GIRATINA_ALTERED = 160,
    OBJ_EVENT_GFX_HEATRAN = 161,
    OBJ_EVENT_GFX_DUMMY_162 = 162,
    OBJ_EVENT_GFX_RECEPTIONIST = 163,
    OBJ_EVENT_GFX_OLD_MAN = 164,
    OBJ_EVENT_GFX_OLD_WOMAN = 165,
    OBJ_EVENT_GFX_PROF_OAK = 166,
    OBJ_EVENT_GFX_JASMINE = 167,
    OBJ_EVENT_GFX_GYM_GUIDE = 168,
    OBJ_EVENT_GFX_PALMER = 169,
    OBJ_EVENT_GFX_UNUSED_BIG_MAN_3 = 170,
    OBJ_EVENT_GFX_UNUSED_BIG_MAN_4 = 171,
    OBJ_EVENT_GFX_UNUSED_BIG_MAN_5 = 172,
    OBJ_EVENT_GFX_STARLY = 173,
    OBJ_EVENT_GFX_BRIEFCASE = 174,
    OBJ_EVENT_GFX_MAID = 175,
    OBJ_EVENT_GFX_PLAYER_M_HOLDING_POKE_BALL = 176,
    OBJ_EVENT_GFX_PLAYER_F_HOLDING_POKE_BALL = 177,
    OBJ_EVENT_GFX_PLAYER_M_SURF = 178,
    OBJ_EVENT_GFX_PLAYER_F_SURF = 179,
    OBJ_EVENT_GFX_PLAYER_M_SPRAYDUCK = 180,
    OBJ_EVENT_GFX_PLAYER_F_SPRAYDUCK = 181,
    OBJ_EVENT_GFX_VENT = 182,
    OBJ_EVENT_GFX_BOOK = 183,
    OBJ_EVENT_GFX_REGIGIGAS = 184,
    OBJ_EVENT_GFX_DRIFLOON = 185,
    OBJ_EVENT_GFX_PLAYER_M_CONTEST = 186,
    OBJ_EVENT_GFX_PLAYER_F_CONTEST = 187,
    OBJ_EVENT_GFX_PLAYER_M_FISHING = 188,
    OBJ_EVENT_GFX_PLAYER_F_FISHING = 189,
    OBJ_EVENT_GFX_MOSS_ROCK = 190,
    OBJ_EVENT_GFX_ICE_ROCK = 191,
    OBJ_EVENT_GFX_BOLLARD = 192,
    OBJ_EVENT_GFX_MYSTERY_GIFT_DELIVERYMAN = 193,
    OBJ_EVENT_GFX_KID_WITH_NDS = 194,
    OBJ_EVENT_GFX_MAGIKARP = 195,
    OBJ_EVENT_GFX_PLAYER_M_POKETCH = 196,
    OBJ_EVENT_GFX_PLAYER_F_POKETCH = 197,
    OBJ_EVENT_GFX_PLAYER_M_SAVE = 198,
    OBJ_EVENT_GFX_PLAYER_F_SAVE = 199,
    OBJ_EVENT_GFX_PLAYER_M_POKECENTER_HEAL = 200,
    OBJ_EVENT_GFX_PLAYER_F_POKECENTER_HEAL = 201,
    OBJ_EVENT_GFX_GALACTIC_HQ_DOOR = 202,
    OBJ_EVENT_GFX_CAVE_PAINTING = 203,
    OBJ_EVENT_GFX_PACHIRISU = 204,
    OBJ_EVENT_GFX_SHROOMISH = 205,
    OBJ_EVENT_GFX_BUNEARY = 206,
    OBJ_EVENT_GFX_HAPPINY = 207,
    OBJ_EVENT_GFX_MACHOP = 208,
    OBJ_EVENT_GFX_ELITE_FOUR_ROOM_DOOR = 209,
    OBJ_EVENT_GFX_PLAYER_M_HEARTHOME_GYM = 210,
    OBJ_EVENT_GFX_PLAYER_F_HEARTHOME_GYM = 211,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_M = 212,
    OBJ_EVENT_GFX_LOOKER = 213,
    OBJ_EVENT_GFX_CHARON = 214,
    OBJ_EVENT_GFX_THORTON = 215,
    OBJ_EVENT_GFX_ARGENTA = 216,
    OBJ_EVENT_GFX_DARACH = 217,
    OBJ_EVENT_GFX_DAHLIA = 218,
    OBJ_EVENT_GFX_CAITLIN = 219,
    OBJ_EVENT_GFX_TURTWIG = 220,
    OBJ_EVENT_GFX_GROTLE = 221,
    OBJ_EVENT_GFX_TORTERRA = 222,
    OBJ_EVENT_GFX_CHIMCHAR = 223,
    OBJ_EVENT_GFX_MONFERNO = 224,
    OBJ_EVENT_GFX_INFERNAPE = 225,
    OBJ_EVENT_GFX_PIPLUP = 226,
    OBJ_EVENT_GFX_PRINPLUP = 227,
    OBJ_EVENT_GFX_EMPOLEON = 228,
    OBJ_EVENT_GFX_CROAGUNK = 229,
    OBJ_EVENT_GFX_GIRATINA_ORIGIN = 230,
    OBJ_EVENT_GFX_FRONTIER_SINGLE_ATTENDANT = 231,
    OBJ_EVENT_GFX_FRONTIER_MULTI_ATTENDANT = 232,
    OBJ_EVENT_GFX_FRONTIER_BOOTH_ATTENDANT = 233,
    OBJ_EVENT_GFX_WIFI_PLAZA_ATTENDANT_M = 234,
    OBJ_EVENT_GFX_WIFI_PLAZA_ATTENDANT_F = 235,
    OBJ_EVENT_GFX_PIKACHU_UNUSED = 236,
    OBJ_EVENT_GFX_ROTOM_HEAT = 237,
    OBJ_EVENT_GFX_ROTOM_FAN = 238,
    OBJ_EVENT_GFX_ROTOM_FROST = 239,
    OBJ_EVENT_GFX_ROTOM_WASH = 240,
    OBJ_EVENT_GFX_ROTOM_MOW = 241,
    OBJ_EVENT_GFX_GAME_DIRECTOR = 242,
    OBJ_EVENT_GFX_OVEN_UNUSED = 243,
    OBJ_EVENT_GFX_FAN_UNUSED = 244,
    OBJ_EVENT_GFX_FRIDGE_UNUSED = 245,
    OBJ_EVENT_GFX_WASHING_MACHINE_UNUSED = 246,
    OBJ_EVENT_GFX_LAWNMOWER_UNUSED = 247,
    OBJ_EVENT_GFX_GRUNTS_GROUP_OF_4 = 248,
    OBJ_EVENT_GFX_GRUNTS_GROUP_OF_3 = 249,
    OBJ_EVENT_GFX_CAVE_PAINTING_SHARDS_LEFT = 250,
    OBJ_EVENT_GFX_CAVE_PAINTING_SHARDS_RIGHT = 251,
    OBJ_EVENT_GFX_DP_PLAYER_M = 252,
    OBJ_EVENT_GFX_DP_PLAYER_F = 253,
    OBJ_EVENT_GFX_DP_PLAYER_M_CONTEST = 254,
    OBJ_EVENT_GFX_DP_PLAYER_F_CONTEST = 255,
    OBJ_EVENT_GFX_PLAYER_M_VS_SEEKER = 256,
    OBJ_EVENT_GFX_PLAYER_F_VS_SEEKER = 257,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_M_SURF = 258,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_F_SURF = 259,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_M_HOLDING_POKEBALL = 260,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_F_HOLDING_POKEBALL = 261,
    OBJ_EVENT_GFX_WALL_BLOCKING_ROTOMS_ROOM = 262,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_F = 263,
    OBJ_EVENT_GFX_DIST_WORLD_B1F_MESPRIT = 264,
    OBJ_EVENT_GFX_DIST_WORLD_B6F_UXIE = 265,
    OBJ_EVENT_GFX_DIST_WORLD_B6F_MESPRIT = 266,
    OBJ_EVENT_GFX_DIST_WORLD_B6F_AZELF = 267,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_M_SAVE = 268,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_F_SAVE = 269,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_M_POKETCH = 270,
    OBJ_EVENT_GFX_DIST_WORLD_PLAYER_F_POKETCH = 271,
    OBJ_EVENT_GFX_PLAYER_M_SAVE_HEARTHOME_GYM = 272,
    OBJ_EVENT_GFX_PLAYER_F_SAVE_HEARTHOME_GYM = 273,
    OBJ_EVENT_GFX_PLAYER_M_POKETCH_HEARTHOME_GYM = 274,
    OBJ_EVENT_GFX_PLAYER_F_POKETCH_HEARTHOME_GYM = 275,
    OBJ_EVENT_GFX_BERRY_SPROUT = 4096,
    OBJ_EVENT_GFX_INVISIBLE = 8192,
    OBJ_EVENT_GFX_SENTINEL_ID = 65535,
};
typedef struct Options Options;
typedef struct BagCursor BagCursor;
typedef struct JournalEntry JournalEntry;
PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);

BOOL ov80_0222DCF0(FrontierScriptContext *ctx);
BOOL FrtCmd_064(FrontierScriptContext *ctx);

BOOL ov80_0222DCF0(FrontierScriptContext *ctx) {
    BOOL v0;
    UnkStruct_ov104_02231148 *v1 = Frontier_GetData(ctx->frontierSystem->frontier);

    v0 = ov80_0223B9EC[v1->unk_08](v1);

    if (v0 == 0) {
        sub_0200FBF4(SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
        sub_0200FBF4(SCREEN_SUB, (((0) << 10) | ((0) << 5) | (0)));
        Frontier_SetData(ctx->frontierSystem->frontier, v1->unk_14);
        Heap_Free(v1);
    }

    return !v0;
}

BOOL FrtCmd_064(FrontierScriptContext *ctx) {
    u16 objEventGfx;
    FieldFrontierDTO *fieldData;
    u16 *destVar = FrontierScript_ReadVarPtr(ctx);

    fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);

    if (PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldData->saveData)) == GENDER_MALE) {
        objEventGfx = OBJ_EVENT_GFX_PLAYER_M;
    } else {
        objEventGfx = OBJ_EVENT_GFX_PLAYER_F;
    }

    *destVar = objEventGfx;
    return 0;
}
