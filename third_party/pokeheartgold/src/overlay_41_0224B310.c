#include "global.h"
struct UnkStruct_ov22_0225AB54;
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_01_unk_struct_ov83_0223_d9_ec.h"

void ov41_0224B310(struct UnkStruct_ov22_0225AB54 *arg0);

void ov41_0224B31C(UnkStruct_ov83_0223D9EC *param0, fx32 param1, fx32 param2, int param3);
BOOL ov41_0224B32C(UnkStruct_ov83_0223D9EC *param0);

void ov41_0224B310(UnkStruct_ov22_0225AB54 *arg0) {
    s32 v0;

    v0 = *((s32 *)&arg0->unk_20) - 1;
    if (v0 >= 0) {
        *((s32 *)&arg0->unk_20) = v0;
    }
}

void ov41_0224B31C(UnkStruct_ov83_0223D9EC *param0, fx32 param1, fx32 param2, int param3) {
    param0->unk_00 = param1;
    param0->unk_04 = param1;
    param0->unk_08 = param2 - param1;
    param0->unk_10 = param3;
    param0->unk_0C = 0;
}

BOOL ov41_0224B32C(UnkStruct_ov83_0223D9EC *param0) {
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
