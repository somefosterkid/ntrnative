#include "global.h"

#include "constants/heap.h"
#include "heap.h"
#include "overlay_49_unk_struct_ov70_02263344_t.h"

typedef struct UnkStruct_ov49_0225F074 {
    u8 unk_0[0x8];
    void *unk_8;
    u8 unk_C[0xC];
    void *unk_18;
} UnkStruct_ov49_0225F074;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02263344_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

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

struct UnkStruct_ov70_022630A4_t {
    u32 unk_00;
    UnkStruct_ov70_0225DEE8 *unk_04;
    UnkStruct_ov70_02263344 unk_08[20];
    UnkStruct_ov70_02263344 unk_2D8;
};

void ov49_0225F098(UnkStruct_ov70_02263344 *param0, UnkStruct_ov70_022630A4 *param1, u32 param2, u32 param3);

BOOL ov49_0225F180(const UnkStruct_ov70_02263344 *param0);

void ov49_0225F018(UnkStruct_ov70_022630A4 *param0, u32 param1);

void ov49_0225F068(void *arg0, s8 arg1, s16 arg2);
void ov49_0225F074(UnkStruct_ov49_0225F074 *arg0);

void ov49_0225F018(UnkStruct_ov70_022630A4 *param0, u32 param1) {
    if (ov49_0225F180(&param0->unk_2D8) == 1) {
        ov49_0225F098(&param0->unk_2D8, param0, param1, 0);
        return;
    }

    {
        int v0;

        for (v0 = 0; v0 < 20; v0++) {
            if (ov49_0225F180(&param0->unk_08[v0]) == 1) {
                ov49_0225F098(&param0->unk_08[v0], param0, param1, v0);
            }
        }
    }
}

void ov49_0225F068(void *arg0, s8 arg1, s16 arg2) {
    *(s16 *)arg0 = arg2;
    *(s8 *)((u8 *)arg0 + 2) = 1;
    *(s8 *)((u8 *)arg0 + 3) = arg1;
}

void ov49_0225F074(UnkStruct_ov49_0225F074 *arg0) {
    void *ptr;
    void *v0;

    ptr = arg0->unk_8;
    if (ptr != NULL) {
        Heap_Free(ptr);
    }
    v0 = arg0->unk_18;
    if (v0 != NULL) {
        Heap_Free(v0);
    }
    memset(arg0, 0, 0x24U);
}
