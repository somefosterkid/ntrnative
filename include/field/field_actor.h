// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_actor.h, the lines in use so
// far. pokebw2's func_ov036_02195a58 is Black's func_ov021_0219e764, and its func_ov012_02167580, func_ov012_0216763c
// and func_ov012_02167788 are Black's func_ov010_0216dae0, func_ov010_0216db4c and func_ov010_0216dc98; its
// func_ov012_02167520, func_ov012_021673f8, func_ov012_0216731c, func_ov012_02167d88 and func_ov012_02167da8 are
// Black's func_ov010_0216dab4, func_ov010_0216d98c, func_ov010_0216d8b0, func_ov010_0216e1a4 and func_ov010_0216e1c4.
#ifndef BWPORT_FIELD_FIELD_ACTOR_H
#define BWPORT_FIELD_FIELD_ACTOR_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/zone.h"
#include "gfl/heap.h"
#include "gfl/blact.h"
#include "work_area.h"

// The resources of an object code's model
typedef struct {
    u16 res1;
    u16 res2;
    u16 animations[3];
} FieldActorResGroup;

// An object code's record in ARCID_MMODEL_TBL, from 4 bytes into the file, at the index GetIndexOfObjID returns. Names
// from swan's FieldActorConfig
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

// What tells one actor from another, for finding it again
typedef struct {
    u16 uid;
    u16 objCode;
    u16 zoneId;
} FieldActorIdentity;

u16 GetActorFaceDir(FieldActor *actor);
void ExpandVecInGridDir(u16 dir, VecFx32 *pos, fx32 distance);
// pokebw2's func_ov012_021670f4
void func_ov010_0216d674(FieldActor *actor, u32 value);
// pokebw2's func_ov012_02166ecc
BOOL func_ov010_0216d454(FieldActor *actor);
void DisableAllActorsMovement(MMSys *mmSys);
void EnableAllActorsMovement(MMSys *mmSys);
void SetActorHidden(FieldActor *actor, BOOL hidden);
void CheckSetActorFaceDir(FieldActor *actor, u16 dir);
FieldActor *FindPlayerFieldActor(MMSys *mmSys);
BOOL IsActorFlag16(FieldActor *actor);
FieldActor *FindActorByMoveCode(MMSys *mmSys, u16 code);
// Ends the actor's movement command
void func_ov010_0216d4b4(FieldActor *actor);
void SetActorWPosAll(FieldActor *actor, const VecFx32 *pos, u16 dir);
void SetActorMovementFlag(FieldActor *actor, u32 flag);
void ClearActorMovementFlag(FieldActor *actor, u32 flag);
void func_ov021_0219e764(FieldActor *actor, VecFx16 *dest);
void DisableActorMovement(FieldActor *actor);
void EnableActorMovement(FieldActor *actor);
void ClearActorFlag(FieldActor *actor, u32 flag);
void func_ov010_0216dae0(FieldActor *actor, BOOL value);
void FldAct_SetAcmd(FieldActor *actor, u16 acmd);
BOOL func_ov010_0216d480(FieldActor *actor);
void SetActorWPosValue(FieldActor *actor, const VecFx32 *pos);
FieldActor *FindFieldActor(MMSys *mmSys, u16 id);
// The rail unit that moves the actor along the rails
RailUnit *FldAct_GetRailUnit(FieldActor *actor);
void SetActorInitedPositionRail(FieldActor *actor, const RailPosition *position);
void func_ov010_0216db4c(FieldActor *actor, BOOL a1);
void func_ov010_0216dc98(FieldActor *actor, BOOL value);
BOOL func_ov010_0216dab4(FieldActor *actor);
MMSys *GetActorMModelSystem(FieldActor *actor);
void CopyActorWPos(FieldActor *actor, VecFx32 *dest);
BOOL CheckActorMovementFlag(FieldActor *actor, u32 flag);
BOOL func_ov010_0216d98c(MMSys *system);
const FieldActorConfig *GetActorMdlInfo(FieldActor *actor);
void func_ov010_0216d8b0(FieldActor *actor, VecFx32 *dest);
void CopyActorPosOffset(FieldActor *actor, VecFx32 *offset);
void func_ov010_0216e1a4(FieldActor *actor, FieldActorIdentity *identity);
BOOL func_ov010_0216e1c4(FieldActor *actor, const FieldActorIdentity *identity);

