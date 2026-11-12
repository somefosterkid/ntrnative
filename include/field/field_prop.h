// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_prop.h, the lines in use so far.
#ifndef BWPORT_FIELD_FIELD_PROP_H
#define BWPORT_FIELD_FIELD_PROP_H

#include "types.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

struct FieldPropAreaBounds {
    fx32 minZ;
    fx32 maxZ;
    fx32 minX;
    fx32 maxX;
};

// Black 2's, twinned by pokebw2; the layout of a prop's entry in a chunk file, 0x10 bytes
struct FieldPropSourceInfo {
    s32 x;
    s32 y;
    s32 z;
    u16 unkC;
    u8 highResId;
    u8 lowResId;
};

s32 FieldPropSystem_InstantiateProps(FieldPropSystem *system, FieldChunk *chunk, const FieldPropSourceInfo *infos,
                                     s32 count);
void FieldPropHandle_CallAnmCmd(FieldPropHandle *handle, u32 animation, u32 command);
BOOL FieldPropHandle_IsAnmFinished(FieldPropHandle *handle);
void FieldPropHandle_Free(FieldPropHandle *handle);
FieldPropHandle *FieldPropSystem_CreateHandleNew(FieldPropSystem *system, u32 propId, SRTMatrix *transform);
FieldPropHandle *FieldPropSystem_CreateHandleFromExisting(FieldPropSystem *system, FieldChunkPropHolder *prop);
FieldChunkPropHolder **FieldPropSystem_FindPropsInArea(FieldPropSystem *system, const FieldPropAreaBounds *bounds,
                                                       u32 filter, u32 *count);
void FieldChunkPropHolder_GetPosAbs(FieldChunkPropHolder *prop, VecFx32 *position);

void *FieldPropResBundle_GetResInfo(FieldPropResBundle *bundle, u32 index);
void *FieldPropSystem_FindResInfo(FieldPropSystem *system, u32 resId);
void FieldPropResInstance_CallAnmCmd(FieldPropResInstance *instance, u32 animation, u32 command);
BOOL FieldPropResInstance_IsAnmIdle(FieldPropResInstance *instance, u32 animation);
void FieldPropResInstance_Free(FieldPropResInstance *instance);
BOOL FieldPropSystem_CheckCreateDoorReq(FieldPropSystem *system, u32 resId, VecFx32 *position, u32 *resIndex);
void FieldPropSystem_InstantiateFromInfo(FieldPropSystem *system, FieldChunk *chunk, const FieldPropSourceInfo *info,
                                         u32 propIndex);
void FieldChunkPropHolder_Release(FieldPropSystem *system, FieldChunkPropHolder *holder);
void FieldChunkPropHolder_SetVisible(FieldChunkPropHolder *holder, BOOL visible);
void FieldChunkPropHolder_ChangeResID(FieldPropSystem *system, FieldChunkPropHolder *holder, u32 resId);
void FieldPropResInstance_AnmStopAll(FieldPropResInstance *instance);
void FieldPropResInstance_AnmSetPlay(FieldPropResInstance *instance, u32 animation);
void FieldPropResInstance_AnmSetPlayLoop(FieldPropResInstance *instance, u32 animation);
void FieldPropResInstance_AnmSetPlayInv(FieldPropResInstance *instance, u32 animation);
void FieldPropResInstance_AnmSetPause(FieldPropResInstance *instance, u32 animation);
void FieldPropRTCState_Update(FieldPropRTCState *state);
u8 FieldPropRTCState_GetPlayAnmIndex(FieldPropRTCState *state);
void FieldPropHandle_CallAnmCmdSilent(FieldPropHandle *handle, u32 command);
BOOL FieldPropHandle_IsAnmIdle(FieldPropHandle *handle, u32 animation);
BOOL FieldPropHandle_IsCurrentAnmIdle(FieldPropHandle *handle);
u16 FieldPropHandle_GetPropType(FieldPropHandle *handle);
BOOL FieldPropHandle_GetAnimSoundIDCore(FieldPropHandle *handle, u32 animation, u16 *soundId);
BOOL FieldPropHandle_GetAnimSoundID(FieldPropHandle *handle, u16 *soundId);
BOOL FieldPropHandle_IsAnimSoundFinished(FieldPropHandle *handle);
FieldPropHandle *FieldPropSystem_CreateHandleAtPos(FieldPropSystem *system, u32 propId, const VecFx32 *position);
FieldChunkPropHolder *FieldPropSystem_FindProp(FieldPropSystem *system, u32 propId, const FieldPropAreaBounds *bounds);
FieldChunkPropHolder *FieldPropSystem_FindPropAtPos(FieldPropSystem *system, u32 propId, const VecFx32 *position);
void FieldPropSearchArea_Set(FieldPropAreaBounds *bounds, const VecFx32 *position);

