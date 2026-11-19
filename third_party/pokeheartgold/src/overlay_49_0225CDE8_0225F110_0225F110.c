#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02263344_t UnkStruct_ov70_02263344;

typedef BOOL (*UnkFuncPtr_ov70_0226DA18)(UnkStruct_ov70_02263344 *, UnkStruct_ov70_0225DEE8 *, u32);

typedef struct {
    UnkFuncPtr_ov70_0226DA18 unk_00;
    UnkFuncPtr_ov70_0226DA18 unk_04;
} UnkStruct_ov70_0226DA18;

typedef struct {
    const UnkStruct_ov70_0226DA18 *unk_00;
    void *unk_04;
    void *unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0226308C;

struct UnkStruct_ov70_02263344_t {
    u16 heapID;
    u8 unk_02;
    u8 unk_03;
    UnkStruct_ov70_0226308C unk_04;
    UnkStruct_ov70_0226308C unk_14;
};

BOOL ov49_0225F170(const UnkStruct_ov70_02263344 *param0);

void ov49_0225F190(UnkStruct_ov70_0226308C *param0, const UnkStruct_ov70_0226DA18 *param1, void *param2, u32 param3, void *param4);

void ov49_0225F110(UnkStruct_ov70_02263344 *param0, const UnkStruct_ov70_0226DA18 *param1, void *param2);

void ov49_0225F110(UnkStruct_ov70_02263344 *param0, const UnkStruct_ov70_0226DA18 *param1, void *param2) {
    ((ov49_0225F170(param0) == 1) ? (void)0 : GF_AssertFail());
    param0->unk_14 = param0->unk_04;

    ov49_0225F190(&param0->unk_04, param1, param2, 0, 0L);
}

void ov49_0225F19C(UnkStruct_ov70_0226308C *param0);

void ov49_0225F148(UnkStruct_ov70_02263344 *param0);

void ov49_0225F148(UnkStruct_ov70_02263344 *param0) {
    GF_ASSERT(param0->unk_04.unk_04 == NULL);
    param0->unk_04 = param0->unk_14;

    ov49_0225F19C(&param0->unk_14);
}