// Whether the actor has finished its movement commands
BOOL IsAllActorAcmdFinished(FieldActor *actor);
VecFx32 *GetMModelWPosPtr(FieldActor *actor);
u32 GetZoneNPCsCount(EventData *eventData);
void SetZoneNPCLocation(EventData *eventData, u32 npcId, u16 direction, u16 x, s32 y, u16 z);
void SetZoneNPCMdlID(EventData *eventData, u16 npcId, u16 objCode);
void SetZoneNPCSCRID(EventData *eventData, u16 npcId, u16 scriptId);
void FldAct_InvokeUpdateCallback(FieldActor *actor);
void FldAct_GetGPos(FieldActor *actor, GridPos *pos);
u16 GetActorMotionDir(FieldActor *actor);
void SetActorGPosX(FieldActor *actor, s16 x);
void SetActorGPosZ(FieldActor *actor, s16 z);
s16 GetGPosX(FieldActor *actor);
s16 GetGPosZ(FieldActor *actor);
void SetActorGPos(FieldActor *actor, s16 x, s16 y, s16 z, u16 dir);
void SetActorMotionDir(FieldActor *actor, u16 dir);
BOOL GetTerrainAtPosByActor(FieldActor *actor, const VecFx32 *position, MapTerrainBuf *terrain);
// The actor's user parameters 0 to 2
u16 GetActorUserParam(FieldActor *actor, u32 index);
void SetActorUserParam(FieldActor *actor, u16 value, u32 index);
void SetActorSCRID(FieldActor *actor, u16 scriptId);
FieldActor *CreateNewActorByParam(MMSys *mmSys, s16 x, s16 z, u16 dir, u16 id, u16 objCode, u16 moveCode, u32 zoneId);
// Sets x and z to the center of a tile, leaving y
void ConvGXZToVector(s16 x, s16 z, VecFx32 *pos);
void SpawnAllZoneNPCs(MMSys *mmSys, ZoneNPC *npcs, s32 zoneId, u32 count, EventWork *eventWork);
u16 GetActorMoveCode(FieldActor *actor);
s16 GetActorWalkAreaW(FieldActor *actor);
s16 GetActorWalkAreaH(FieldActor *actor);
ActorPositionRail *GetNPCRailPosPtrAddr(FieldActor *actor);
ActorPositionRail *ClearActorPositionBlock(FieldActor *actor, u32 size);
void SetCachedTileUnderActor(FieldActor *actor, u32 tileType);
void SetCachedOrigYTileUnderActor(FieldActor *actor, u32 tileType);
u32 GetCachedTileUnderActor(FieldActor *actor);
u32 GetCachedOrigYTileUnderActor(FieldActor *actor);
s16 GetActorDefaultGPosX(FieldActor *actor);
s16 GetActorDefaultGPosZ(FieldActor *actor);
s16 FldAct_GetInitGPosX(FieldActor *actor);
void SetActorInitialGPosX(FieldActor *actor, s16 x);
s16 FldAct_GetInitGPosY(FieldActor *actor);
void SetActorInitialGPosY(FieldActor *actor, s16 y);
s16 FldAct_GetInitGPosZ(FieldActor *actor);
void SetActorInitialGPosZ(FieldActor *actor, s16 z);
void adjustXPos(FieldActor *actor, s16 dx);
s16 FldAct_GetGPosY(FieldActor *actor);
void SetActorGPosY(FieldActor *actor, s16 y);
void adjustYPos(FieldActor *actor, s16 dz);
fx32 GetActorPosY(FieldActor *actor);
u8 GetActorCollWidth(FieldActor *actor);
u8 GetActorCollHeight(FieldActor *actor);
void FldAct_SetReflectingFlag(FieldActor *actor, BOOL value);
BOOL FldAct_CheckReflectingFlag(FieldActor *actor);
u32 CheckMMSysFlag(MMSys *system, u32 flag);
// The collision flags at a grid position, with the world position's y
u32 ActorRouteCollCheckCore(FieldActor *actor, const VecFx32 *position, s16 x, s16 y, s16 z, u16 dir);
u32 ActorRouteCollCheck(FieldActor *actor, s16 x, s16 y, s16 z, u16 dir);
BOOL IsGPosOutsideActorWalkArea(FieldActor *actor, s16 x, s16 z);
BOOL CheckBlockedCollPathToPosition(FieldActor *actor, u16 dir, VecFx32 position);
BOOL GetTileTypeAtPosByActor(FieldActor *actor, const VecFx32 *position, u32 *tileType);
BOOL GetHeightFromMap(FieldActor *actor, const VecFx32 *position, fx32 *height);
void SetActorInitialGPosToNowGPos(FieldActor *actor);
BOOL CheckRecalcActorY(FieldActor *actor);
BOOL FldAct_CacheTerrainInfo(FieldActor *actor);
void SetRailActorFlag(FieldActor *actor);
void InitRailActor(FieldActor *actor, const ZoneNPC *npc);
void SetActorInitPositionRail(FieldActor *actor, const RailPosition *position);
void SetActorEvType(FieldActor *actor, u16 evType);
u16 GetActorSpawnFlag(FieldActor *actor);
u32 GetDefaultActorDir(FieldActor *actor);
void SetActorFaceDir(FieldActor *actor, u16 dir);
void SetActorAreaW(FieldActor *actor, s16 w);
void SetActorAreaH(FieldActor *actor, s16 h);
void SetNextActorAcmd(FieldActor *actor, u16 acmd);
void SetActorWPosOffset(FieldActor *actor, const VecFx32 *offset);
void SetActorFlag32(FieldActor *actor, BOOL value);
FieldActor *GetFirstActorOnGPos(MMSys *system, s16 x, s16 z, BOOL checkInit);

