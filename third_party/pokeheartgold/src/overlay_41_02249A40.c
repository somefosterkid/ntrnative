#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

void ov41_02249A40(UnkStruct_ov22_02259560 *arg0);

void ov41_02249A50(UnkStruct_ov22_02259560 *arg0, UnkStruct_ov22_02259560 *arg1);
void ov41_02249A60(UnkStruct_ov22_02259560 *arg0);

void ov41_02249A40(UnkStruct_ov22_02259560 *arg0) {
    u32 v1;
    s8 *v0;

    v0 = (s8 *)arg0;
    v1 = sizeof(UnkStruct_ov22_02259560);
    do {
        *v0 = 0;
        v0 += 1;
        v1 -= 1;
    } while (v1 != 0);
}

void ov41_02249A50(UnkStruct_ov22_02259560 *arg0, UnkStruct_ov22_02259560 *arg1) {
    arg0->unk_08 = arg1->unk_08;
    arg1->unk_08->unk_0C = arg0;
    arg0->unk_0C = arg1;
    arg1->unk_08 = arg0;
}

void ov41_02249A60(UnkStruct_ov22_02259560 *arg0) {
    arg0->unk_0C->unk_08 = arg0->unk_08;
    arg0->unk_08->unk_0C = arg0->unk_0C;
}
