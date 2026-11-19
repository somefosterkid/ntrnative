#include "global.h"

#include "heap.h"
#include "overlay_49_unk_struct_ov70_0226_c6f8.h"


void *ov49_022684F4(void *);

s32 ov49_02268A6C(void *);

void *ov49_02269090(void *);

void ov49_02268850(UnkStruct_ov70_0226C6F8 *ptr);

void ov49_02268850(UnkStruct_ov70_0226C6F8 *ptr) {
    ov49_02268A6C(ptr->unk_18);
    ov49_02269090(ptr->unk_14);
    ov49_022684F4(ptr->unk_10);
    Heap_Free(ptr);
}

s32 ov45_0222A288(void *, s32);

s32 ov45_0222A330(void *);

s32 ov45_0222A35C(void *);

s32 ov45_0222A374(void *);

s32 ov45_0222A3A0(void *);

struct UnkStruct_ov66_0222DFF8_t;

u8 ov45_0222A3BC(struct UnkStruct_ov66_0222DFF8_t *);

u8 ov45_0222A3D4(struct UnkStruct_ov66_0222DFF8_t *);

u8 ov45_0222A3EC(struct UnkStruct_ov66_0222DFF8_t *);

s32 ov49_02258C08(void *, s32);

s32 ov49_0225E574(void *);

s32 ov49_0225E580(void *);

s32 ov49_0225E714(void *);

s32 ov49_0225E760(void *, s32);

s32 ov49_0225E824(void *);

s32 ov49_022684FC(void *);

s32 ov49_02268588(void *, s32);

s32 ov49_022685F8(void *, s32);

s32 ov49_02268620(void *, s32);

s32 ov49_02268974();

s32 ov49_022689A0();

s32 ov49_022689D4();

s32 ov49_02268A00(UnkStruct_ov70_0226C6F8 *);

s32 ov49_02268A7C(void *);

s32 ov49_02269098(void *);

void ov49_02268870(UnkStruct_ov70_0226C6F8 *arg0);

void ov49_02268870(UnkStruct_ov70_0226C6F8 *arg0) {
    s32 v0;

    if (ov45_0222A288(arg0->unk_04, 1) != 0) {
        ov45_0222A3BC(arg0->unk_04);
        v0 = ov49_02268974();
        ov49_02268588(arg0->unk_10, v0);
        ov49_02258C08(arg0->unk_0C, v0);
    }
    if (ov45_0222A288(arg0->unk_04, 2) != 0) {
        ov45_0222A3D4(arg0->unk_04);
        ov49_022685F8(arg0->unk_10, ov49_022689A0());
    }
    if (ov45_0222A288(arg0->unk_04, 3) != 0) {
        ov45_0222A3EC(arg0->unk_04);
        ov49_02268620(arg0->unk_10, ov49_022689D4());
    }
    if ((ov45_0222A3A0(arg0->unk_04) == 1) && (ov45_0222A35C(arg0->unk_04) == 0) && (ov49_0225E824(arg0->unk_08) == 0)) {
        ov49_0225E760(arg0->unk_08, 1);
    }
    if ((ov45_0222A288(arg0->unk_04, 5) != 0) && (ov45_0222A35C(arg0->unk_04) == 1)) {
        ov49_0225E714(arg0->unk_08);
        ov49_0225E760(arg0->unk_08, 2);
    }
    if ((ov45_0222A288(arg0->unk_04, 6) != 0) && (ov45_0222A374(arg0->unk_04) == 1)) {
        ov49_0225E574(arg0->unk_08);
    }
    if (ov45_0222A330(arg0->unk_04) == 1) {
        ov49_0225E580(arg0->unk_08);
        ov49_02268A00(arg0);
    }
    ov49_022684FC(arg0->unk_10);
    ov49_02269098(arg0->unk_14);
    ov49_02268A7C(arg0->unk_18);
}

s32 ov49_02269154(void *);

void ov49_02268968(UnkStruct_ov70_0226C6F8 *arg0);

void ov49_02268968(UnkStruct_ov70_0226C6F8 *arg0) {
    ov49_02269154(arg0->unk_14);
}
