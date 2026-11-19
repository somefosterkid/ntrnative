#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_42.h"
#include "overlay_49.h"
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

void ov49_0225D030(const UnkStruct_ov70_02260E8C *param0, VecFx32 *param1);

extern const UnkStruct_ov44_02232914 ov49_02269A7C[3];

void ov49_0225E420(const UnkStruct_ov70_02261E10 *param0, u32 param1, u32 param2, VecFx32 *param3);

s32 ov49_0225D04C(UnkStruct_ov70_02260E8C *);
s32 ov49_0225D064(UnkStruct_ov70_02260E8C *);
s32 ov49_0225D040(UnkStruct_ov70_02260E8C *, s32);
void ov49_0225E4CC(UnkStruct_ov70_02261E10 *arg0, u32 arg1, s32 arg2);
s32 ov49_0225D07C(UnkStruct_ov70_02260E8C *, s32);
void ov49_0225E4F8(UnkStruct_ov70_02261E10 *arg0, u32 arg1, s32 arg2);
s32 ov49_0225D088(UnkStruct_ov70_02260E8C *);
void ov49_0225E524(UnkStruct_ov70_02261E10 *arg0, u32 arg1);
s32 ov49_0225D090(UnkStruct_ov70_02260E8C *);
void ov49_0225E54C(UnkStruct_ov70_02261E10 *arg0, u32 arg1);
s32 ov49_0225CED0(struct UnkStruct_ov70_02260BB8_t *);
void ov49_0225E574(UnkStruct_ov70_02261E10 *arg0);
s32 ov49_0225CEFC(struct UnkStruct_ov70_02260BB8_t *);
void ov49_0225E580(UnkStruct_ov70_02261E10 *arg0);

void ov49_0225E420(const UnkStruct_ov70_02261E10 *param0, u32 param1, u32 param2, VecFx32 *param3) {
    GF_ASSERT(param0->unk_613 > param1);
    GF_ASSERT(param2 < 3);

    ov49_0225D030(param0->unk_208[param1], param3);

    param3->x += (ov49_02269A7C[param2].unk0 * (FX32_ONE));
    param3->z += (ov49_02269A7C[param2].unk2 * (FX32_ONE));
    param3->y = (FX32_CONST(40));
}

void ov49_0225E47C(UnkStruct_ov70_02261E10 *arg0, u32 arg1) {
    if ((u32)arg0->unk_613 <= arg1) {
        GF_AssertFail();
    }
    ov49_0225D04C(arg0->unk_208[arg1]);
}

void ov49_0225E4A4(UnkStruct_ov70_02261E10 *arg0, u32 arg1) {
    if ((u32)arg0->unk_613 <= arg1) {
        GF_AssertFail();
    }
    ov49_0225D064(arg0->unk_208[arg1]);
}

void ov49_0225E4CC(UnkStruct_ov70_02261E10 *arg0, u32 arg1, s32 arg2) {
    if ((u32)arg0->unk_613 <= arg1) {
        GF_AssertFail();
    }
    ov49_0225D040(arg0->unk_208[arg1], arg2);
}

void ov49_0225E4F8(UnkStruct_ov70_02261E10 *arg0, u32 arg1, s32 arg2) {
    if ((u32)arg0->unk_613 <= arg1) {
        GF_AssertFail();
    }
    ov49_0225D07C(arg0->unk_208[arg1], arg2);
}

void ov49_0225E524(UnkStruct_ov70_02261E10 *arg0, u32 arg1) {
    if ((u32)arg0->unk_613 <= arg1) {
        GF_AssertFail();
    }
    ov49_0225D088(arg0->unk_208[arg1]);
}

void ov49_0225E54C(UnkStruct_ov70_02261E10 *arg0, u32 arg1) {
    if ((u32)arg0->unk_613 <= arg1) {
        GF_AssertFail();
    }
    ov49_0225D090(arg0->unk_208[arg1]);
}

void ov49_0225E574(UnkStruct_ov70_02261E10 *arg0) {
    ov49_0225CED0(arg0->unk_04);
}

void ov49_0225E580(UnkStruct_ov70_02261E10 *arg0) {
    ov49_0225CEFC(arg0->unk_04);
}
