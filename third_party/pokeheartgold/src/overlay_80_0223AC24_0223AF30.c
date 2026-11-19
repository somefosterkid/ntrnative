#include "global.h"

#include "heap.h"
#include "overlay_80_0223AC24.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "sys_task.h"

void *ov80_0223ADF4(enum HeapID);

void *ov80_0223AF30(enum HeapID arg0);

struct UnkStruct_ov104_0223EA84_t;
struct UnkStruct_ov104_0223EBD0_t;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov104_0223EA38;
typedef struct UnkStruct_ov104_0223EA84_t UnkStruct_ov104_0223EA84;
typedef struct UnkStruct_ov104_0223EBD0_t UnkStruct_ov104_0223EBD0;
struct UnkStruct_ov104_0223EA84_t {
    UnkStruct_ov104_0223EA38 unk_00;
    UnkStruct_ov104_0223EA38 unk_14;
    Window *unk_28;
    u8 unk_2C;
    u8 unk_2D;
    u8 unk_2E;
    u8 unk_2F;
};
struct UnkStruct_ov104_0223EBD0_t {
    Window *unk_00;
    UnkStruct_ov104_0223EA84 *unk_04[96];
    u8 unk_184;
    u8 unk_185;
    u8 unk_186;
    u8 unk_187;
    u8 unk_188;
    u8 unk_189;
    u8 unk_18A;
    s8 unk_18B;
    u8 unk_18C;
    u8 unk_18D;
    u8 unk_18E[1];
};
void ov80_0223AF80(UnkStruct_ov104_0223EBD0 *param0, u8 param1, u8 param2, Window *param3, u8 param4);

void *ov80_0223AF30(enum HeapID heapID) {
    s32 v1;
    void *v0;
    void *v2;

    v0 = Heap_Alloc(heapID, 0x190U);
    memset(v0, 0, 0x190U);
    v1 = 0;
    v2 = v0;
    do {
        *(void **)((u8 *)v2 + 4) = ov80_0223ADF4(heapID);
        v1 += 1;
        v2 += 4;
    } while (v1 < 0x60);
    return v0;
}

SysTask *ov80_0223AE0C(void *);

void ov80_0223AF60(void *ptr) {
    s32 v0;
    void *v1;

    v0 = 0;
    v1 = ptr;
    do {
        ov80_0223AE0C(*(void **)((u8 *)v1 + 4));
        v0 += 1;
        v1 += 4;
    } while (v0 < 0x60);
    Heap_Free(ptr);
}

void ov80_0223AF80(UnkStruct_ov104_0223EBD0 *param0, u8 param1, u8 param2, Window *param3, u8 param4) {
    param0->unk_00 = param3;
    param0->unk_18C = param4;
    param0->unk_189 = param1;
    param0->unk_18A = param2;
    param0->unk_184 = 0;
    param0->unk_185 = 0;
    param0->unk_186 = 0;
    param0->unk_187 = 0;
    param0->unk_188 = 0;
    param0->unk_18B = 0;
    param0->unk_18D = 1;
}
