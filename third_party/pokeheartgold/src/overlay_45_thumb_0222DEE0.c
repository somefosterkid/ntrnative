#include "global.h"

#include "error_handling.h"
#include "pm_string.h"

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

u32 ov45_0222DEE0(const UnkStruct_ov66_02231D38 *param0, u32 param1);

s32 ov45_0222DF14(u8 *arg0, u32 arg1);
u16 ov45_0222DF38(s32 arg0, u32 arg1);
u16 ov45_0222DF50(void *arg0);
s32 ov45_0222DF58(void *arg0, u32 arg1);
void ov45_0222DF78(void *arg0, enum HeapID arg1);
void ov45_0222DFD0(void *arg0);

u32 ov45_0222DEE0(const UnkStruct_ov66_02231D38 *param0, u32 param1) {
    GF_ASSERT(param1 < 20);
    return param0->unk_00[param1].unk_00;
}

typedef enum {
    UnkEnum_ov66_02231E54_00 = 0,
    UnkEnum_ov66_02231E54_01 = 1 << 0,
    UnkEnum_ov66_02231E54_02 = 1 << 1,
} UnkEnum_ov66_02231E54;

BOOL ov45_0222DEF4(const UnkStruct_ov66_02231D38 *param0, u32 param1);

BOOL ov45_0222DEF4(const UnkStruct_ov66_02231D38 *param0, u32 param1) {
    GF_ASSERT(param1 < 20);

    if (param0->unk_00[param1].unk_02 & UnkEnum_ov66_02231E54_01) {
        return 1;
    }

    return 0;
}

s32 ov45_0222DF14(u8 *arg0, u32 arg1) {
    if (arg1 >= 0x14U) {
        GF_AssertFail();
    }
    if (2 & *(u16 *)((u8 *)(arg0 + (arg1 * 4)) + 2)) {
        return 1;
    }
    return 0;
}

u16 ov45_0222DF38(s32 arg0, u32 arg1) {
    if (arg1 >= 0x14U) {
        GF_AssertFail();
    }
    return *(u16 *)((u8 *)(arg0 + (arg1 * 4)) + 2);
}

u16 ov45_0222DF50(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x50);
}

s32 ov45_0222DF58(void *arg0, u32 arg1) {
    if (arg1 >= 0x14U) {
        GF_AssertFail();
    }
    if (*(u16 *)((u8 *)arg0 + 0x52) == arg1) {
        return 1;
    }
    return 0;
}

void ov45_0222DF78(void *arg0, enum HeapID heapID) {
    s32 v2;
    void *v1;
    void *v0;
    s32 v3;
    s32 v5;
    void *v4;
    void *v6;

    v0 = arg0;
    memset(arg0, 0, 0x1D4U);
    v2 = 0;
    v1 = arg0;
    do {
        v6 = v1;
        v5 = 0;
loop_2:
        *(String **)((u8 *)v6 + 0x10) = String_New(8U, heapID);
        v5 += 1;
        v6 += 4;
        if (v5 < 4) {
            goto loop_2;
        }
        v1 += 0x34;
        v3 = v2 + 1;
        v2 = v3;
    } while (v3 < 8);
    v4 = v0 + 0x1A0;
    *(void **)((u8 *)v0 + 0x1CC) = v4;
    *(void **)((u8 *)v0 + 0x1D0) = v4;
}

void ov45_0222DFD0(void *arg0) {
    void *v0;
    s32 v1;
    s32 v4;
    void *v2;
    void *v3;

    v0 = arg0;
    v4 = 0;
    v3 = arg0;
    do {
        v1 = 0;
        v2 = v3;
loop_2:
        String_Delete(*(String **)((u8 *)v2 + 0x10));
        v1 += 1;
        v2 += 4;
        if (v1 < 4) {
            goto loop_2;
        }
        v4 += 1;
        v3 += 0x34;
    } while (v4 < 8);
    memset(v0, 0, 0x1D4U);
}