// An actor of a zone's entities, from which actors are created. Names and layout from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
// Field names from swan's ZoneNPCPositionGrid and ZoneNPCPositionRail.
struct ZoneNPCGridPosition {
    u16 x;
    u16 z;
    s32 y;
};

struct ZoneNPCRailPosition {
    u16 railIndex;
    u16 frontPos;
    s16 sidePos;
};

struct ZoneNPC {
    u16 uid;
    u16 modelId;
    u16 moveCode;
    u16 evType;
    u16 spawnFlag;
    u16 scrId;
    u16 direction;
    u16 param0;
    u16 param1;
    u16 param2;
    u16 areaW;
    u16 areaH;
    BOOL isRail;
    union {
        ZoneNPCGridPosition grid;
        ZoneNPCRailPosition rail;
    } pos;
};

// Moves grid coordinates or a position by a distance in a direction
void AdjusGridXZByDir(u32 dir, s16 *x, s16 *z, s16 distance);

// The sizes of an actor's works (FieldActor, field_actor_internal.h). Its move code's (GetNPCRailPosPtrAddr,
// ClearActorPositionBlock): Black's 0x10 bytes, which the rail move codes' and the comm actors' works fill and outgrow
// on a PC target, where they hold pointers (fldmmdl_rail.c, field_comm_actor.c)
#define FIELD_ACTOR_MOVE_WORK_SIZE WORK_AREA_SIZE(0x10)
// Its event type's (func_ov010_0216d6a0), its action command's (func_ov010_0216d6bc) and its billboard's
// (FldAct_ResetBlActWork), which its works with pointers still fit on a PC target
#define FIELD_ACTOR_EVTYPE_WORK_SIZE 0x10
#define FIELD_ACTOR_ACMD_WORK_SIZE 0x10
#define FIELD_ACTOR_BLACT_WORK_SIZE 0x20

// An actor's place on a rail, from swan's ActorPositionRail, at the start of the work of its move code
struct ActorPositionRail {
    union {
        // ACTOR_RAIL_POS_SET once the place is set (field_actor_tool.c), until a rail move code takes a unit
        u32 unk0;
        // The rail unit that moves the actor, while a rail move code runs (fldmmdl_rail.c, pokebw2's ActorRailWork)
        RailUnit *unit;
    };
    RailPosition position;
};

// The direction the actor faced before
u16 func_ov010_0216d5fc(FieldActor *actor);