// The ways a prop's animations play, the index of its controller in field_buildmodel.c's FIELD_PROP_ANMCNT_VTABLES
// (pokebw2's)
typedef enum {
    FIELD_PROP_ANMCNT_STATIC,
    FIELD_PROP_ANMCNT_AMBIENT,
    FIELD_PROP_ANMCNT_DYNAMIC,
    FIELD_PROP_ANMCNT_RTC, // by the time of day
    FIELD_PROP_ANMCNT_COUNT
} FieldPropAnmControllerType;

// Black 2's; twinned code confirms the offsets of controllerType, ambientAnimationCount and animationIds in Black
struct FieldPropResAnmHeader {
    u8 controllerType;
    u8 unk1;
    u8 ambientAnimationCount;
    u8 unk3;
    u32 animationIds[4];
};

// Black 2's, not checked against Black
// How a prop's animations play, by its animation header's controllerType: static, ambient, dynamic or by the time of
// day
struct FieldPropAnmController {
    void (*init)(FieldPropSystem *system, FieldPropResInstance *instance);
    void (*update)(FieldPropSystem *system, FieldPropResInstance *instance);
    void (*command)(FieldPropResInstance *instance, u32 animation, u32 command);
};

// Black 2's; twinned code confirms the offsets of countFlags and offsets in Black
struct FieldPropResBundle {
    u8 unk0[2];
    u8 countFlags;
    u8 unk3;
    u32 offsets[1];
};

// Black 2's; twinned code confirms the offsets of type in Black
// Field names from swan's field_static_prop.h.
struct FieldPropResInfo {
    u16 resId;
    u16 type;
    u16 doorResId;
    s16 doorX;
    s16 doorY;
    s16 doorZ;
    u16 unk0c;
    u16 unk0e;
    FieldPropResAnmHeader animationHeader;
};

// A prop's loaded resources, 0x18 bytes. Layout from swan.
struct FieldPropResource {
    FieldPropResInfo *info;
    void *model;
    void *animations[4];
};

// Black 2's; twinned code confirms the offsets of position and rotationY in Black
// A prop placed in a chunk's data. Layout from swan's field_static_prop.h.
struct FieldPropPosition {
    VecFx32 position;
    u16 rotationY;
    u16 pad0e;
};

struct FieldPropInstance {
    u32 resIndex;
    FieldPropPosition pos;
};

struct FieldChunkPropHolder {
    FieldChunk *chunk;
    u32 savedResIndex;
    u16 propIndex;
    u16 visible;
    FieldPropInstance *instance;
};

// Black 2's; twinned code confirms the offsets of dayPeriod, previousDayPeriod, dayPartChanged, playAnmIndex and season
// in Black
struct FieldPropRTCState {
    u32 dayPeriod;
    u32 previousDayPeriod;
    BOOL dayPartChanged;
    u8 playAnmIndex;
    u8 season;
    u8 padding[2];
};

// Black 2's; twinned code confirms the offsets of actor in Black
struct FieldPropResInstanceHandle {
    G3DActor *actor;
    u32 unk4;
};

// Black 2's; twinned code confirms the offsets of handles and count in Black
// The props' actors, which the mapper reads. Swan has the first two fields; the rest is cleared and freed with them
struct FieldPropResBank {
    FieldPropResInstanceHandle *handles;
    u32 count;
    void *unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
};

// Black 2's; twinned code confirms the offsets of rtcState and resBundle in Black
// Layout from swan
struct FieldPropSystem {
    u16 heapId;
    FieldG3DMapper *mapper;
    FieldPropRTCState rtcState;
    FieldPropResBundle *resBundle;
    u8 resIdToIndex[0x190];
    u16 resInfoCount;
    FieldPropResBank resBank;
    u32 resourceCount;
    FieldPropResource *resources;
    void *textureResource;
    u32 resInstanceCount;
    FieldPropResInstance *resInstances;
    FieldPropHandle *handles[7];
    FieldChunkPropHolder chunkPropHolders[0x120];
};

struct FieldPropResInstance {
    G3DActor *actor;
    FieldPropResource *resource;
    u32 animationState[4];
};

struct FieldPropHandle {
    FieldPropSystem *system;
    u32 animation;
    FieldChunkPropHolder *holder;
    FieldPropResInstance instance;
    SRTMatrix transform;
};

// Black 2's, not checked against Black
// The sounds of a prop type's animations
struct FieldPropDoorSounds {
    u16 propType;
    u16 soundIds[4];
};

