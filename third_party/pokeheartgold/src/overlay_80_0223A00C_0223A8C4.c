#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_01_unk_struct_ov83_0223_d9_ec.h"

void ov80_0223A8C4(UnkStruct_ov83_0223D9EC *param0, fx32 param1, fx32 param2, int param3);
BOOL ov80_0223A8D4(UnkStruct_ov83_0223D9EC *param0);
VecFx32 ov80_0223A91C(fx32 param0, fx32 param1, fx32 param2);

void ov80_0223A8C4(UnkStruct_ov83_0223D9EC *param0, fx32 param1, fx32 param2, int param3) {
    param0->unk_00 = param1;
    param0->unk_04 = param1;
    param0->unk_08 = param2 - param1;
    param0->unk_10 = param3;
    param0->unk_0C = 0;
}

BOOL ov80_0223A8D4(UnkStruct_ov83_0223D9EC *param0) {
    fx32 v0;

    v0 = FX_MulInline(param0->unk_08, param0->unk_0C << 12);
    v0 = FX_Div(v0, param0->unk_10 << 12);

    param0->unk_00 = v0 + param0->unk_04;

    if ((param0->unk_0C + 1) <= param0->unk_10) {
        param0->unk_0C++;
        return 0;
    }

    param0->unk_0C = param0->unk_10;

    return 1;
}

VecFx32 ov80_0223A91C(fx32 param0, fx32 param1, fx32 param2) {
    VecFx32 v0;

    v0.x = param0;
    v0.y = param1;
    v0.z = param2;
    return v0;
}
