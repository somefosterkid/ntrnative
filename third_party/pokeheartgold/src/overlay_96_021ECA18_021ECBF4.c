#include "global.h"

#include "sys_task.h"
#include "overlay_96_app_021ed1d0.h"

void Heap_Free(void *ptr);

void ov96_021ECBF4(void *ptr);

void ov96_021ECBF4(void *ptr) {
    Heap_Free(ptr);
}

s32 ov96_021ECD24(Ov96PlayerSlot *);

void ov96_021ECBFC(Ov96PlayerSlot *arg0);

void ov96_021ECBFC(Ov96PlayerSlot *arg0) {
    s32 v0;
    Ov96PlayerSlot *v1;

    v1 = arg0;
    v0 = 0;
    do {
        ov96_021ECD24(v1);
        v0 += 1;
        v1 += 1;
    } while (v0 < 4);
}

s32 ov96_021ECDC4(Ov96PlayerSlot *);

s32 ov96_021ECC14(Ov96PlayerSlot *arg0);

s32 ov96_021ECC14(Ov96PlayerSlot *arg0) {
    s32 v0;
    Ov96PlayerSlot *v1;
    s32 v2;

    v0 = 0;
    v1 = arg0;
    v2 = 1;
    do {
        if (ov96_021ECDC4(v1) == 0) {
            v2 = 0;
        }
        v0 += 1;
        v1 += 1;
    } while (v0 < 4);
    return v2;
}

s32 ov96_021ECDD4(Ov96PlayerSlot *, s32);

void ov96_021ECC38(Ov96PlayerSlot *arg0, s32 arg1, s32 arg2);

void ov96_021ECC38(Ov96PlayerSlot *arg0, s32 arg1, s32 arg2) {
    if (arg2 != 0) {
        ov96_021ECDD4(&arg0[arg1], arg2);
    }
}

s32 ov96_021ECC4C(Ov96PlayerSlot *arg0, s32 arg1);

s32 ov96_021ECC4C(Ov96PlayerSlot *arg0, s32 arg1) {
    return arg0[arg1].target;
}

s32 ov96_021ECC58(s32 *arg0, s32 *arg1);

s32 ov96_021ECC58(s32 *arg0, s32 *arg1) {
    u16 v0;
    u16 v1;

    v1 = (u16)*arg1;
    v0 = (u16)*arg0;
    if (v0 == v1) {
        return 0;
    }
    if ((u32)v0 < (u32)v1) {
        return 1;
    }
    return -1;
}
