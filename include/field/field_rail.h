// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_rail.h, the lines in use so far.
// pokebw2's func_ov036_021b06ec and func_ov036_021b0704 are Black's func_ov021_021b7d38 and func_ov021_021b7d50.
#ifndef BWPORT_FIELD_FIELD_RAIL_H
#define BWPORT_FIELD_FIELD_RAIL_H

// The rail system of the no-grid maps, where the player moves along rails. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "field/zone.h"

// field_rail.c: the rail system
// Finds where the segment from start to end meets a rail
BOOL CalculateRailCurves(FieldRailSystem *rail, const VecFx32 *start, const VecFx32 *end, RailPosition *railPos,
                         VecFx32 *hit);
// The world position of the rail position
void func_ov021_021b7d38(FieldRailSystem *rail, const RailPosition *railPos, VecFx32 *pos);
s32 func_ov021_021b7d50(FieldRailSystem *rail, const RailPosition *pos);
void RailUnit_GetCalcPos(RailUnit *unit, VecFx32 *pos);

// rail_attr.c: the tile attributes of the rail lines
typedef struct RailAttr RailAttr;

RailAttr *AllocateRailAttrBlock(u32 heapId);

u32 FieldRailTilemap_GetTileAtPos(RailAttr *attr, const RailPosition *pos);
void func_ov021_021bb11c(RailAttr *attr);
// Whether a tile class is one of the rails' special tiles
BOOL func_ov021_021bb188(u16 tileClass);
BOOL func_ov021_021bb1ac(u32 tileClass);
BOOL func_ov021_021bb1b8(u32 tileClass);
BOOL func_ov021_021bb1c4(u32 tileClass);
BOOL func_ov021_021bb1d0(u32 tileClass);
BOOL func_ov021_021bb1dc(u32 tileClass);
BOOL func_ov021_021bb1e8(u32 tileClass);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// field_rail.c: the rail system
FieldRailSystem *FieldRailSystem_Create(HeapID heapId, u32 unitCount, FieldCamera *camera);
// Updates the camera from its parent unit when the unit asks for it, the camera is dirty or force is set
BOOL FieldRailSystem_UpdateCamera(FieldRailSystem *rail, BOOL force);
// Writes the unit's rail position to three script variables, or sets it from three values
void SetWkToActorRailPos(RailUnit *unit, u16 *a1, u16 *a2, u16 *a3);
void FieldRailTilemap_Load(RailAttr *attr, u32 fileId, u32 heapId);
void func_ov021_021bb0a4(RailAttr *attr);
// Starts on the rail data, with every line on
void FieldRailSystem_Load(FieldRailSystem *rail, RailDataHandle *data);
// pokebw2's FieldRailSystem_Free
void func_ov021_021b791c(FieldRailSystem *rail);
// pokebw2's FieldRailSystem_Reset
void func_ov021_021b7954(FieldRailSystem *rail);
// Does nothing: what is left of a check whether the rails are loaded and active
void func_ov021_021b79e4(FieldRailSystem *rail);
RailUnit *FieldRailSystem_AcquireUnit(FieldRailSystem *rail);
void FieldRailSystem_ReleaseUnit(FieldRailSystem *rail, RailUnit *unit);
void FieldRailSystem_SetCameraParent(FieldRailSystem *rail, RailUnit *unit);
void FieldRailSystem_ClearCameraParent(FieldRailSystem *rail);
// Sets pos to line componentId of the rail data, front steps along it and side steps from its edge
void FieldRailSystem_NormalizePos(FieldRailSystem *rail, RailDataHandle *data, u16 componentId, u16 front, u16 side,
                                  RailPosition *pos);
void FieldRailSystem_SetPlayerPos(FieldRailSystem *rail, const RailPosition *pos);
// pokebw2's FieldRailSystem_SetLineEnabled
// Turns a line on or off: the system keeps up to 18 lines that are off, and finds no line where one of them is
void func_ov021_021b7c04(FieldRailSystem *rail, u32 lineId, BOOL enabled);
BOOL func_ov021_021b7cd8(FieldRailSystem *rail, const RailPosition *railPos, u32 railDir, RailPosition *next);
// The length of a step on the rails
fx32 func_ov021_021b7c38(FieldRailSystem *rail);
void func_ov021_021b8484(RailUnit *unit, u16 a1, u16 a2, u16 a3);
void func_ov021_021b7dc0(const RailUnit *unit, RailPosition *pos);
// Whether the unit has a move pending
u8 func_ov021_021b8040(RailUnit *unit);
void RailUnit_SetRailPos(RailUnit *unit, const RailPosition *pos);

