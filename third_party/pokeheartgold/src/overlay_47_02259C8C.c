#include "global.h"

#include "error_handling.h"
#include "pm_string.h"

s32 ov45_0222F544(s32);

u16 *ov45_0222F628(s32);

void ov47_02259C8C(void *arg0, s32 arg1, enum HeapID arg2);

void ov47_02259D24(void *arg0);
void *ov47_02258D18(void *, s32);
void *ov47_02259D40(void *arg0, void *arg1);
void *ov47_02258D44(void *, s32);
void *ov47_02259D58(void *arg0, void *arg1, s32 arg2);

void ov47_02259C8C(void *arg0, s32 arg1, enum HeapID heapID) {
    s32 v0;
    s32 v1;
    s32 v4;
    s32 v6;
    s32 v7;
    u16 *src;
    u16 *v2;
    void *v3;
    void *v5;

    v5 = arg0;
    v0 = arg1;
    *(String **)((u8 *)v5 + 0x10) = String_New(0x100U, heapID);
    v6 = 0;
    v3 = v5;
    do {
        *(String **)((u8 *)v3 + 0x14) = String_New(0x100U, heapID);
        v3 += 4;
        v6 += 1;
    } while (v6 < 3);
    if (v0 == 1) {
        v7 = 0;
    } else {
        v7 = 4;
    }
    *(s32 *)((u8 *)v5 + 8) = ov45_0222F544(v7 + 2);
    *(s32 *)v5 = ov45_0222F544(v7 + 1);
    *(s32 *)((u8 *)v5 + 4) = ov45_0222F544(v7);
    v1 = ov45_0222F544(v7 + 3);
    *(s32 *)((u8 *)v5 + 0xC) = v1;
    if (v1 == 1) {
        src = ov45_0222F628(v7);
        if (src == NULL) {
            GF_AssertFail();
        }
        CopyU16ArrayToString(*(String **)((u8 *)v5 + 0x10), src);
        v4 = 0;
        do {
            v2 = ov45_0222F628(v7 + 1 + v4);
            if (v2 == NULL) {
                GF_AssertFail();
            }
            CopyU16ArrayToString(*(String **)((u8 *)v5 + 0x14), v2);
            v4 += 1;
            v5 += 4;
        } while (v4 < 3);
    }
}

void ov47_02259D24(void *arg0) {
    s32 v0;
    void *v1;

    v1 = arg0;
    String_Delete(*(String **)((u8 *)v1 + 0x10));
    v0 = 0;
    do {
        String_Delete(*(String **)((u8 *)v1 + 0x14));
        v0 += 1;
        v1 += 4;
    } while (v0 < 3);
}

void *ov47_02259D40(void *arg0, void *arg1) {
    if (*(s32 *)((u8 *)arg0 + 0xC) == 0) {
        return ov47_02258D18(arg1, *(s32 *)arg0);
    }
    return *(void **)((u8 *)arg0 + 0x10);
}

void *ov47_02259D58(void *arg0, void *arg1, s32 arg2) {
    if (*(s32 *)((u8 *)arg0 + 0xC) == 0) {
        return ov47_02258D44(arg1, *(s32 *)arg0);
    }
    return *(void **)((u8 *)(arg0 + (arg2 * 4)) + 0x14);
}
