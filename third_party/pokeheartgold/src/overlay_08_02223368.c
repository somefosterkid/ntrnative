#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_bag.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "overworld_anim_manager.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sys_task.h"
#include "bg_window.h"
#include "filesystem.h"
#include "msgdata.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_07_ball_rotation.h"
#include "battle/battle_cursor.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_bag_context.h"

struct UnkBallData;
struct BattleContext;
struct BattleSystem;
struct BgConfig;
struct TextOBJ;
struct MsgData;
struct NARC;
struct SpriteList;
struct MessageFormat;
struct SysTask;
struct SysTaskQueue;
struct UnkStruct_ov101_021D13C8_t;
struct UnkStruct_ov101_021D7E48_t;
typedef struct UnkStruct_ov101_021D13C8_t UnkStruct_ov101_021D13C8;
typedef enum {
    UnkEnum_ov101_021D9688_00 = 0,
    UnkEnum_ov101_021D9688_01,
    UnkEnum_ov101_021D9688_02,
    UnkEnum_ov101_021D9688_03,
    UnkEnum_ov101_021D9688_04,
    UnkEnum_ov101_021D9688_05,
    UnkEnum_ov101_021D9688_06,
} UnkEnum_ov101_021D9688;
typedef enum {
    UnkEnum_ov101_021D2BDC_00,
    UnkEnum_ov101_021D2BDC_01,
    UnkEnum_ov101_021D2BDC_02,
    UnkEnum_ov101_021D2BDC_03,
} UnkEnum_ov101_021D2BDC;
typedef enum {
    UnkEnum_ov101_021D4F58_00 = 0,
    UnkEnum_ov101_021D4F58_01,
    UnkEnum_ov101_021D4F58_02,
    UnkEnum_ov101_021D4F58_03,
    UnkEnum_ov101_021D4F58_04,
    UnkEnum_ov101_021D4F58_05,
} UnkEnum_ov101_021D4F58;
typedef enum {
    UnkEnum_ov101_021D394C_00 = 0,
    UnkEnum_ov101_021D394C_01,
    UnkEnum_ov101_021D394C_02,
} UnkEnum_ov101_021D394C;
typedef enum {
    UnkEnum_ov101_021D5814_00 = 0,
    UnkEnum_ov101_021D5814_01,
    UnkEnum_ov101_021D5814_02,
    UnkEnum_ov101_021D5814_03,
    UnkEnum_ov101_021D5814_04,
    UnkEnum_ov101_021D5814_05,
} UnkEnum_ov101_021D5814;
typedef enum {
    UnkEnum_ov101_021D6764_00 = 0,
    UnkEnum_ov101_021D6764_01,
    UnkEnum_ov101_021D6764_02,
    UnkEnum_ov101_021D6764_03,
} UnkEnum_ov101_021D6764;
typedef enum {
    UnkEnum_ov101_021D9934_00 = 0,
    UnkEnum_ov101_021D9934_01,
    UnkEnum_ov101_021D9934_02,
    UnkEnum_ov101_021D9934_03,
    UnkEnum_ov101_021D9934_04,
    UnkEnum_ov101_021D9934_05,
    UnkEnum_ov101_021D9934_06,
} UnkEnum_ov101_021D9934;
typedef enum {
    UnkEnum_ov101_021D9934_1_00 = 0,
    UnkEnum_ov101_021D9934_1_01,
    UnkEnum_ov101_021D9934_1_02,
    UnkEnum_ov101_021D9934_1_03,
} UnkEnum_ov101_021D9934_1;
typedef enum {
    UnkEnum_ov101_021D1894_00,
    UnkEnum_ov101_021D1894_01,
    UnkEnum_ov101_021D1894_02,
    UnkEnum_ov101_021D1894_03,
} UnkEnum_ov101_021D1894;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
typedef struct {
    MsgData *unk_00;
    MessageFormat *unk_04;
    Window unk_08[1];
    void *unk_18;
} UnkStruct_ov101_021D148C;
typedef struct {
    u32 unk_00;
    int unk_04;
    UnkEnum_ov101_021D1894 unk_08;
    UnkEnum_ov101_021D1894 unk_0C;
    SysTask *unk_10;
} UnkStruct_ov101_021D1894;
typedef struct {
    u32 unk_00;
    int unk_04;
    fx32 unk_08;
    int unk_0C;
    UnkEnum_ov101_021D2BDC unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
    UnkStruct_ov101_021D13C8 *unk_20;
} UnkStruct_ov101_021D2D88;
typedef struct {
    u32 unk_00;
    u32 unk_04;
    int unk_08;
    UnkStruct_ov101_021D13C8 *unk_0C;
    SysTask *unk_10;
    OverworldAnimManager *unk_14;
    OverworldAnimManager *unk_18;
} UnkStruct_ov101_021D4714;
typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    int unk_10;
    int unk_14;
    UnkEnum_ov101_021D5814 unk_18;
    UnkEnum_ov101_021D6764 unk_1C;
    UnkStruct_ov101_021D13C8 *unk_20;
    SysTask *unk_24;
    OverworldAnimManager *unk_28;
    OverworldAnimManager *unk_2C;
    OverworldAnimManager *unk_30;
    OverworldAnimManager *unk_34;
    OverworldAnimManager *unk_38;
    OverworldAnimManager *unk_3C;
    OverworldAnimManager *unk_40;
    OverworldAnimManager *unk_44;
} UnkStruct_ov101_021D4764;
typedef struct {
    UnkStruct_ov101_021D13C8 *unk_00;
    u16 unk_04[16];
    u16 unk_24[16];
    u16 unk_44[16];
    u16 unk_64[16];
    u16 unk_84[16];
    u16 unk_A4[5][16];
    u16 unk_144[5][16];
    u16 unk_1E4[5][16];
} UnkStruct_ov101_021D4F58;
typedef struct UnkStruct_ov101_021D7E48_t UnkStruct_ov101_021D7E48;
struct UnkStruct_ov101_021D13C8_t {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
    u32 unk_34;
    u32 unk_38;
    u32 unk_3C;
    u32 unk_40;
    UnkEnum_ov101_021D9688 unk_44;
    u32 unk_48;
    UnkEnum_ov101_021D9934_1 unk_4C;
    UnkEnum_ov101_021D9934 unk_50;
    u32 unk_54[3];
    int unk_60;
    int unk_64;
    int unk_68;
    fx32 unk_6C;
    int unk_70;
    u32 unk_74;
    u32 unk_78;
    UnkEnum_ov101_021D394C unk_7C[3];
    int unk_88;
    fx32 unk_8C;
    fx32 unk_90[3];
    fx32 unk_9C[3];
    int unk_A8[3];
    UnkStruct_ov101_021D2D88 unk_B4[3];
    UnkStruct_ov101_021D4714 unk_120;
    UnkStruct_ov101_021D4764 unk_13C;
    UnkStruct_ov101_021D4F58 unk_184;
    UnkStruct_ov101_021D148C unk_408;
    UnkStruct_ov101_021D1894 unk_424;
    NARC *unk_438;
    BgConfig *unk_43C;
    NNSG2dScreenData *unk_440;
    NNSG2dCharacterData *unk_444;
    NNSG2dPaletteData *unk_448;
    OverworldAnimManagerList *unk_44C;
    UnkStruct_ov101_021D7E48 *unk_450;
    OverworldAnimManager *unk_454;
    u32 unk_458;
    u32 unk_45C;
    u32 unk_460;
    u32 unk_464;
    u32 unk_468;
    u32 unk_46C;
    u32 unk_470;
    u32 unk_474;
    u32 unk_478;
    u32 unk_47C;
    u32 unk_480;
    u32 unk_484;
    u32 unk_488;
    u32 unk_48C;
    u32 unk_490;
    u32 unk_494;
    u32 unk_498;
    u32 unk_49C;
    u32 unk_4A0;
    u32 unk_4A4;
    u32 unk_4A8;
    u32 unk_4AC;
    u32 unk_4B0;
    u32 unk_4B4;
    u32 unk_4B8;
    u32 unk_4BC;
    u32 unk_4C0;
    int msgBoxFrame;
};
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    fx32 unk_0C;
    UnkEnum_ov101_021D4F58 unk_10;
    fx32 unk_14[5];
    UnkStruct_ov101_021D13C8 *unk_28;
    UnkStruct_ov101_021D4F58 *unk_2C;
} UnkStruct_ov101_021D5388;
extern int (*const *const Unk_ov101_021D8774[13])(UnkStruct_ov101_021D4764 *);
void TouchscreenHitbox_FindRectAtTouchNew(UnkStruct_ov101_021D5388 *param0);
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
typedef struct Indicator {
    ManagedSprite *sprite;
    SysTask *animTask;
    int x;
    int y;
    int bounceAngle;
    fx32 subscreenOffset;
    s16 exitTimer;
    u8 exitPending;
    u8 hasDropped;
    u8 isExiting;
    u8 exitStep;
    u8 exitFrameCount;
} Indicator;
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
typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
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
int ov12_022581D4(BattleSystem *battleSys, BattleContext *battleCtx, enum BattleContextParam paramID, int battler);
BattleContext *BattleSystem_GetBattleContext(BattleSystem *battleSys);

void ov08_02223368(OverworldAnimManager *param0, void *param1);
int ov08_02223374(BattleBag *battleBag);

void ov08_02223368(OverworldAnimManager *param0, void *param1) {
    UnkStruct_ov101_021D5388 *v0 = param1;
    TouchscreenHitbox_FindRectAtTouchNew(v0);
}

int ov08_02223374(BattleBag *battleBag) {
    return ov12_022581D4(battleBag->context->battleSys, BattleSystem_GetBattleContext(battleBag->context->battleSys), BATTLECTX_SELECTED_PARTY_SLOT, battleBag->context->battler);
}
