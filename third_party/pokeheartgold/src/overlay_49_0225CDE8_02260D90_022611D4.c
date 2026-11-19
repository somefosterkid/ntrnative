#include "global.h"
#include "overlay_49_unk_struct_ov70_02263270.h"

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_02260AD4_t;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

void ov49_0225CC20(UnkStruct_ov70_02260AD4 *param0, fx32 param1, fx32 param2, fx32 param3);

typedef struct {
    u32 unk_00;
    u16 unk_04;
    u16 unk_06;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54 *unk_0C;
    UnkStruct_ov70_02263270 unk_10;
} UnkStruct_ov70_02265104;

void ov49_022611D4(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02260AD4 *param1);

void ov49_022611D4(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02260AD4 *param1) {
    VecFx32 v0;

    ov49_02259154(param0->unk_0C, &v0);
    ov49_0225CC20(param1, v0.x, v0.y, v0.z);
}