typedef struct RailPoint RailPoint;
typedef struct RailLine RailLine;
typedef struct RailCamera RailCamera;
typedef struct RailCurve RailCurve;
typedef struct RailDisabledLines RailDisabledLines;

// A point of the rails, where up to four lines meet: each line's ID (0xffff for none), the direction it leaves in and
// the rail's width there. Layouts from swan's RailPoint, RailLine, RailCurve, RailCamera, RailDataHandle, RailUnit and
// FieldRailSystem, which are Black's (their sizes: the strides of GetRailPoint and the other getters)
struct RailPoint {
    u32 lineIds[4];
    u32 dirs[4];
    s32 widths[4];
    VecFx32 pos;
    u32 cameraId;
    char name[48];
};

// A line between two points, length tiles long
struct RailLine {
    u32 pointA;
    u32 pointB;
    u32 dir;
    u32 curveId;
    u32 cameraId;
    u32 length;
    char name[48];
};

// A camera of the rails: the camera function to call, and its parameters (the target, for some, at 0xc)
struct RailCamera {
    u32 funcId;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
};

// How a line bends: the function pair that converts between rail and world positions, and the center of its arc
struct RailCurve {
    u32 handlerId;
    VecFx32 center;
    u8 unk_10[0x14];
};

typedef void (*RailCameraFunc)(FieldRailSystem *rail);
typedef void (*RailToWorldFunc)(RailUnit *unit, VecFx32 *pos);
typedef BOOL (*WorldToRailFunc)(u32 lineId, FieldRailSystem *rail, const VecFx32 *pos, RailPosition *railPos,
                                VecFx32 *alignedPos);

// The points, lines, cameras and curves of a map's rails, and the function tables that the cameras and curves index
struct RailDataHandle {
    u16 pointCount;
    u16 lineCount;
    u16 cameraCount;
    u16 curveCount;
    u16 camFuncCount;
    u16 railToWorldCount;
    u16 worldToRailCount;
    u16 unk_0E;
    RailPoint *points;
    RailLine *lines;
    RailCamera *cameras;
    RailCurve *curves;
    RailCameraFunc *camFuncs;
    RailToWorldFunc *railToWorldFuncs;
    WorldToRailFunc *worldToRailFuncs;
    fx32 tileWidth;
};

// The lines that are off, 0xffff in a free slot
struct RailDisabledLines {
    u16 lineIds[18];
};

// Something that moves along the rails: the point it is at, or the line, with its position along the line (front)
// and across it (side), in sixteenths of a tile
struct RailUnit {
    u16 onLine;
    u16 acquired;
    union {
        RailPoint *point;
        RailLine *line;
    } component;
    u16 active;
    s32 front;
    s32 length;
    s32 side;
    s32 width;
    fx32 tileWidth;
    u32 dir;
    const RailDataHandle *handle;
    RailDisabledLines *disabled;
    u8 pending;
    s8 counter;
    u16 pendingArgB;
    u16 pendingArgA;
    u8 unk_32;
    u8 unk_33;
    u8 unk_34;
    VecFx32 calcPos;
    VecFx32 prevCalcPos;
    VecFx32 nextVec;
    // Black copies a rail position into the unit as four halfwords (RailUnit_SetRailPos)
    union {
        RailPosition railPos;
        u16 railPosHalves[4];
    };
    union {
        RailPosition prevRailPos;
        u16 prevRailPosHalves[4];
    };
    RailPosition nextPos;
    u8 nextCached;
    u8 nextOk;
    u8 nextDir;
    FieldRailSystem *rail;
};

struct FieldRailSystem {
    u16 heapId;
    u16 loaded;
    u16 cameraEnabled;
    RailUnit *units;
    s32 unitCount;
    FieldCamera *camera;
    RailUnit *cameraParent;
    u32 cameraDirty;
    RailUnit *playerUnit;
    RailDataHandle data;
    RailDisabledLines disabledLines;
};


u32 func_ov021_021b7c90(FieldRailSystem *rail, RailPosition *pos, u32 dir, VecFx32 *out);
u8 func_ov021_021b7e60(RailUnit *unit, u32 dir, RailPosition *outPos, VecFx32 *outVec);
u8 func_ov021_021b8038(RailUnit *unit);
void func_ov021_021b8050(RailUnit *unit, s16 *out);
BOOL func_ov021_021b80e0(RailUnit *unit, u32 param_2, u32 param_3);
void func_ov021_021b810c(RailUnit *unit);
BOOL func_ov021_021b83b0(const VecFx32 *a, const VecFx32 *b, fx32 dist);

#endif // BWPORT_FIELD_FIELD_RAIL_H
