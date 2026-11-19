#include "global.h"
struct UnkStruct_ov70_02261E10_t;

#include "constants/heap.h"

#include "overlay_42.h"
#include "unk_02005D10.h"
#include "overlay_49_unk_struct_ov70_02261_e10_t.h"

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

UnkStruct_ov44_02232914 ov49_0225D1EC(const UnkStruct_ov70_022610B8 *param0);

BOOL ov49_0225E9D0(const UnkStruct_ov70_022610B8 *param0, u8 param1, u8 param2);

void ov49_0225EAB4(struct UnkStruct_ov70_02261E10_t *arg0, void *arg1);

BOOL ov49_0225E9D0(const UnkStruct_ov70_022610B8 *param0, u8 param1, u8 param2) {
    UnkStruct_ov44_02232914 v0;
    UnkStruct_ov44_02232914 v1;

    v1.unk0 = param1 * 16;
    v1.unk2 = param2 * 16;

    v0 = ov49_0225D1EC(param0);

    if ((v0.unk0 == v1.unk0) && (v0.unk2 == v1.unk2)) {
        return 1;
    }

    return 0;
}

u16 ov49_0225D1C0(const UnkStruct_ov70_022610B8 *param0);

void ov49_0225D214(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, u8 param2, int param3);

void ov49_0225EA10(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225EA10(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1) {
    u32 v0 = ov49_0225D1C0(param1);
    (((v0 == 11) || (v0 == 12)) ? (void)0 : GF_AssertFail());

    if ((v0 == 11) || (v0 == 12)) {
        ov49_0225D214(param0, param1, 1, 0);
    }
}

void ov49_0225EA40(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225EA40(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1) {
    u32 v0 = ov49_0225D1C0(param1);
    (((v0 == 11) || (v0 == 12)) ? (void)0 : GF_AssertFail());

    if ((v0 == 11) || (v0 == 12)) {
        ov49_0225D214(param0, param1, 0, 2);
    }
}

u8 ov49_0225D3BC(u8 *, void *, s32);

s32 ov49_0225D470(void *, s32);

s32 ov49_0225EAE0(void *, void *);

void ov49_0225EA70(struct UnkStruct_ov70_02261E10_t *arg0, void *arg1);

void ov49_0225EA70(UnkStruct_ov70_02261E10_t *arg0, void *arg1) {
    s32 v2;

    v2 = 0;
    if (ov49_0225D3BC(*((void **)&arg0->unk_04), arg1, 0) == 0) {
        v2 = 1;
    } else if (ov49_0225D470(arg1, 0) > 0x2000) {
        v2 = 1;
    }
    if (v2 == 1) {
        ov49_0225EAE0(arg0, arg1);
        PlaySE(0x5BCU);
    }
}

void ov49_0225EAB4(UnkStruct_ov70_02261E10_t *arg0, void *arg1) {
    s32 v0;

    v0 = 0;
    if (ov49_0225D3BC(*((void **)&arg0->unk_04), arg1, 0) == 0) {
        v0 = 1;
    }
    if (v0 == 1) {
        ov49_0225EAE0(arg0, arg1);
        PlaySE(0x5C0U);
    }
}
