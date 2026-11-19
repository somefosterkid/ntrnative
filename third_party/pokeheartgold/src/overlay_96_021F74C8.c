#include "global.h"

#include "sys_task.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"
#include "overlay_96_ov96_sprite_mgr.h"

void Heap_Free(void *ptr);

void ov96_021F74C8(UnkStruct_ov96_021F74A4 *ptr);

void ov96_021F74C8(UnkStruct_ov96_021F74A4 *ptr) {
    Heap_Free(ptr);
}

void ov96_021F74D0(UnkStruct_ov96_021F74A4 *arg0, void *arg1, s32 arg2);

void ov96_021F74D0(UnkStruct_ov96_021F74A4 *arg0, void *arg1, s32 arg2) {
    void *v0;
    s32 v2;
    UnkStruct_ov96_021F74A4_Entry *v1;

    v1 = arg0->entries;
    v2 = 0;
    do {
        v0 = ov96_021EB3E4(arg1, arg2, 3, 0x65, 2);
        v1->unk_10 = v0;
        ov96_021EB52C(v0, 1, 1);
        ov96_021EB564(v1->unk_10, 8);
        v1->unk_14 = ov96_021EB3E4(arg1, arg2, 3, 0x65, 5);
        v1->unk_18 = ov96_021EB3E4(arg1, arg2, 3, 0x65, 6);
        ov96_021EB630(v1->unk_10, 0x14);
        ov96_021EB630(v1->unk_14, 7);
        ov96_021EB630(v1->unk_18, 8);
        v2 += 1;
        v1 += 1;
    } while (v2 < 0x1D);
}
