#ifndef POKEHEARTGOLD_OV_01_021F1348_H
#define POKEHEARTGOLD_OV_01_021F1348_H

#include "constants/heap.h"

#include "billboard.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "map_object.h"
#include "script.h"
#include "simple3dmodel.h"
#include "task.h"
#include "unk_020689C8.h"

struct OverworldAnimManagerList;
struct ResourceHeap;

// The field effects' billboard resource allocator (FieldEffectManager.unk_20): the model/metadata heaps and the 3D
// resource manager, plus `unk_04` BillboardResources slots in `unk_20` named by the id table `unk_1C` (id 0xF = free).
struct UnkStruct_ov5_021DF8C8_t {
    u32 unk_00;
    BillboardResources *unk_04;
};

struct UnkStruct_ov5_021DF8FC_t {
    enum HeapID heapID;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    BillboardList *unk_0C;
    struct ResourceHeap *modelResHeap;
    struct ResourceHeap *metadataResHeap;
    GF_3DGfxRawResMan *texResMan;
    struct UnkStruct_ov5_021DF8C8_t *unk_1C;
    BillboardResources *unk_20;
};

typedef struct FieldEffectRenderer {
    u32 id;
    void *context;
} FieldEffectRenderer;

typedef struct FieldEffectManager {
    enum HeapID heapID;
    u32 rendererCount;
    u32 animManCount;
    u32 dummy0C;
    FieldSystem *fieldSystem;
    FieldEffectRenderer *renderers;
    NARC *fldEffNARC;
    struct OverworldAnimManagerList *animManList;
    struct UnkStruct_ov5_021DF8FC_t *unk_20;
} FieldEffectManager; // sizeof == 0x24

#define FIELD_EFFECT_RENDERER_ID_NONE 0x17 // the id of an unused FieldEffectRenderer slot

// A FieldEffectRenderer's context starts with the manager that made it (more members follow in each renderer's own type).
typedef struct FieldEffectRendererContext {
    FieldEffectManager *fieldEffMan;
} FieldEffectRendererContext;

typedef void *(*FieldEffectRenderer_NewFunc)(FieldEffectManager *);
typedef void (*FieldEffectRenderer_FreeFunc)(void *);

// ov01_02208C5C: one per renderer id, ended by an entry with id 0x17.
typedef struct FieldEffectRendererFuncs {
    u32 id;
    FieldEffectRenderer_NewFunc newFunc;
    FieldEffectRenderer_FreeFunc freeFunc;
} FieldEffectRendererFuncs;

// (field_effect_manager?)
FieldEffectManager *FieldEffectManager_New(FieldSystem *fieldSystem, u32 rendererCount, enum HeapID heapID);
void FieldEffectManager_InitAnimManagerList(FieldEffectManager *fieldEffectManager, u32 animManagerCount);
void ov01_021F1390(FieldEffectManager *fieldEffectManager, enum HeapID heapID, int, int, int, int, int, int, int);
void FieldEffectManager_InitRenderers(FieldEffectManager *fieldEffectManager, const u32 *arg1);
void FieldEffectManager_Render(FieldEffectManager *fieldEffectManager);
void FieldEffectManager_Free(FieldEffectManager *fieldEffectManager);
void ov01_021F1610(FieldEffectManager *fieldEffMan);
void ov01_021F13B0(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F14F4(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F151C(FieldEffectManager *fieldEffMan, FieldEffectRenderer *renderer);
FieldEffectRenderer *ov01_021F1560(FieldEffectManager *fieldEffMan);
FieldEffectRenderer *ov01_021F1588(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F15A0(FieldEffectRenderer *renderer);
void ov01_021F15AC(FieldEffectRenderer *renderer, u32 id, void *context);
BOOL ov01_021F15B4(const FieldEffectRenderer *renderer);
const FieldEffectRendererFuncs *ov01_021F15C4(u32 id);
void ov01_021F1804(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, u32 id);
BillboardResources *ov01_021F18A8(struct UnkStruct_ov5_021DF8FC_t *billboardAlloc, u32 id);
void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);
void ov01_021F18FC(FieldEffectManager *fieldEffMan, u32 slotID);
void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);
void *ov01_021F14B4(FieldEffectManager *fieldEffMan, u32 fileId, s32 allocAtEnd);
void ov01_021F1A48(FieldEffectManager *fieldEffMan, NNSG3dRenderObj *renderObj, NNSG3dResMdl **model, NNSG3dResFileHeader **resFile);
void ov01_021F197C(FieldEffectManager *fieldEffMan, u32 id, GF_3DGfxRawResMan *resMan);
void ov01_021F1AB8(FieldEffectManager *fieldEffMan, u32 fileId, s32 allocAtEnd, NNSG3dRenderObj *renderObj, NNSG3dResMdl **model, NNSG3dResFileHeader **resFile);
NNSG3dResTex *ov01_021F1AD4(FieldEffectManager *fieldEffMan, s32 id);

typedef struct UnkOv01_021FFECC {
    void *unk0;
    Simple3DModel unk4[4];
    NNSG3dRenderObj unk54[4];
} UnkOv01_021FFECC;

// userData record of the field effect anim managers made from a map object's position (the callers build it on the
// stack and pass it to ov01_021F1620): 0x24 bytes at 32 bits, four pointers in it.
typedef struct FieldEffectAnimArgs {
    VecFx32 pos;
    FieldSystem *fieldSystem; // ov01_021F1468(manager)
    FieldEffectManager *fieldEffMan; // ov01_021F146C(mapObject)
    UnkOv01_021FFECC *unk_14;
    LocalMapObject *mapObject;
    s16 unk_1C;
    s16 unk_1E;
    s8 direction;
} FieldEffectAnimArgs;

// The context of the ov01_02209138 and ov01_02209218 renderer anim managers (0x40 bytes at 32 bits; word 0 of those
// tables is its size, so the host tables must use sizeof(FieldEffectAnimWork)).
typedef struct FieldEffectAnimWork {
    int state;
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    int unk_10;
    BOOL heightSet; // sub_0206121C result
    FieldEffectAnimArgs args; // userData copied in by ov01_021FF174 (built by ov01_021FF070/0E4, ov01_021FF8F0/964)
    Billboard *billboard;
} FieldEffectAnimWork;

typedef struct {
    int unk0;
    BOOL unk4;
    u32 localMapObjectId;
    u32 mapId;
    UnkOv01_021FFFCC_common unk10;
} UnkOv01_021FFFCD;

typedef UnkOv01_021FFFCC_common UnkOv01_021FFF5C;

void *ov01_021F1430(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd, u32 initValue);
FieldEffectManager *ov01_021F146C(LocalMapObject *mapObject);
FieldSystem *ov01_021F1468(const FieldEffectManager *fieldEffMan);
void *ov01_021F1450(FieldEffectManager *fieldEffMan, u32 id);
OverworldAnimManager *ov01_021F1620(const FieldEffectManager *fieldEffMan, const OverworldAnimManagerFuncs *funcs, const VecFx32 *initialPos, int userInt, const void *userData, int sysTaskPriority);
void ov01_021F1448(void *a0); // UnkOv01_021FFECC or NNSG3dResFileHeader?
void ov01_021F19F4(FieldEffectManager *fieldEffMan, Simple3DModel *model, u32 modelIndex, u32 memberIdx, BOOL allocAtEnd);
void ov01_021F1640(OverworldAnimManager *animMan);

#endif // POKEHEARTGOLD_OV_01_021F1348_H
