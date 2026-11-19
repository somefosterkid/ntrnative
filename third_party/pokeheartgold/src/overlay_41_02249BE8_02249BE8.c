#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"


void ov41_02249AF4(UnkStruct_ov22_02259560 *param0, int param1, int param2);

void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);

void ov41_02249BE8(UnkStruct_ov22_02259560 *param0, int param1, int param2);

void ov41_02249BE8(UnkStruct_ov22_02259560 *param0, int param1, int param2) {
    UnkStruct_ov22_02259560 *v0;
    int v1, v2;

    v0 = param0->unk_08;

    while (v0 != param0) {
        ov41_02249B44(v0, &v1, &v2);

        v1 += param1;
        v2 += param2;

        ov41_02249AF4(v0, v1, v2);

        v0 = v0->unk_08;
    }
}