u32 FieldPropResAnmHeader_GetAnmCount(const FieldPropResAnmHeader *header);
u8 FieldPropResInfo_GetTypeConv(const FieldPropResInfo *resInfo);
void *FieldPropResBundle_GetModelData(FieldPropResBundle *bundle, u32 index);
void FieldPropResource_Free(FieldPropResource *resource);
void FieldPropSystem_UpdateResInstance(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropSystem_InstantiateProp(FieldChunk *chunk, FieldPropInstance *instance, u32 propIndex);
FieldPropInstance *FieldChunk_GetPropInstance(FieldChunk *chunk, u32 propIndex);
void FieldPropSystem_ReleaseChunkPropHolder(FieldPropSystem *system, FieldChunkPropHolder *holder);
u8 FieldChunkPropHolder_GetResIndex(FieldChunkPropHolder *holder);
void FieldPropRTCState_Init(FieldPropRTCState *state, u8 season);
void FieldPropAnmController_Static_Init(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropAnmController_Ambient_Init(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropAnmController_RTC_Init(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropAnmController_Ambient_Update(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropAnmController_RTC_Update(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropAnmController_Dynamic_Update(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropAnmController_Dynamic_ExecCommand(FieldPropResInstance *instance, u32 animation, u32 command);
void FieldPropHandle_Draw(FieldPropHandle *handle);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
FieldPropResAnmHeader *FieldPropResInfo_GetAnmHeader(FieldPropResInfo *resInfo);
u32 FieldPropSystem_ConvResIDToIndex(const FieldPropSystem *system, u32 resId);
void FieldPropSystem_DeleteHandle(FieldPropSystem *system, FieldPropHandle *handle);
u16 FieldPropSystem_GetHandleID(FieldPropSystem *system, FieldPropHandle *handle);
FieldPropHandle *FieldPropSystem_FindHandleByID(FieldPropSystem *system, u32 id);
FieldChunkPropHolder *FieldPropSystem_LinkPropToChunk(FieldPropSystem *system, FieldChunk *chunk, FieldPropInstance *instance, u32 propIndex);
BOOL FieldPropRTCState_HasDayPartChanged(FieldPropRTCState *state);
void FieldChunkPropHolder_CallAnmCmd(FieldPropSystem *system, FieldChunkPropHolder *prop, u32 animation, u32 command);
void *FieldPropSystem_GetResInfo(FieldPropSystem *system, u32 index);
FieldPropResBank *FieldPropSystem_GetResBank(FieldPropSystem *system);
FieldPropSystem *FieldPropSystem_Create(HeapID heapId, FieldG3DMapper *mapper, u16 season);
// Loads the props of the area, and recolors their textures with the field's color post-FX
void FieldPropSystem_LoadArea(FieldPropSystem *system, u16 zoneId, AreaData *area, void *postFx);
void FieldPropSystem_InitResources(FieldPropSystem *system, void *postFx);
void FieldPropSystem_InstantiateResources(FieldPropSystem *system, FieldPropResBank *bank);
void FieldPropSystem_InitResource(FieldPropSystem *system, FieldPropResource *resource, u32 index, void *postFx);
void FieldPropSystem_LoadResBundle(FieldPropSystem *system, u32 arcId, u32 fileId);
void FieldPropSystem_FreeResBundle(FieldPropSystem *system);
void FieldPropSystem_BuildResIDLUT(FieldPropSystem *system, u32 defaultResId);
void FieldPropResInstance_Init(FieldPropSystem *system, FieldPropResInstance *instance, FieldPropResource *resource);
void FieldPropSystem_RegistHandle(FieldPropSystem *system, FieldPropHandle *handle);
void FieldPropSystem_Update(FieldPropSystem *system);
void FieldPropSystem_DrawAllHandles(FieldPropSystem *system);
void FieldPropSystem_UnlinkChunk(FieldPropSystem *system, FieldChunk *chunk);
void FieldPropSystem_FreeResInstances(FieldPropSystem *system, FieldPropResBank *bank);
void FieldPropSystem_FreeResources(FieldPropSystem *system);
void FieldPropSystem_Free(FieldPropSystem *system);
void FieldPropSystem_ReleaseChunkPropHolders(FieldPropSystem *system, FieldChunk *chunk);
u8 FieldChunkPropHolder_GetPropType(FieldPropSystem *system, FieldChunkPropHolder *holder);
void FieldPropAnmController_Static_Update(FieldPropSystem *system, FieldPropResInstance *instance);
void FieldPropAnmController_Static_ExecCommand(FieldPropResInstance *instance, u32 animation, u32 command);

#endif // BWPORT_FIELD_FIELD_PROP_H
