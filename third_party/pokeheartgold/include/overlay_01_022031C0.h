#ifndef POKEHEARTGOLD_OVERLAY_01_022031C0_H
#define POKEHEARTGOLD_OVERLAY_01_022031C0_H

#include "field/overlay_01_021FB878.h"

#include "field_types_def.h"

#include "heap.h"
#include "overlay_01_021F1348.h"
#include "unk_0206793C.h"

// The context of the field effect renderer made by ov01_022031C0 (sizeof == 0x140 at 32 bits): the manager first,
// two models (files 0x81 and 0x68) each with a render object, the second one animated.
typedef struct UnkOv01_022031C0 {
    FieldEffectManager *fxMgr;
    NNSFndAllocator alloc;
    void *mdlData0;
    void *mdlData1;
    Field3dModel model0;
    Field3dModel model1;
    Field3dObject obj0;
    Field3dObject obj1;
    Field3DModelAnimation anim;
} UnkOv01_022031C0;

// What ov01_0220329C hands the renderer's init function (sub_02068D98 of the manager).
typedef struct UnkOv01_022031C0_Data {
    VecFx32 pos;
    FieldSystem *fieldSystem;
    FieldEffectManager *fieldEffMan;
    UnkOv01_022031C0 *manager;
    LocalMapObject *mapObject;
    s16 unk1C;
    s16 unk1E;
    s8 unk20;
    u8 unk21[3];
} UnkOv01_022031C0_Data;

// The OverworldAnimManager context of the four renderers on that manager (sizeof == 0x54 at 32 bits).
typedef struct UnkOv01_022031C0_Work {
    int state;
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    int counter;
    int unk14;
    UnkOv01_022031C0_Data data;
    fx32 *coordPtr;
    fx32 coordStart;
    int delta;
    VecFx32 followPos;
} UnkOv01_022031C0_Work;

#ifndef SDK_PORT
// DS layout check: the pointer fields are 4 bytes there (the PC build's are wider)
typedef char UnkOv01_022031C0_Work_size_check[(sizeof(UnkOv01_022031C0_Work) == 0x54) ? 1 : -1];
#endif

void ov01_022039BC(u32 direction, VecFx32 *pos, fx32 **coordPtr);

void *ov01_022031C0(FieldEffectManager *fieldEffMan);
void ov01_022031E8(void *a0);
void ov01_022031F8(UnkOv01_022031C0 *ctx);
void ov01_02203270(UnkOv01_022031C0 *ctx);

OverworldAnimManager *ov01_0220329C(LocalMapObject *obj, int a1);

// The init (one per mode), exit, tick and render functions of the four funcs tables ov01_022094C8/DC/F0/ov01_02209504 (context UnkOv01_022031C0_Work).
BOOL ov01_02203820(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
BOOL ov01_0220335C(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
BOOL ov01_022035DC(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_022033E0(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_02203890(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_022033E4(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_02203654(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_022034F8(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_022037E8(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_022034B8(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);
void ov01_022035A4(OverworldAnimManager *animMan, UnkOv01_022031C0_Work *work);

#endif // POKEHEARTGOLD_OVERLAY_01_022031C0_H
