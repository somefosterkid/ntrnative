#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_49_unk_struct_ov70_02261_e10_t.h"
#include "overlay_49_unk_struct_ov70_022629_fc.h"

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_02260E8C_t;

struct UnkStruct_ov70_022610B8_t;

struct UnkStruct_ov70_02261E10_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;

void ov49_0225CFA8(UnkStruct_ov70_02260E8C *param0, const VecFx32 *param1);

typedef struct {
    UnkStruct_ov70_022610B8 *unk_00[3];
    UnkStruct_ov70_022610B8 *unk_0C[3];
    BOOL unk_18;
    UnkStruct_ov70_022629FC unk_1C[24];
    UnkStruct_ov70_022629FC unk_13C[24];
    UnkStruct_ov70_022610B8 *unk_25C[24];
    UnkStruct_ov70_022629FC unk_2BC[24];
    u8 unk_3DC;
    u8 unk_3DD;
    u8 unk_3DE;
    u8 unk_3DF;
    u32 unk_3E0;
} UnkStruct_ov70_02261E10_sub1;

void ov49_0225E3B8(UnkStruct_ov70_02261E10 *param0, u32 param1, fx32 param2);

s32 ov49_0225CFEC(UnkStruct_ov70_02260E8C *, const VecFx32 *);
void ov49_0225E3F4(void *arg0, u32 arg1, void *arg2);

void ov49_0225E3B8(UnkStruct_ov70_02261E10 *param0, u32 param1, fx32 param2) {
    VecFx32 v0;

    GF_ASSERT(param0->unk_613 > param1);

    v0.x = param2;
    v0.z = (FX32_CONST(40));
    v0.y = (FX32_CONST(0));

    ov49_0225CFA8(param0->unk_208[param1], &v0);
}

void ov49_0225E3F4(void *arg0, u32 arg1, void *arg2) {
    if ((u32) * (u8 *)((u8 *)arg0 + 0x613) <= arg1) {
        GF_AssertFail();
    }
    ov49_0225CFEC(*(void **)((u8 *)(arg0 + (arg1 * 4)) + 0x208), arg2);
}
