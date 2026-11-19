#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "unk_0208F814.h"

BOOL sub_02037108(int cmd, const void *data, int size);

u16 sub_0203769C(void);

void sub_0208FAF8(UnkStruct_02095EAC *param0, int param1);

void sub_0208F828(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;

    if (sub_0203769C() != 0) {
        UnkStruct_02095EAC_sub1 *v1 = (UnkStruct_02095EAC_sub1 *)param2;

        if (v1->unk_3EC * 1000 > 30 * 15 * 32) {
            MI_CpuCopyFast(v1->unk_00, &v0->unk_4434[v1->unk_3EC * 1000], (30 * 15 * 32) % 1000);
        } else {
            MI_CpuCopyFast(v1->unk_00, &v0->unk_4434[v1->unk_3EC * 1000], 1000);
        }

        MI_CpuCopyFast(v0->unk_4434, v0->unk_32C.pixelBuffer, 30 * 15 * 32);
        CopyWindowToVram(&v0->unk_32C);
    } else {
        UnkStruct_02095EAC_sub1 *v2 = (UnkStruct_02095EAC_sub1 *)param2;

        if (v0->unk_4430 * 1000 < 30 * 15 * 32) {
            v0->unk_4430++;
            sub_0208FAF8(v0, v0->unk_4430);
        } else {
            sub_02037108(124, NULL, 0);
        }
    }
}

void sub_0208F8BC(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;
    UnkStruct_ov58_021D2820 *v1 = (UnkStruct_ov58_021D2820 *)param2;

    if (param0 != 0) {
        v0->unk_9421[param0] = *v1;
    }
}
