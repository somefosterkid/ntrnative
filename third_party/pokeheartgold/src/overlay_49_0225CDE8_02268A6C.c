#include "global.h"

#include "heap.h"
#include "unk_struct_ov49_02268db0.h"

s32 ov49_02268DB0(UnkStruct_ov49_02268DB0 *arg0);

void ov49_02268A6C(void *arg0);

void ov49_02268A6C(void *ptr) {
    ov49_02268DB0(ptr);
    Heap_Free(ptr);
}

u16 ov45_0222A35C(void *);

s32 ov45_0222B1DC(void *);

s32 ov45_0222B1EC(void *);

s32 ov49_0225E624(void *, s32);

s32 ov49_02268ADC(void *, s32, s32);

s32 ov49_02268C74(void *, s32, s32);

s32 ov49_02268D94(void *);

void ov49_02268A7C(void *arg0);

void ov49_02268A7C(void *arg0) {
    s32 v1;
    s32 v2;
    u16 v0;

    v1 = ov45_0222B1DC(*(void **)arg0);
    v2 = ov45_0222B1EC(*(void **)arg0);
    v0 = ov45_0222A35C(*(void **)arg0);
    if (*(u16 *)((u8 *)arg0 + 0xA) != v0) {
        *(u16 *)((u8 *)arg0 + 0xA) = v0;
        if (v0 == 1) {
            ov49_0225E624(*(void **)((u8 *)arg0 + 4), 1);
            ov49_02268C74(arg0, v1, v2);
            ov49_02268D94(arg0);
        }
    }
    if (v0 != 0) {
        if (*(u8 *)((u8 *)arg0 + 8) != v1) {
            ov49_02268C74(arg0, v1, v2);
        }
        ov49_02268ADC(arg0, v1, v2);
    }
}
