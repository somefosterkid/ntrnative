// Black's code at ov021 0x0219ef14-0x0219ef44: the reset of a rail data handle, which gives it the tables of the curves'
// conversions between rail and world positions and of the rails' camera functions. The name is pokebw2's descriptive
// one (src/ov036/rail_data_handle.c): the tables are the file's own, as field_rail_loader.c's size sort lays out its
// scene node tables in Black's order only without them.

#include "types.h"
#include "field/field_rail_loader.h"
#include "field/field_rail.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "struct_decls.h"

void func_ov021_021b9b7c(RailUnit *unit, VecFx32 *out);
void func_ov021_021b9c28(RailUnit *unit, VecFx32 *out);
void func_ov021_021b9d60(RailUnit *unit, VecFx32 *out);
BOOL func_ov021_021b9e84(u32 lineId, FieldRailSystem *rail, const VecFx32 *pos, RailPosition *outPos, VecFx32 *outWorld);
BOOL func_ov021_021b9fc8(u32 lineId, FieldRailSystem *rail, const VecFx32 *pos, RailPosition *outPos, VecFx32 *outWorld);
void func_ov021_021ba25c(FieldRailSystem *rail);
void func_ov021_021ba2b4(FieldRailSystem *rail);
void func_ov021_021ba348(FieldRailSystem *rail);
void func_ov021_021ba4fc(FieldRailSystem *rail);
void func_ov021_021ba550(FieldRailSystem *rail);
void func_ov021_021ba6e8(FieldRailSystem *rail);
void func_ov021_021ba7b0(FieldRailSystem *rail);
void func_ov021_021ba8e0(FieldRailSystem *rail);
void func_ov021_021ba9b4(FieldRailSystem *rail);
void func_ov021_021baa30(FieldRailSystem *rail);
void func_ov021_021baa9c(FieldRailSystem *rail);
void func_ov021_021bab30(FieldRailSystem *rail);
void func_ov021_021babb4(FieldRailSystem *rail);
void func_ov021_021babdc(FieldRailSystem *rail);
void func_ov021_021ba494(FieldRailSystem *rail);
void func_ov021_021ba310(FieldRailSystem *rail);
void func_ov021_021ba4c4(FieldRailSystem *rail);
void func_ov021_021bac04(FieldRailSystem *rail);
void func_ov021_021baca8(FieldRailSystem *rail);
void func_ov021_021bac74(FieldRailSystem *rail);

WorldToRailFunc RAIL_CONV_WORLD_TO_RAIL_FUNCS[3] = {
    func_ov021_021b9e84,
    func_ov021_021b9fc8,
    func_ov021_021b9fc8,
};

RailToWorldFunc RAIL_CONV_RAIL_TO_WORLD_FUNCS[3] = {
    func_ov021_021b9b7c,
    func_ov021_021b9c28,
    func_ov021_021b9d60,
};

RailCameraFunc RAIL_CAMERA_CURVE_FUNC_SET[20] = {
    func_ov021_021ba25c,
    func_ov021_021ba2b4,
    func_ov021_021ba348,
    func_ov021_021ba4fc,
    func_ov021_021ba550,
    func_ov021_021ba6e8,
    func_ov021_021ba7b0,
    func_ov021_021ba8e0,
    func_ov021_021ba9b4,
    func_ov021_021baa30,
    func_ov021_021baa9c,
    func_ov021_021bab30,
    func_ov021_021babb4,
    func_ov021_021babdc,
    func_ov021_021ba494,
    func_ov021_021ba310,
    func_ov021_021ba4c4,
    func_ov021_021bac04,
    func_ov021_021baca8,
    func_ov021_021bac74,
};

void RailDataHandle_Reset(RailDataHandle *handle) {
    MI_CpuFill8(handle, 0, sizeof(RailDataHandle));
    handle->camFuncCount = NELEMS(RAIL_CAMERA_CURVE_FUNC_SET);
    handle->railToWorldCount = NELEMS(RAIL_CONV_RAIL_TO_WORLD_FUNCS);
    handle->worldToRailCount = NELEMS(RAIL_CONV_WORLD_TO_RAIL_FUNCS);
    handle->camFuncs = RAIL_CAMERA_CURVE_FUNC_SET;
    handle->railToWorldFuncs = RAIL_CONV_RAIL_TO_WORLD_FUNCS;
    handle->worldToRailFuncs = RAIL_CONV_WORLD_TO_RAIL_FUNCS;
}
