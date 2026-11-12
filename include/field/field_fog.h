// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_fog.h, the lines in use so far.
#ifndef BWPORT_FIELD_FIELD_FOG_H
#define BWPORT_FIELD_FIELD_FOG_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The field's fog, which takes effect when flushed
// pokebw2's FieldFog_Create: the fog, off, with the default table
FieldFog *func_ov021_021a0104(HeapID heapId);
u32 FieldFog_GetDepthShift(FieldFog *fog);
void FieldFog_Animate(FieldFog *fog, u32 color, u32 depthShift, u32 duration);

// The fog's animation toward an offset and depth shift (pokebw2's)
typedef struct FieldFogAnimation {
    u16 frame;
    u16 frameCount;
    s32 offsetStart;
    s32 offsetDelta;
    s32 depthShiftStart;
    s32 depthShiftDelta;
} FieldFogAnimation;

// pokebw2's layout, which is Black's (0x44, its allocation in field_fog.c)
struct FieldFog {
    u8 enabled;
    u8 alphaMode;
    u8 depthShift;
    u8 alpha;
    s32 offset;
    u16 color; // GXRgb
    union {
        u32 words[8];
        u8 bytes[32];
    } table;
    FieldFogAnimation animation;
    BOOL dirty;
};

// The zone's fog data controller (pokebw2's field_zonefog.c; Black's 0x10, its allocation): loads the zone's fog
// file a few frames after a request
typedef struct FieldFogCtrl {
    u8 loadRequested;
    s8 loadDelay;
    u16 fileId;
    u32 *fogData;
    u32 staticLightIndex;
    ArcTool *arc;
} FieldFogCtrl;

// field_zonefog.c: the controller, no light yet, its fog archive open
FieldFogCtrl *func_ov021_021a02c4(HeapID heapId);
void func_ov021_021a02f4(FieldFogCtrl *ctrl);
void func_ov021_021a030c(FieldFogCtrl *ctrl, HeapID heapId);
void func_ov021_021a0338(FieldFogCtrl *ctrl, s32 fileId, u32 staticLightIndex, u8 delay);
u8 func_ov021_021a0354(FieldFogCtrl *ctrl);
void FieldFogCtrl_Reset(FieldFogCtrl *ctrl);
u32 func_ov021_021a0390(FieldFogCtrl *ctrl);
u32 func_ov021_021a03a4(FieldFogCtrl *ctrl);
u32 func_ov021_021a03b8(FieldFogCtrl *ctrl);
u32 func_ov021_021a03cc(FieldFogCtrl *ctrl);

#endif // BWPORT_FIELD_FIELD_FOG_H