// Black 2's; twinned code confirms the offsets of x, y and z in Black
// A position on the grid of 16-unit tiles. The game copies it as 8 bytes, with a field after z
struct GridPos {
    s16 x;
    s16 y;
    s16 z;
    s16 unk6;
};

// Sets a ZoneNPC's position on the grid
void func_ov010_0216e618(ZoneNPC *npc, u16 x, u16 z, s32 y);

#define ACTOR_RAIL_POS_SET 0xefefefef

// What the terrain effects of an actor look at
struct ActorTerrainEffectParam {
    u32 tileTypeOrigY;
    u32 tileType;
    u16 tileClassOrigY;
    u16 tileClass;
    u16 tileFlagsOrigY;
    u16 tileFlags;
    const FieldActorConfig *config;
    void *effects;
    u8 season;
};

// The collision flags of the tile next to the actor in a direction
u32 ActorRouteCollCheckOneTileInDir(FieldActor *actor, u16 dir);
void ActorTerrainEffect_InitParam(FieldActor *actor, ActorTerrainEffectParam *param);
void ActorTerrainEffect_TallGrass(FieldActor *actor, ActorTerrainEffectParam *param);
// Call the move code's unk4 and unk8 functions
void func_ov010_0216d6f4(FieldActor *actor);
u32 func_ov010_0216da20(FieldActor *actor, u32 flag);
BOOL func_ov010_0216db10(FieldActor *actor);
void func_ov010_0216d708(FieldActor *actor);
void func_ov010_0216d71c(FieldActor *actor);
void func_ov010_0216d76c(FieldActor *actor);
void func_ov010_0216dfac(FieldActor *actor);
BOOL func_ov010_0216db68(FieldActor *actor);
BOOL func_ov010_0216dcb4(FieldActor *actor);
// Sets movement flag 0x20
void func_ov010_0216da70(FieldActor *actor);
BOOL func_ov010_0216db24(FieldActor *actor);
BOOL func_ov010_0216dc00(FieldActor *actor);
void func_ov010_0216d900(FieldActor *actor, const VecFx32 *offset);
BOOL func_ov010_0216dbe8(FieldActor *actor);
void func_ov010_0216dbcc(FieldActor *actor, BOOL value);
BOOL func_ov010_0216dd04(FieldActor *actor);
void func_ov010_02163010(FieldActor *actor);
void func_ov010_02163028(FieldActor *actor);
BOOL func_ov010_02163090(FieldActor *actor);
void func_ov010_021630f4(FieldActor *actor);
void func_ov010_0216310c(FieldActor *actor);
void func_ov010_02163130(FieldActor *actor);
void func_ov010_02163154(FieldActor *actor);
void func_ov010_02163188(FieldActor *actor);
void func_ov010_02163230(FieldActor *actor);
void func_ov010_021633a8(FieldActor *actor);
void func_ov010_02163418(FieldActor *actor, ActorTerrainEffectParam *param);
u32 func_ov010_02163464(u32 tileClass, u8 season);
void func_ov010_021634d8(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_0216352c(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_0216355c(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_02163580(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_02163618(FieldActor *actor, ActorTerrainEffectParam *param);
BOOL func_ov010_021636a8(FieldActor *actor, ActorTerrainEffectParam *param);
BOOL func_ov010_021636c8(ActorTerrainEffectParam *param);
void func_ov010_021636f0(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_0216372c(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_0216375c(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_021637e4(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_02163868(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_021638ac(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_02163e7c(FieldActor *actor, u16 dir);
u32 func_ov010_02163f40(FieldActor *actor, u16 dir);
void func_ov010_02163f8c(FieldActor *actor, const VecFx32 *offset);
void func_ov010_02163fc0(FieldActor *actor, u16 dir, fx32 distance);
BOOL func_ov010_02164380(FieldActor *actor, RailPosition *position);

// Black 2's; twinned code confirms the offsets of unk4, unk8 and unk10 in Black
// An actor's move code and its functions, called with the actor: unkC before the code is changed
struct FieldActorMoveCode {
    u32 code;
    void (*unk4)(FieldActor *actor);
    void (*unk8)(FieldActor *actor);
    void (*unkC)(FieldActor *actor);
    void (*unk10)(FieldActor *actor);
};

// Black 2's, not checked against Black
// The saved positions of the strength boulders that were moved, in grid units, by the slots of fldmmdl.c's table of
// boulders. A slot without a saved position holds STRENGTH_ROCK_POS_NONE
#define STRENGTH_ROCK_SLOT_COUNT 30

// Black 2's, not checked against Black
#define STRENGTH_ROCK_POS_NONE (-1)

// Black 2's, not checked against Black
struct StrengthRockSave {
    VecFx32 pos[STRENGTH_ROCK_SLOT_COUNT];
};

void ChangeActorDirection(FieldActor *actor, u16 dir);
// The actor's current movement command
u16 FldAct_GetAcmd(FieldActor *actor);
// Updates until the materials have loaded
void FldActSys_ForceFullSync(MMSys *system);
// The 0x10-byte block an NPC's rail move keeps: the fields func_ov021_0219e00c sets, as the bytes it stores them
typedef struct NPCRailMove {
    s32 unk_0;
    s32 unk_4;
    u16 unk_8;
    u16 unk_a;
    union {
        u8 unk_c;
        struct {
            u8 state : 4;
            u8 dir : 4;
        };
    };
    s8 unk_d;
    s8 unk_e;
    u8 unk_f;
} NPCRailMove;

// The work of a jump along the rail (func_ov021_0219e0a8 updates it)
typedef struct ActorRailJumpWork {
    s32 speed;
    s32 progress;
    u16 step;
    u16 phase;
    s8 dir;
    s8 timer;
    s8 unk_e;
    s8 height;
} ActorRailJumpWork;

void *FldActSys_GetFieldBlAct(MMSys *system);
u16 GetActorEvType(FieldActor *actor);
void *FldAct_GetBlActWorkPtr(FieldActor *actor);
u32 FldAct_GetBlActIdx(FieldActor *actor, u32 param);
void FieldActor_NextAcmdState(FieldActor *actor);
u16 FldAct_GetAcmdState(FieldActor *actor);
void adjustZPos(FieldActor *actor, s16 dy);
// Whether another actor has the object code
BOOL FldAct_CheckObjCodeShared(FieldActor *actor, u16 objCode);
const FieldActorResGroup *GetNPCMdlInfoG2DRscGroup(const FieldActorConfig *config);
// Creates the actor of the entity with an ID, if it is spawned
FieldActor *func_ov010_0216cee4(MMSys *mmSys, ZoneNPC *npcs, s32 zoneId, u32 count, EventWork *eventWork, u16 uid);
// Starts a movement command
void func_ov010_0216d438(FieldActor *actor, u16 acmd);
// The base priority of the actors' tasks
u16 func_ov010_0216d4f4(MMSys *system);
// pokebw2's GetMMSysTCBMgr
void *func_ov010_0216d520(MMSys *system);
// pokebw2's GetMMSysNoGridMapper
NoGridMapper *FldActSys_GetNoGridMapper(MMSys *system);
void FldActSys_InitConfigArc(MMSys *system, HeapID heapId);
void FldActSys_FreeConfigArc(MMSys *system);
u16 func_ov010_0216d678(FieldActor *actor);
void *func_ov010_0216d6b8(FieldActor *actor);
void *func_ov010_0216d6d4(FieldActor *actor);
// The state of the actor's movement command
void func_ov010_0216d7c8(FieldActor *actor, u16 state);
void func_ov010_0216d8d0(FieldActor *actor, VecFx32 *dest);
void func_ov010_0216d8e0(FieldActor *actor, const VecFx32 *value);
void func_ov010_0216d8f0(FieldActor *actor, VecFx32 *dest);
void func_ov010_0216d95c(MMSys *system, BOOL flag);
void func_ov010_0216d974(MMSys *system, BOOL flag);
// Set movement flag 0x10, clear it, and clear flag 0x40
void func_ov010_0216da44(FieldActor *actor);
void func_ov010_0216da7c(FieldActor *actor);
BOOL func_ov010_0216daa0(FieldActor *actor);
void func_ov010_0216dc64(FieldActor *actor, BOOL value);
void func_ov010_0216dccc(FieldActor *actor, BOOL value);
void func_ov010_0216dce8(FieldActor *actor, BOOL value);
// How an actor of the event type looks for the player: the types that see only the way they face give 1
u16 func_ov010_0216e434(u16 type);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
ZoneNPC *GetZoneNPCs(EventData *eventData);
void GetNPCMdlInfoForOBJCODE(const MMSys *actorSystem, u16 objCode, FieldActorConfig *config);
void SetActorFlag(FieldActor *actor, u32 flag);
u16 GetActorUID(FieldActor *actor);
u16 FldAct_GetSCRID(FieldActor *actor);
u16 FldAct_GetObjCode(FieldActor *actor);
// Makes the actor the one an entry describes
void FldAct_Transplant(FieldActor *actor, const ZoneNPC *npc);
// The actor's rail position, the one a step ahead of it, and the one a step in a direction, FALSE if there is none
void GetActorRailPos(FieldActor *actor, RailPosition *position);
u32 GetIndexOfObjID(u16 objCode);
void DeleteActor(FieldActor *actor);
void ChangeActorMoveCodeSeq(FieldActor *actor, u16 moveCode);
FieldG3DMapper *GetMMSysG3DMapper(MMSys *system);
Field *GetMMSysField(MMSys *mmSys);
// The movement command of a direction in the row of a table that has the command
u16 GetAcmdForDir(u16 dir, u16 acmd);
u16 GetActorZoneID(FieldActor *actor);
// Steps through the system's actors from *index, returning TRUE with the next one in *actor
BOOL NextActor(const MMSys *mmSys, FieldActor **actor, u32 *index);
FieldActor *CreateNewActorByEntityNoWKOBJCODE(MMSys *mmSys, const ZoneNPC *npc, u32 zoneId);
// The position with every offset added
void CopyActorPosAllAdd(FieldActor *actor, VecFx32 *dest);
// The other Trainer of a double battle pair
FieldActor *FindPairedTrainerActor(FieldActor *actor);
void FldActSys_Update(MMSys *system);
void FldActSys_FinishAsyncMatLoadSafe(MMSys *system);
BOOL FldActSys_IsAsyncLoadPending(MMSys *system);
BOOL CheckActorFlag(FieldActor *actor, u32 flag);

static inline BOOL IsActorFlagClear(FieldActor *actor, u32 flag) {
    if (CheckActorFlag(actor, flag) == FALSE) {
        return TRUE;
    }
    return FALSE;
}
BOOL CheckActorNewPosOtherActorCollision(FieldActor *actor, s16 x, s16 y, s16 z);
BOOL CheckActorVolumeOverGPos(FieldActor *actor, s16 x, s16 z, BOOL checkInit);
TCBManager *GetMMSysTCBMgr(MMSys *system);
// The first actor over a grid position whose height is within maxHeightDiff of y, other than exclude
FieldActor *FindActorByGPos(MMSys *system, s16 x, s16 z, fx32 y, fx32 maxHeightDiff, BOOL checkInit);
// The first actor on a rail position
FieldActor *FindActorByRailPos(MMSys *system, const RailPosition *position, BOOL checkInit);
// The object code that an object code stands for, through the work values of WKOBJCODE00 and on
u16 ResolvePossibleWKOBJCODE(EventWork *eventWork, u16 objCode);
u16 func_ov010_0216d538(MMSys *system);
// Whether the actor exists
BOOL func_ov010_0216da30(FieldActor *actor);
void func_ov021_0219f058(FieldActor *actor, u16 *a1, u16 *a2);
BOOL func_ov010_0216dc4c(FieldActor *actor);
void func_ov021_0219f0b0(FieldActor *actor, u16 a1, u16 a2);
void func_ov021_0219b414(FieldActor *actor);
void func_ov021_02197eb4(FieldActor *actor);
BOOL func_ov021_0219b41c(FieldActor *actor);
u32 func_ov010_0216d564(FieldActor *actor);
void func_ov010_02163650(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_0216369c(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_0216377c(FieldActor *actor, ActorTerrainEffectParam *param);
void func_ov010_021637b0(FieldActor *actor, ActorTerrainEffectParam *param);
// Whether the player is within 17 tiles of the actor on both axes
BOOL func_ov010_02164168(FieldActor *actor);
void func_ov010_021641c8(FieldActor *actor);
void FldActSys_DeleteAllActors(MMSys *mmSys);
void FldAct_UpdateBlInfoForNewObjCode(FieldActor *actor, u16 objCode);
void FldActSys_AttachField(MMSys *system, HeapID heapId, GameData *gameData, Field *field, FieldG3DMapper *mapper,
                           NoGridMapper *noGridMapper);
void FldActSys_FinishAsyncMatLoad(MMSys *system);
void FldActSys_SuspendAllActors(MMSys *system);
void DeleteAllActors(MMSys *system);
MMSys *FldActSys_Create(HeapID heapId, u32 capacity, void *save);
void FldActSys_Free(MMSys *system);
u16 GetActorLimit(const MMSys *system);
HeapID FldActSys_GetFieldHeapID(MMSys *system);
void *FldAct_ResetBlActWork(FieldActor *actor, u32 size);
void *FldAct_GetFieldBlAct(FieldActor *actor);
void GetMMSysMdlInfoCacheEntry(MMSys *system, u32 index, FieldActorConfig *config);
u16 GetMMSysMdlInfoCacheEntryCount(MMSys *system);
const FieldActorResGroup *GetNPCMdlInfoG3DRscGroup(const FieldActorConfig *config);
void FldActSys_UpdateResourceLoader(MMSys *system);
void func_ov021_02197c4c(MMSys *system);
void func_ov021_0219d6b8(FieldActor *actor);
void func_ov021_0219d6cc(FieldActor *actor);
void func_ov021_02197c78(FieldActor *actor);
void func_ov010_0216cd50(MMSys *system);
void func_ov010_0216cdb8(MMSys *system, u16 *cameraAngle);
void func_ov010_0216d2f8(MMSys *system);
// Work areas of the actor, cleared to a size and returned
void *func_ov010_0216d6a0(FieldActor *actor, u32 size);
void *func_ov010_0216d6bc(FieldActor *actor, u32 size);
void func_ov010_0216da50(FieldActor *actor);
void func_ov010_0216e464(FieldActor *actor);
void func_ov010_0216e468(FieldActor *actor);
void func_ov010_0216e46c(FieldActor *actor);
void func_ov010_0216e470(FieldActor *actor);
// The size of the strength boulders' save, and its initialization
u32 func_ov010_0216e474(void);
void func_ov010_0216e47c(StrengthRockSave *save);
// Saves the position of the actor's boulder
void func_ov010_0216e57c(FieldActor *actor);
// Sets a ZoneNPC's position on a rail
void func_ov010_0216e62c(GameData *gameData, PartyPkm *pkm, u32 species);
// Calls the function, if there is one, and returns its address inverted (DS Protect's callback)
uintptr_t func_ov010_0216e680(void (*callback)(void));
u8 func_ov021_0219e634(FieldActor *actor, RailPosition *position);
// The unit vector of a direction along the actor's rail
void func_ov021_0219e784(FieldActor *actor, u32 dir, VecFx16 *dest);
// Sets the frame of the model's animation for a time. FALSE if the actor has no model
BOOL func_ov021_0219f7cc(FieldActor *actor, u16 time);
// The billboard actors' material colors, read from a file of archive 0xaf after the handle (pokebw2's)
typedef struct FldActMatColorPreset {
    ArcTool *arcTool;
    s32 normal[3];
    u16 diffuse[3];
    u16 ambient[3];
    u16 specular[3];
    u16 emissive[3];
} FldActMatColorPreset;

FldActMatColorPreset *FldActMatColorPreset_Create(HeapID heapId);
void FldActMatColorPreset_Free(FldActMatColorPreset *preset);
void FldActMatColorPreset_Load(FldActMatColorPreset *preset, u32 presetId);
void FldActMatColorPreset_Apply(FldActMatColorPreset *preset, BlActScene *scene);

void FldActSys_ClearCache(MMSys *mmSys);
void LoadMModelSystemInfoCache(MMSys *mmSys, s32 index);

// Starts the action command acmd turned to dir
static inline void StartDirAcmd(FieldActor *actor, u32 dir, u32 acmd) {
    u32 result = GetAcmdForDir(dir, acmd);

    func_ov010_0216d438(actor, result);
}

#endif // BWPORT_FIELD_FIELD_ACTOR_H
