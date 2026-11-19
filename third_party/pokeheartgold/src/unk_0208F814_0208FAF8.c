#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "unk_0208F814.h"

BOOL sub_02037088(int cmd, const void *data, int size);

void sub_0208FAF8(UnkStruct_02095EAC *param0, int param1);

void sub_0208FAF8(UnkStruct_02095EAC *param0, int param1) {
    u8 *v0 = (u8 *)param0->unk_32C.pixelBuffer;

    MI_CpuCopyFast(&v0[param1 * 1000], param0->unk_7C74.unk_00, 1000);

    {
        int v1;
        u32 *v2, v3;

        v2 = (u32 *)param0->unk_7C74.unk_00;

        for (v3 = 0, v1 = 0; v1 < 1000 / 4; v1++) {
            v3 ^= v2[v1];
        }

        param0->unk_7C74.unk_3E8 = v3;
    }

    param0->unk_7C74.unk_3EC = param1;

    sub_02037088(118, &param0->unk_7C74, sizeof(UnkStruct_02095EAC_sub1));
}

s32 sub_0208FB4C(s32 param_1, s32 param_2) {
    return param_2 + 0x8000 + param_1 * 0x3f0;
}

u32 sub_0208FB5C(void) {
    return 4;
}

u32 sub_0208FB60(void) {
    return 4;
}
