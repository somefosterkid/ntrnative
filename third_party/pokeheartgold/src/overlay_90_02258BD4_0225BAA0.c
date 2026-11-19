#include "global.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"

#include "overlay_90.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"

s32 ov45_0222ACB8(void *, u32, u32, u32, s32, s32, s32);

void ov90_0225BAA0(UnkStruct_ov114_0225E854_t *arg0) {
    u8 v0;
    UnkStruct_ov66_02230DBC *v1;

    v0 = arg0->unk_0C.unk_08;
    if ((v0 != 0) && ((u32) arg0->unk_1C.unk_14 >= 0xAU)) {
        v1 = arg0->unk_0C.unk_0C;
        ov45_0222ACB8(v1->unk_30, arg0->unk_07, v0, v1->unk_04.unk_00[0], (s32)v1->unk_04.unk_00[1], (s32)v1->unk_04.unk_00[2], (s32)v1->unk_04.unk_00[3]);
    }
}
