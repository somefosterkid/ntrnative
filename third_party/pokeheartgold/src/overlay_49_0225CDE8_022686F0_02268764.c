#include "global.h"

#include "heap.h"
#include "overlay_49_unk_struct_ov70_0226_c6f8.h"

s32 ov45_0222A324(void *);

s32 ov45_0222A330(void *);

s32 ov45_0222A35C(void *);

s32 ov45_0222A374(void *);

s32 ov45_0222A394(void *);

s32 ov45_0222A3A0(void *);

struct UnkStruct_ov66_0222DFF8_t;

u8 ov45_0222A3BC(struct UnkStruct_ov66_0222DFF8_t *);

u8 ov45_0222A3D4(struct UnkStruct_ov66_0222DFF8_t *);

u8 ov45_0222A3EC(struct UnkStruct_ov66_0222DFF8_t *);

s32 ov49_02258BEC(void *, s32);

void *ov49_02259FE8(void *);

void *ov49_02259FF0(void *);

void *ov49_02259FF8(void *);

s32 ov49_0225E574(void *);

s32 ov49_0225E714(void *);

s32 ov49_0225E760(void *, s32);

void *ov49_02268490(enum HeapID, s32, s32, s32);

s32 ov49_02268974();

s32 ov49_022689A0();

s32 ov49_022689D4();

s32 ov49_02268A00(UnkStruct_ov70_0226C6F8 *);

void *ov49_02268A0C(void *, void *, enum HeapID);

void *ov49_02268FAC(void *, enum HeapID);

UnkStruct_ov70_0226C6F8 *ov49_02268764(enum HeapID arg0, void *arg1);

UnkStruct_ov70_0226C6F8 *ov49_02268764(enum HeapID heapID, void *arg1) {
    s32 v1;
    s32 v0;
    s32 v3;
    s32 v5;
    s32 v6;
    u32 v7;
    s32 v4;
    UnkStruct_ov70_0226C6F8 *v2;
    s8 *v8;
    v2 = Heap_Alloc(heapID, sizeof(UnkStruct_ov70_0226C6F8));
    v8 = (s8 *)v2;
    v7 = sizeof(UnkStruct_ov70_0226C6F8);
    do {
        *v8 = 0;
        v8 += 1;
        v7 -= 1;
    } while (v7 != 0);
    v2->unk_00 = arg1;
    v2->unk_04 = ov49_02259FE8(arg1);
    v2->unk_08 = ov49_02259FF8(arg1);
    v2->unk_0C = ov49_02259FF0(arg1);
    ov45_0222A3BC(v2->unk_04);
    v5 = ov49_02268974();
    ov45_0222A3D4(v2->unk_04);
    v1 = ov49_022689A0();
    ov45_0222A3EC(v2->unk_04);
    v2->unk_10 = ov49_02268490(heapID, v5, v1, ov49_022689D4());
    v2->unk_14 = ov49_02268FAC(arg1, heapID);
    v2->unk_18 = ov49_02268A0C(v2->unk_04, v2->unk_08, heapID);
    ov49_02258BEC(v2->unk_0C, v5);
    v3 = ov45_0222A35C(v2->unk_04);
    ov45_0222A324(v2->unk_04);
    v0 = ov45_0222A374(v2->unk_04);
    v6 = ov45_0222A3A0(v2->unk_04);
    v4 = ov45_0222A330(v2->unk_04);
    ov45_0222A394(v2->unk_04);
    if (v3 == 2) {
        ov49_0225E714(v2->unk_08);
    }
    if (((v3 != 1) && (v3 == 0)) && (v6 == 1)) {
        ov49_0225E760(v2->unk_08, 3);
    }
    if (v0 == 1) {
        ov49_0225E574(v2->unk_08);
    }
    if (v4 == 1) {
        ov49_02268A00(v2);
    }
    return v2;
}
