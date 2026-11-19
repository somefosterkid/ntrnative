#include "global.h"
#include "sys_task.h"

struct ManagedSprite;
struct NARC;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
struct SysTask;
struct SysTaskQueue;
enum HeapID {
    HEAP_ID_SYSTEM = 0,
    HEAP_ID_SAVE,
    HEAP_ID_DEBUG,
    HEAP_ID_APPLICATION,
    HEAP_ID_FIELD1,
    HEAP_ID_BATTLE,
    HEAP_ID_BAG,
    HEAP_ID_POKETCH_MAIN,
    HEAP_ID_POKETCH_APP,
    HEAP_ID_BOX_DATA,
    HEAP_ID_BOX_GRAPHICS,
    HEAP_ID_FIELD2,
    HEAP_ID_PARTY_MENU,
    HEAP_ID_13,
    HEAP_ID_14,
    HEAP_ID_COMMUNICATION,
    HEAP_ID_16,
    HEAP_ID_TOWN_MAP,
    HEAP_ID_NAMING_SCREEN_APP,
    HEAP_ID_POKEMON_SUMMARY_SCREEN,
    HEAP_ID_20,
    HEAP_ID_21,
    HEAP_ID_22,
    HEAP_ID_23,
    HEAP_ID_24,
    HEAP_ID_TRAINER_CASE,
    HEAP_ID_TRADE_ROOM,
    HEAP_ID_27,
    HEAP_ID_28,
    HEAP_ID_MINING,
    HEAP_ID_DISTORTION_WORLD_WARP,
    HEAP_ID_31,
    HEAP_ID_FIELD3,
    HEAP_ID_UNDERGROUND,
    HEAP_ID_34,
    HEAP_ID_35,
    HEAP_ID_BERRY_TAG,
    HEAP_ID_POKEDEX,
    HEAP_ID_OPTIONS_MENU,
    HEAP_ID_DRAWING,
    HEAP_ID_MAIL,
    HEAP_ID_MAIL_VIEWER,
    HEAP_ID_JOURNAL,
    HEAP_ID_43,
    HEAP_ID_POFFIN_CASE,
    HEAP_ID_POFFIN_MANAGER,
    HEAP_ID_POFFIN_CUTSCENE,
    HEAP_ID_CHOOSE_STARTER_APP,
    HEAP_ID_48,
    HEAP_ID_NINTENDO_WFC,
    HEAP_ID_50,
    HEAP_ID_51,
    HEAP_ID_52,
    HEAP_ID_53,
    HEAP_ID_54,
    HEAP_ID_POFFIN_BERRY_SELECTION,
    HEAP_ID_56,
    HEAP_ID_TRADE_SEQUENCE,
    HEAP_ID_TRADE_SEQUENCE_PHASE,
    HEAP_ID_59,
    HEAP_ID_60,
    HEAP_ID_PC_HALL_OF_FAME,
    HEAP_ID_62,
    HEAP_ID_HALL_OF_FAME,
    HEAP_ID_64,
    HEAP_ID_65,
    HEAP_ID_66,
    HEAP_ID_MOVE_REMINDER,
    HEAP_ID_68,
    HEAP_ID_69,
    HEAP_ID_70,
    HEAP_ID_EGG_HATCH,
    HEAP_ID_BOAT_CUTSCENE,
    HEAP_ID_EVOLUTION,
    HEAP_ID_BATTLE_TOWER_RECORDS_APP,
    HEAP_ID_END_CREDITS,
    HEAP_ID_76,
    HEAP_ID_GAME_START,
    HEAP_ID_MIGRATE_FROM_GBA,
    HEAP_ID_79,
    HEAP_ID_RANKINGS_MACHINE,
    HEAP_ID_MAIN_MENU,
    HEAP_ID_ROWAN_INTRO,
    HEAP_ID_ROWAN_INTRO_TV,
    HEAP_ID_DIPLOMA,
    HEAP_ID_RANGER_LINK,
    HEAP_ID_MYSTERY_GIFT_APP,
    HEAP_ID_WONDER_CARDS_APP,
    HEAP_ID_88,
    HEAP_ID_89,
    HEAP_ID_90,
    HEAP_ID_NETWORK_ICON,
    HEAP_ID_92,
    HEAP_ID_BATTLE_FACTORY_APP,
    HEAP_ID_94,
    HEAP_ID_95,
    HEAP_ID_96,
    HEAP_ID_97,
    HEAP_ID_BATTLE_HALL_APP,
    HEAP_ID_99,
    HEAP_ID_BATTLE_CASTLE_APP,
    HEAP_ID_101,
    HEAP_ID_102,
    HEAP_ID_BATTLE_ARCADE_APP,
    HEAP_ID_104,
    HEAP_ID_105,
    HEAP_ID_106,
    HEAP_ID_107,
    HEAP_ID_108,
    HEAP_ID_109,
    HEAP_ID_110,
    HEAP_ID_SPEAR_PILLAR_CUTSCENE,
    HEAP_ID_112,
    HEAP_ID_113,
    HEAP_ID_FRONTIER_RECORD_APP,
    HEAP_ID_SCRATCH_OFF_CARD_APP,
    HEAP_ID_116,
    HEAP_ID_117,
    HEAP_ID_118,
    HEAP_ID_119,
    HEAP_ID_LIBRARY_TV,
    HEAP_ID_PARTY_MENU1,
    HEAP_ID_PARTY_MENU2,
    HEAP_ID_MAX,
};
typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;
typedef struct NARC NARC;
typedef struct PaletteData PaletteData;
typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;
typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;
typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
typedef struct {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;
void *Heap_Alloc(u32 heapID, u32 size);
typedef struct Vec2F32 {
    f32 x;
    f32 y;
} Vec2F32;

UnkStruct_0208B878 *sub_02087284(UnkStruct_0208B284 param0, SpriteSystem *param1, SpriteManager *param2, PaletteData *param3);
BOOL sub_020872DC(s16 centerX, s16 centerY, f32 particleX, f32 particleY, f32 *outX, f32 *outY, f32 length, s16 minDistanceToCenter);

UnkStruct_0208B878 *sub_02087284(UnkStruct_0208B284 param0, SpriteSystem *param1, SpriteManager *param2, PaletteData *param3) {
    UnkStruct_0208B878 *v0 = Heap_Alloc(param0.heapID, sizeof(UnkStruct_0208B878));
    v0->unk_10.unk_00 = param0.unk_00;
    v0->unk_10.heapID = (enum HeapID)param0.heapID;
    v0->unk_10.unk_08 = param0.unk_08;
    v0->unk_10.unk_0C = param0.unk_0C;
    v0->unk_10.unk_10 = param0.unk_10;
    v0->unk_00.unk_04 = param1;
    v0->unk_00.unk_08 = param2;
    v0->unk_00.unk_0C = param3;
    v0->unk_E4 = 0;
    v0->unk_FC = (15 * 1);
    v0->unk_100 = (15 * 1);
    v0->unk_F4 = 0L;
    v0->unk_F0 = 0;

    return v0;
}

BOOL sub_020872DC(s16 centerX, s16 centerY, f32 particleX, f32 particleY, f32 *outX, f32 *outY, f32 length, s16 minDistanceToCenter) {
    Vec2F32 outVec, vecToCenter, zeroVec;

    zeroVec.x = 0;
    zeroVec.y = 0;

    vecToCenter.x = (centerX - particleX);
    vecToCenter.y = (centerY - particleY);

    outVec.x = 0;
    outVec.y = 0;

    f32 distSquared = (vecToCenter.x * vecToCenter.x) + (vecToCenter.y * vecToCenter.y);

    f32 dist = ((f32)((FX_Sqrt((fx32)(((distSquared) > 0) ? ((distSquared) * (1 << 12) + 0.5f) : ((distSquared) * (1 << 12) - 0.5f)))) / (f32)(1 << 12)));

    if (dist < length || minDistanceToCenter > dist || dist == 0) {
        return 0;
    }

    outVec.x = (vecToCenter.x * length) / dist;
    outVec.y = (vecToCenter.y * length) / dist;

    *outX = (outVec.x + zeroVec.x);
    *outY = (outVec.y + zeroVec.y);

    return 1;
}
