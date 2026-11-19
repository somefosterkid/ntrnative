#include "global.h"

#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

u32 ov70_022421D0(UnkStruct_ov70_02242E58 *, u8 *);

s32 ov70_022430CC(UnkStruct_ov70_02242E58 *arg0);

s32 ov70_022430CC(UnkStruct_ov70_02242E58 *arg0) {
    u32 temp_r0;

    temp_r0 = ov70_022421D0(arg0, arg0->unk_64);
    if (temp_r0 != -2U) {
        if (temp_r0 <= 8U) {
            Sprite_SetDrawFlag(arg0->unk_C, 0);
            arg0->unk_4C = 6;
            arg0->unk_3C = (s16)temp_r0;
        }
    } else {
        Sprite_SetDrawFlag(arg0->unk_C, 0);
        sub_020198FC(arg0->unk_1C, 0, 4, 0, 4);
        arg0->unk_4C = 3;
        arg0->unk_50 = 6;
        arg0->unk_3C = -1;
    }
    return -1;
}
