#include "global.h"

#include "overlay_75.h"
#include "overlay_75_unk_struct_ov98_02247704.h"
#include "sys_task.h"

void ov75_022476E8(SysTask *param0, void *param1) {
    UnkStruct_ov75_022476E8 *v1 = param1;
    u32 v0;

    if (v1->unk_04 == 0) {
        return;
    }
    v1->unk_32B ^= 1;
    if (v1->unk_32B & 1) {
        return;
    }
    if (v1->unk_32C <= 1) {
        GX_LoadBGPltt(v1->unk_88[v1->unk_328], 0, 0x20);
    }
    v0 = v1->unk_32C;
    if ((v0 == 0) || (v0 == 2)) {
        GXS_LoadBGPltt(v1->unk_88[v1->unk_328], 0, 0x20);
    }
    if (v1->unk_32A == 0) {
        v1->unk_328++;
        if (v1->unk_328 >= 21) {
            v1->unk_328 = 19;
            v1->unk_32A ^= 1;
        }
    } else {
        v1->unk_328--;
        if (v1->unk_328 < 0) {
            v1->unk_328 = 1;
            v1->unk_32A ^= 1;
        }
    }
}
