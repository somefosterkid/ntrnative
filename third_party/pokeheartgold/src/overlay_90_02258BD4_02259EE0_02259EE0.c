#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"

void ov90_02259FC8(UnkStruct_ov114_0225DAC0 *param0, s32 param1);

s32 ov90_02258CF0(UnkStruct_ov114_0225CBF0 *, s32, s32, s32, s32);

void ov90_02259EE0(UnkStruct_ov114_0225DAC0 *param0, s16 param1, s16 param2);

void ov90_02259EE0(UnkStruct_ov114_0225DAC0 *param0, s16 param1, s16 param2) {
    f32 v0;

    if ((s32)param1 > 0) {
        v0 = 0.5f + (f32)(param1 << 0xC);
    } else {
        v0 = (f32)(param1 << 0xC) - 0.5f;
    }
    ov90_02258CF0(&param0->unk_1C, 0xFFF01000, (s32)v0, 0x1C000, 8);
    param0->unk_30 = 0;
    param0->unk_32 = param2;
    param0->unk_34 = 0x57E;
    ov90_02259FC8(param0, 0 - ((s32)param0->unk_1C.unk_00 >> 0xC));
}
