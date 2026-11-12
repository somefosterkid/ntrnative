// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_actor_internal.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_FIELD_ACTOR_INTERNAL_H
#define BWPORT_FIELD_FIELD_ACTOR_INTERNAL_H

// The layouts of the actors and their system. Names and layouts from swan's FieldActor and FieldActorSystem
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "field/field_actor.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// A position on the grid
typedef struct {
    s16 x;
    s16 y;
    s16 z;
} GPosXYZ;

// Black 2's; twinned code confirms the offsets of flags, movementFlags, uid, zoneId, modelId, moveCode, evType,
// spawnFlag, scrId, defaultDir, faceDir, motionDir, lastFaceDir, lastMotionDir, param0, param1, param2, nextAcmd,
// areaW, areaH, defaultGPos, initGPos, gPos, wPos, wPosOffset, unk5C, unk68, currentTileUnder, currentTileUnderOrigY,
// collisionWidth, collisionHeight, modelPosOffsetX, modelPosOffsetY, modelPosOffsetZ, tcb, actorSystem, moveCodeVTable,
// sceneNodeVTable, defaultRailPos, unkA4 and config, and its size in Black
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
    // The position before the last move
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
    // The move code's work, which starts with the actor's place on a rail (Black's: 0x10 bytes, the place and a u32)
    union {
        ActorPositionRail defaultRailPos;
        u8 moveCodeWork[FIELD_ACTOR_MOVE_WORK_SIZE];
    };
    u8 unkA4[FIELD_ACTOR_EVTYPE_WORK_SIZE];
    u8 unkB4[FIELD_ACTOR_ACMD_WORK_SIZE];
    u8 blActWork[FIELD_ACTOR_BLACT_WORK_SIZE];
    FieldActorConfig config;
};

