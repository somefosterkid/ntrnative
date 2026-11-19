#include "global.h"

struct ActorPositionRail;

struct ArcTool;

struct Field;

struct FieldActor;

struct FieldActorMoveCode;

struct FieldActorSceneNodeVTable;

struct FieldG3DMapper;

struct GameData;

struct MMSys;

struct NoGridMapper;

struct RailPosition;

struct RailUnit;

struct StrengthRockSave;

struct TCB;

struct TCBManager;

typedef struct ArcTool ArcTool;

typedef struct Field Field;

typedef struct FieldActor FieldActor;

typedef struct FieldG3DMapper FieldG3DMapper;

typedef struct GameData GameData;

typedef struct MMSys MMSys;

typedef struct NoGridMapper NoGridMapper;

typedef struct RailPosition RailPosition;

typedef struct RailUnit RailUnit;

typedef struct TCBManager TCBManager;

typedef struct TCB TCB;

typedef struct ActorPositionRail ActorPositionRail;

typedef struct FieldActorMoveCode FieldActorMoveCode;

typedef struct FieldActorSceneNodeVTable FieldActorSceneNodeVTable;

typedef struct StrengthRockSave StrengthRockSave;

typedef u16 HeapID;

struct RailPosition {
    u16 componentId;

    union {
        struct {
            u8 componentIsLine;
            u8 railDirection;
        };
        u16 lineAndDirection;
    };
    s16 posSide;
    u16 posFront;
};

s32 GetDeltaXByFacingDirection(u32 direction);

u16 sub_02061200(s32 x1, s32 z1, s32 x2, s32 z2);

s32 GetDeltaYByFacingDirection(s32 direction);

typedef struct {
    u16 res1;
    u16 res2;
    u16 animations[3];
} FieldActorResGroup;

typedef struct {
    u16 uid;
    u8 entityType;
    u8 sceneNodeType;
    u8 enableShadow;
    u8 footprintType;
    u8 enableReflections;
    u8 billboardSize;
    u8 spriteAtlasSize;
    u8 spriteControllerType;
    u8 gender;
    u8 collWidth;
    u8 collHeight;
    s8 wPosOffsetX;
    s8 wPosOffsetY;
    s8 wPosOffsetZ;
    FieldActorResGroup rscIndices;
    u16 padding;
} FieldActorConfig;

u32 MapObject_GetXCoord(FieldActor *actor);

s16 MapObject_GetZCoord(FieldActor *actor);

s16 MapObject_GetPreviousXCoord(FieldActor *actor);

s16 MapObject_GetPreviousZCoord(FieldActor *actor);

struct ActorPositionRail {
    union {

        u32 unk0;

        RailUnit *unit;
    };
    RailPosition position;
};

struct FieldActorMoveCode {
    u32 code;
    void (*unk4)(FieldActor *actor);
    void (*unk8)(FieldActor *actor);
    void (*unkC)(FieldActor *actor);
    void (*unk10)(FieldActor *actor);
};

struct StrengthRockSave {
    VecFx32 pos[30];
};

void MapObject_ForceSetHeldMovement(FieldActor *actor, u32 acmd);

u16 sub_0206234C(u16 dir, u16 acmd);

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} GPosXYZ;

struct FieldActor {
    u32 flags;
    u32 movementFlags;
    u16 uid;
    u16 zoneId;
    u16 modelId;
    u16 moveCode;
    u16 evType;
    u16 spawnFlag;
    u16 scrId;
    u16 defaultDir;
    u16 faceDir;
    u16 motionDir;
    u16 lastFaceDir;
    u16 lastMotionDir;
    u16 param0;
    u16 param1;
    u16 param2;
    u16 nextAcmd;
    u16 acmdState;
    u16 unk2A;
    s16 areaW;
    s16 areaH;
    GPosXYZ defaultGPos;

    GPosXYZ initGPos;
    GPosXYZ gPos;
    s16 unk42;
    VecFx32 wPos;
    VecFx32 wPosOffset;
    VecFx32 unk5C;
    VecFx32 unk68;
    u32 currentTileUnder;
    u32 currentTileUnderOrigY;
    u8 collisionWidth;
    u8 collisionHeight;
    s8 modelPosOffsetX;
    s8 modelPosOffsetY;
    s8 modelPosOffsetZ;
    u8 shadowGroup;
    u16 padShadowGroup;
    TCB *tcb;
    MMSys *actorSystem;
    const FieldActorMoveCode *moveCodeVTable;
    const FieldActorSceneNodeVTable *sceneNodeVTable;

    union {
        ActorPositionRail defaultRailPos;
        u8 moveCodeWork[(0x10)];
    };
    u8 unkA4[0x10];
    u8 unkB4[0x10];
    u8 blActWork[0x20];
    FieldActorConfig config;
};

struct MMSys {
    u32 flags;
    u16 actorCapacity;
    s16 actorCount;
    HeapID heapId;
    HeapID fieldHeapId;
    u16 unkC;
    u16 unkE;

    ArcTool *unk10;
    ArcTool *dataArc;
    FieldActor *actorHeap;
    void *tcbMgrHeap;
    TCBManager *tcbMgr;
    void *fieldBlAct;
    void *actorG3DSystem;
    StrengthRockSave *mmodelSave;
    GameData *gameData;
    FieldG3DMapper *g3dMapper;
    NoGridMapper *noGridMapper;
    Field *field;
    u16 *cameraAngle;
    u16 *actorConfigCache;
};

struct FieldActorSceneNodeVTable {
    void (*onCreate)(FieldActor *actor);
    void (*update)(FieldActor *actor);
    void (*onDelete)(FieldActor *actor);
    void (*suspend)(FieldActor *actor);
    void (*wakeUp)(FieldActor *actor);
    u32 (*getBlActIdx)(FieldActor *actor, u32 param);
};

typedef struct {
    u8 state;
    u8 active;
    s16 x;
    s16 z;
    s16 unk6;
    FieldActor *target;
} ActorFollowWork;

u32 sub_0206623C(FieldActor *param_1, ActorFollowWork *work);

u32 sub_0206623C(FieldActor *param_1, ActorFollowWork *work) {
    int x;
    s32 z;
    int ox;
    s16 oz;
    s16 ix;
    s16 iz;
    u16 dir;
    u16 acmd;

    x = MapObject_GetXCoord(param_1);
    z = MapObject_GetZCoord(param_1);
    ox = MapObject_GetXCoord(work->target);
    oz = MapObject_GetZCoord(work->target);
    ix = MapObject_GetPreviousXCoord(work->target);
    iz = MapObject_GetPreviousZCoord(work->target);
    if (x == ox && z == oz) {
        return 0;
    }
    dir = sub_02061200(x, z, ix, iz);
    x += GetDeltaXByFacingDirection((u16)dir);
    z += GetDeltaYByFacingDirection((u16)dir);
    if (x != ox || z != oz) {
        acmd = sub_0206234C(dir, 0xc);
        MapObject_ForceSetHeldMovement(param_1, acmd);
        return 1;
    }
    return 0;
}
