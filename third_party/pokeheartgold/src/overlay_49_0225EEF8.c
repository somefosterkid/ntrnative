#include "global.h"

#include "heap.h"
#include "error_handling.h"
#include "overlay_49.h"

typedef struct UnkStruct_ov49_0225EF90 {
    u8 unk_0[0x10];
    s32 unk_10;
} UnkStruct_ov49_0225EF90;

typedef struct UnkStruct_ov49_0225EF8C {
    u8 unk_0[0x10];
    s32 unk_10;
} UnkStruct_ov49_0225EF8C;

typedef struct UnkStruct_ov49_0225EF88 {
    u8 unk_0[0x10];
    s32 unk_10;
} UnkStruct_ov49_0225EF88;

typedef struct UnkStruct_ov49_0225EF3C {
    u8 unk_0[0xC];
    s32 unk_C;
} UnkStruct_ov49_0225EF3C;

void ov49_0225F074(void *);

void ov49_0225EEF8(void *param_1);

s32 ov49_0225F018(void *, s32);
void ov49_0225EF24(void *arg0);
void ov49_0225EF30(void *arg0);
s32 ov49_0225EF3C(UnkStruct_ov49_0225EF3C *arg0);
void *ov49_0225EF40(void *arg0, u32 arg1);
void ov49_0225EF68(void *arg0);
void *ov49_0225EF84(void *arg0);
s32 ov49_0225EF88(UnkStruct_ov49_0225EF88 *arg0);
void ov49_0225EF8C(UnkStruct_ov49_0225EF8C *arg0, s32 arg1);
void ov49_0225EF90(UnkStruct_ov49_0225EF90 *arg0);
s32 ov49_0225F0D8(s32, void *, s32);
void ov49_0225EF98(s32 arg0, u32 arg1, void *arg2, s32 arg3);
s32 ov49_0225F110(s32, void *, void *);
void ov49_0225EFC4(s32 arg0, u32 arg1, void *arg2, void *arg3);
s32 ov49_0225F10C(void *, s32);

void ov49_0225EEF8(void *param_1) {
    s32 iVar1;
    u8 *iVar2;

    iVar1 = 0;
    iVar2 = (u8 *)param_1 + 8;
    do {
        ov49_0225F074(iVar2);
        iVar1 = iVar1 + 1;
        iVar2 = iVar2 + 0x24;
    } while (iVar1 < 0x14);
    ov49_0225F074((u8 *)param_1 + 0x2d8);
    Heap_Free(param_1);
    return;
}

void ov49_0225EF24(void *arg0) {
    ov49_0225F018(arg0, 0);
}

void ov49_0225EF30(void *arg0) {
    ov49_0225F018(arg0, 1);
}

s32 ov49_0225EF3C(UnkStruct_ov49_0225EF3C *arg0) {
    return arg0->unk_C;
}

void *ov49_0225EF40(void *arg0, u32 size) {
    void *v0;

    if (*(void **)((u8 *)arg0 + 8) != NULL) {
        GF_AssertFail();
    }
    v0 = Heap_Alloc((enum HeapID) * (u16 *)arg0, size);
    *(void **)((u8 *)arg0 + 8) = v0;
    memset(v0, 0, size);
    return *(void **)((u8 *)arg0 + 8);
}

void ov49_0225EF68(void *arg0) {
    if (*(void **)((u8 *)arg0 + 8) == NULL) {
        GF_AssertFail();
    }
    Heap_Free(*(void **)((u8 *)arg0 + 8));
    *(void **)((u8 *)arg0 + 8) = NULL;
}

void *ov49_0225EF84(void *arg0) {
    return *(void **)((u8 *)arg0 + 8);
}

s32 ov49_0225EF88(UnkStruct_ov49_0225EF88 *arg0) {
    return arg0->unk_10;
}

void ov49_0225EF8C(UnkStruct_ov49_0225EF8C *arg0, s32 arg1) {
    arg0->unk_10 = arg1;
}

void ov49_0225EF90(UnkStruct_ov49_0225EF90 *arg0) {
    arg0->unk_10 = (s32)(arg0->unk_10 + 1);
}

void ov49_0225EF98(s32 arg0, u32 arg1, void *arg2, s32 arg3) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg1 >= 0x14U) {
        GF_AssertFail();
    }
    ov49_0225F0D8(arg0 + 8 + (0x24 * arg1), arg2, arg3);
}

void ov49_0225EFC4(s32 arg0, u32 arg1, void *arg2, void *arg3) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg1 >= 0x14U) {
        GF_AssertFail();
    }
    ov49_0225F110(arg0 + 8 + (0x24 * arg1), arg2, arg3);
}

void ov49_0225EFF0(u8 *arg0, u32 arg1, s32 arg2) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg1 >= 0x14U) {
        GF_AssertFail();
    }
    ov49_0225F10C(arg0 + 8 + (0x24 * arg1), arg2);
}