// Black 2's; twinned code confirms the offsets of flags, actorCapacity, actorCount and actorHeap in Black. Black's
// actorHeap is at 0x18, straight after dataArc, so there is no actorConfigArc before it
struct MMSys {
    u32 flags;
    u16 actorCapacity;
    s16 actorCount;
    HeapID heapId;
    HeapID fieldHeapId;
    u16 unkC;
    u16 unkE;
    // Black's: arc 0x31, which FldActSys_AttachField opens
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

// The UID of the player's actor
#define ACTOR_UID_PLAYER 0xff

// Black 2's; twinned code confirms the offsets of onCreate, update, suspend, wakeUp and getBlActIdx in Black
// The functions of an actor's kind of scene node, by its config's sceneNodeType: onDelete when the actor is deleted
// or its model is changed, suspend and wakeUp around a suspension of the system
struct FieldActorSceneNodeVTable {
    void (*onCreate)(FieldActor *actor);
    void (*update)(FieldActor *actor);
    void (*onDelete)(FieldActor *actor);
    void (*suspend)(FieldActor *actor);
    void (*wakeUp)(FieldActor *actor);
    u32 (*getBlActIdx)(FieldActor *actor, u32 param);
};

// The move code executors and the main vtables, by move code and scene node type (ov021's tables)
extern const FieldActorMoveCode *const data_ov021_021d5e04[];
extern const u32 *const ACMD_DIR_LUT[];
extern const FieldActorSceneNodeVTable *const data_ov021_021d770c[];
// The scene node kinds' functions that data_ov021_021d770c lists
extern const FieldActorSceneNodeVTable data_ov021_021d8558;
extern const FieldActorSceneNodeVTable data_ov021_021d8570;
extern const FieldActorSceneNodeVTable data_ov021_021d8588;
extern const FieldActorSceneNodeVTable data_ov021_021d85a0;
extern const FieldActorSceneNodeVTable data_ov021_021d85b8;
extern const FieldActorSceneNodeVTable data_ov021_021d85d0;
extern const FieldActorSceneNodeVTable data_ov021_021d85e8;
extern const FieldActorSceneNodeVTable data_ov021_021d8600;
extern const FieldActorSceneNodeVTable data_ov021_021d8618;
extern const FieldActorSceneNodeVTable data_ov021_021d8630;
extern const FieldActorSceneNodeVTable data_ov021_021d8648;
extern const FieldActorSceneNodeVTable data_ov021_021d8660;
extern const FieldActorSceneNodeVTable data_ov021_021d8678;
extern const FieldActorSceneNodeVTable data_ov021_021d8690;
extern const FieldActorSceneNodeVTable data_ov021_021d86a8;
extern const FieldActorSceneNodeVTable data_ov021_021d86c0;
extern const FieldActorSceneNodeVTable data_ov021_021d86d8;
extern const FieldActorSceneNodeVTable data_ov021_021d86f0;
typedef u32 (*FieldActorFunc)(FieldActor *);
extern const FieldActorFunc *const ACTOR_ACTION_COMMAND_TABLES[];
extern const FieldActorFunc *const ACTOR_ACTION_COMMAND_TABLES_RAIL[];
extern const FieldActorFunc data_ov021_021d4180[];
extern const FieldActorFunc data_ov021_021d41c0[];
extern const FieldActorFunc data_ov021_021d4170[];
extern const FieldActorFunc data_ov021_021d4198[];
extern const FieldActorFunc data_ov021_021d4a14[];
extern const FieldActorFunc data_ov021_021d4a08[];
extern const FieldActorFunc data_ov021_021d49fc[];
extern const FieldActorFunc data_ov021_021d49f0[];
extern const FieldActorFunc data_ov021_021d49e4[];
extern const FieldActorFunc data_ov021_021d49d8[];
extern const FieldActorFunc data_ov021_021d49cc[];
extern const FieldActorFunc data_ov021_021d49c0[];
extern const FieldActorFunc data_ov021_021d49b4[];
extern const FieldActorFunc data_ov021_021d49a8[];
extern const FieldActorFunc data_ov021_021d499c[];
extern const FieldActorFunc data_ov021_021d4990[];
extern const FieldActorFunc data_ov021_021d4984[];
extern const FieldActorFunc data_ov021_021d4978[];
extern const FieldActorFunc data_ov021_021d496c[];
extern const FieldActorFunc data_ov021_021d4960[];
extern const FieldActorFunc data_ov021_021d4954[];
extern const FieldActorFunc data_ov021_021d4948[];
extern const FieldActorFunc data_ov021_021d493c[];
extern const FieldActorFunc data_ov021_021d4930[];
extern const FieldActorFunc data_ov021_021d48f4[];
extern const FieldActorFunc data_ov021_021d48e8[];
extern const FieldActorFunc data_ov021_021d48dc[];
extern const FieldActorFunc data_ov021_021d48d0[];
extern const FieldActorFunc data_ov021_021d48c4[];
extern const FieldActorFunc data_ov021_021d48b8[];
extern const FieldActorFunc data_ov021_021d48ac[];
extern const FieldActorFunc data_ov021_021d48a0[];
extern const FieldActorFunc data_ov021_021d4894[];
extern const FieldActorFunc data_ov021_021d4888[];
extern const FieldActorFunc data_ov021_021d487c[];
extern const FieldActorFunc data_ov021_021d4870[];
extern const FieldActorFunc data_ov021_021d4864[];
extern const FieldActorFunc data_ov021_021d4858[];
extern const FieldActorFunc data_ov021_021d484c[];
extern const FieldActorFunc data_ov021_021d4840[];
extern const FieldActorFunc data_ov021_021d4834[];
extern const FieldActorFunc data_ov021_021d4828[];
extern const FieldActorFunc data_ov021_021d481c[];
extern const FieldActorFunc data_ov021_021d4810[];
extern const FieldActorFunc data_ov021_021d4804[];
extern const FieldActorFunc data_ov021_021d47f8[];
extern const FieldActorFunc data_ov021_021d47ec[];
extern const FieldActorFunc data_ov021_021d47e0[];
extern const FieldActorFunc data_ov021_021d47d4[];
extern const FieldActorFunc data_ov021_021d4468[];
extern const FieldActorFunc data_ov021_021d47bc[];
extern const FieldActorFunc data_ov021_021d47b0[];
extern const FieldActorFunc data_ov021_021d47a4[];
extern const FieldActorFunc data_ov021_021d4798[];
extern const FieldActorFunc data_ov021_021d478c[];
extern const FieldActorFunc data_ov021_021d4780[];
extern const FieldActorFunc data_ov021_021d4774[];
extern const FieldActorFunc data_ov021_021d4768[];
extern const FieldActorFunc data_ov021_021d475c[];
extern const FieldActorFunc data_ov021_021d4750[];
extern const FieldActorFunc data_ov021_021d472c[];
extern const FieldActorFunc data_ov021_021d47c8[];
extern const FieldActorFunc data_ov021_021d46fc[];
extern const FieldActorFunc data_ov021_021d46f0[];
extern const FieldActorFunc data_ov021_021d46e4[];
extern const FieldActorFunc data_ov021_021d46d8[];
extern const FieldActorFunc data_ov021_021d46cc[];
extern const FieldActorFunc data_ov021_021d46c0[];
extern const FieldActorFunc data_ov021_021d43d8[];
extern const FieldActorFunc data_ov021_021d41b0[];
extern const FieldActorFunc data_ov021_021d4160[];
extern const FieldActorFunc data_ov021_021d4190[];
extern const FieldActorFunc data_ov021_021d4168[];
extern const FieldActorFunc data_ov021_021d41a0[];
extern const FieldActorFunc data_ov021_021d4188[];
extern const FieldActorFunc data_ov021_021d4660[];
extern const FieldActorFunc data_ov021_021d4624[];
extern const FieldActorFunc data_ov021_021d4618[];
extern const FieldActorFunc data_ov021_021d460c[];
extern const FieldActorFunc data_ov021_021d4600[];
extern const FieldActorFunc data_ov021_021d45c4[];
extern const FieldActorFunc data_ov021_021d45b8[];
extern const FieldActorFunc data_ov021_021d45ac[];
extern const FieldActorFunc data_ov021_021d45a0[];
extern const FieldActorFunc data_ov021_021d4924[];
extern const FieldActorFunc data_ov021_021d4918[];
extern const FieldActorFunc data_ov021_021d490c[];
extern const FieldActorFunc data_ov021_021d4900[];
extern const FieldActorFunc data_ov021_021d4594[];
extern const FieldActorFunc data_ov021_021d4588[];
extern const FieldActorFunc data_ov021_021d457c[];
extern const FieldActorFunc data_ov021_021d4570[];
extern const FieldActorFunc data_ov021_021d4744[];
extern const FieldActorFunc data_ov021_021d4738[];
extern const FieldActorFunc data_ov021_021d4708[];
extern const FieldActorFunc data_ov021_021d4714[];
extern const FieldActorFunc data_ov021_021d4564[];
extern const FieldActorFunc data_ov021_021d4558[];
extern const FieldActorFunc data_ov021_021d454c[];
extern const FieldActorFunc data_ov021_021d4540[];
extern const FieldActorFunc data_ov021_021d4504[];
extern const FieldActorFunc data_ov021_021d44a4[];
extern const FieldActorFunc data_ov021_021d4498[];
extern const FieldActorFunc data_ov021_021d4654[];
extern const FieldActorFunc data_ov021_021d448c[];
extern const FieldActorFunc data_ov021_021d4474[];
extern const FieldActorFunc data_ov021_021d4414[];
extern const FieldActorFunc data_ov021_021d4438[];
extern const FieldActorFunc data_ov021_021d4444[];
extern const FieldActorFunc data_ov021_021d445c[];
extern const FieldActorFunc data_ov021_021d4678[];
extern const FieldActorFunc data_ov021_021d469c[];
extern const FieldActorFunc data_ov021_021d46a8[];
extern const FieldActorFunc data_ov021_021d4720[];
extern const FieldActorFunc data_ov021_021d4a20[];
extern const FieldActorFunc data_ov021_021d43fc[];
extern const FieldActorFunc data_ov021_021d43f0[];
extern const FieldActorFunc data_ov021_021d4384[];
extern const FieldActorFunc data_ov021_021d424c[];
extern const FieldActorFunc data_ov021_021d436c[];
extern const FieldActorFunc data_ov021_021d4360[];
extern const FieldActorFunc data_ov021_021d4354[];
extern const FieldActorFunc data_ov021_021d4348[];
extern const FieldActorFunc data_ov021_021d433c[];
extern const FieldActorFunc data_ov021_021d4330[];
extern const FieldActorFunc data_ov021_021d4324[];
extern const FieldActorFunc data_ov021_021d4318[];
extern const FieldActorFunc data_ov021_021d430c[];
extern const FieldActorFunc data_ov021_021d4300[];
extern const FieldActorFunc data_ov021_021d42f4[];
extern const FieldActorFunc data_ov021_021d42e8[];
extern const FieldActorFunc data_ov021_021d42c4[];
extern const FieldActorFunc data_ov021_021d42d0[];
extern const FieldActorFunc data_ov021_021d4408[];
extern const FieldActorFunc data_ov021_021d4420[];
extern const FieldActorFunc data_ov021_021d4450[];
extern const FieldActorFunc data_ov021_021d4684[];
extern const FieldActorFunc data_ov021_021d46b4[];
extern const FieldActorFunc data_ov021_021d4a2c[];
extern const FieldActorFunc data_ov021_021d4288[];
extern const FieldActorFunc data_ov021_021d41c8[];
extern const FieldActorFunc data_ov021_021d4270[];
extern const FieldActorFunc data_ov021_021d43b4[];
extern const FieldActorFunc data_ov021_021d4258[];
extern const FieldActorFunc data_ov021_021d41ec[];
extern const FieldActorFunc data_ov021_021d43e4[];
extern const FieldActorFunc data_ov021_021d427c[];
extern const FieldActorFunc data_ov021_021d43cc[];
extern const FieldActorFunc data_ov021_021d43c0[];
extern const FieldActorFunc data_ov021_021d466c[];
extern const FieldActorFunc data_ov021_021d4264[];
extern const FieldActorFunc data_ov021_021d439c[];
extern const FieldActorFunc data_ov021_021d4390[];
extern const FieldActorFunc data_ov021_021d4240[];
extern const FieldActorFunc data_ov021_021d4234[];
extern const FieldActorFunc data_ov021_021d421c[];
extern const FieldActorFunc data_ov021_021d4228[];
extern const FieldActorFunc data_ov021_021d42a0[];
extern const FieldActorFunc data_ov021_021d42ac[];
extern const FieldActorFunc data_ov021_021d43a8[];
extern const FieldActorFunc data_ov021_021d463c[];
extern const FieldActorFunc data_ov021_021d4630[];
extern const FieldActorFunc data_ov021_021d4480[];
extern const FieldActorFunc data_ov021_021d45f4[];
extern const FieldActorFunc data_ov021_021d4378[];
extern const FieldActorFunc data_ov021_021d45dc[];
extern const FieldActorFunc data_ov021_021d45d0[];
extern const FieldActorFunc data_ov021_021d4534[];
extern const FieldActorFunc data_ov021_021d4528[];
extern const FieldActorFunc data_ov021_021d451c[];
extern const FieldActorFunc data_ov021_021d4510[];
extern const FieldActorFunc data_ov021_021d42dc[];
extern const FieldActorFunc data_ov021_021d41a8[];
extern const FieldActorFunc data_ov021_021d4690[];
extern const FieldActorFunc data_ov021_021d4a38[];
extern const FieldActorFunc data_ov021_021d41b8[];
extern const FieldActorFunc data_ov021_021d41d4[];
extern const FieldActorFunc data_ov021_021d4294[];
extern const FieldActorFunc data_ov021_021d42b8[];
extern const FieldActorFunc data_ov021_021d442c[];
extern const FieldActorFunc data_ov021_021d4178[];
extern const FieldActorFunc data_ov021_021d44f8[];
extern const FieldActorFunc data_ov021_021d44ec[];
extern const FieldActorFunc data_ov021_021d44e0[];
extern const FieldActorFunc data_ov021_021d44bc[];
extern const FieldActorFunc data_ov021_021d44b0[];
extern const FieldActorFunc data_ov021_021d44d4[];
extern const FieldActorFunc data_ov021_021d44c8[];
extern const FieldActorFunc data_ov021_021d41f8[];
extern const FieldActorFunc data_ov021_021d8104[];
extern const FieldActorFunc data_ov021_021d810c[];
extern const FieldActorFunc data_ov021_021d80fc[];
extern const FieldActorFunc data_ov021_021d80f4[];
extern const FieldActorFunc data_ov021_021d8114[];
extern const FieldActorFunc data_ov021_021d811c[];
extern const FieldActorFunc data_ov021_021d8128[];
extern const FieldActorFunc data_ov021_021d8134[];
extern const FieldActorFunc data_ov021_021d8140[];
extern const FieldActorFunc data_ov021_021d814c[];
extern const FieldActorFunc data_ov021_021d8158[];
extern const FieldActorFunc data_ov021_021d8164[];
extern const FieldActorFunc data_ov021_021d8170[];
extern const FieldActorFunc data_ov021_021d817c[];
extern const FieldActorFunc data_ov021_021d8188[];
extern const FieldActorFunc data_ov021_021d8194[];
extern const FieldActorFunc data_ov021_021d81a0[];
extern const FieldActorFunc data_ov021_021d81ac[];
extern const FieldActorFunc data_ov021_021d81b8[];
extern const FieldActorFunc data_ov021_021d81c4[];
extern const FieldActorFunc data_ov021_021d81d0[];
extern const FieldActorFunc data_ov021_021d81dc[];
extern const FieldActorFunc data_ov021_021d81e8[];
extern const FieldActorFunc data_ov021_021d81f4[];
extern const FieldActorFunc data_ov021_021d8200[];
extern const FieldActorFunc data_ov021_021d820c[];
extern const FieldActorFunc data_ov021_021d8218[];
extern const FieldActorFunc data_ov021_021d8224[];
extern const FieldActorFunc data_ov021_021d8230[];
extern const FieldActorFunc data_ov021_021d823c[];
extern const FieldActorFunc data_ov021_021d8260[];
extern const FieldActorFunc data_ov021_021d826c[];
extern const FieldActorFunc data_ov021_021d8314[];
extern const FieldActorFunc data_ov021_021d8320[];
extern const FieldActorFunc data_ov021_021d832c[];
extern const FieldActorFunc data_ov021_021d8338[];
extern const FieldActorFunc data_ov021_021d8344[];
extern const FieldActorFunc data_ov021_021d8350[];
extern const FieldActorFunc data_ov021_021d8308[];
extern const FieldActorFunc data_ov021_021d82fc[];
extern const FieldActorFunc data_ov021_021d82f0[];
extern const FieldActorFunc data_ov021_021d82e4[];
extern const FieldActorFunc data_ov021_021d82d8[];
extern const FieldActorFunc data_ov021_021d82cc[];
extern const FieldActorFunc data_ov021_021d82c0[];
extern const FieldActorFunc data_ov021_021d82b4[];
extern const FieldActorFunc data_ov021_021d82a8[];
extern const FieldActorFunc data_ov021_021d829c[];
extern const FieldActorFunc data_ov021_021d8290[];
extern const FieldActorFunc data_ov021_021d8284[];
extern const FieldActorFunc data_ov021_021d8278[];
extern const FieldActorFunc data_ov021_021d8248[];
extern const FieldActorFunc data_ov021_021d8254[];

#endif // BWPORT_FIELD_FIELD_ACTOR_INTERNAL_H
