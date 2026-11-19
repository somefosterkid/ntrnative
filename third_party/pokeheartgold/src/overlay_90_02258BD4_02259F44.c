#include "global.h"

#include "overlay_90.h"
#include "unk_02005D10.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"

s32 ov90_02258CF0(void *, s32, s32, s32, s32);

s32 ov90_02259FC8(void *, s32);

void ov90_02259F44(void *arg0, s16 arg1) {
    ov90_02258CF0(arg0 + 0x1C, *((s32 *)&((UnkStruct_ov114_0225DAC0 *)arg0)->unk_1C.unk_00), 0xFFF01000, 0, 8);
    ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_30 = 0;
    ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_32 = arg1;
    ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_34 = 0x57F;
    ov90_02259FC8(arg0, 0 - ((s32) *((s32 *)&((UnkStruct_ov114_0225DAC0 *)arg0)->unk_1C.unk_00) >> 0xC));
}

s32 ov90_02258D4C(void *, s32);

s32 ov90_02259F7C(void *arg0);

s32 ov90_02259F7C(void *arg0) {
    s16 v0;
    s32 v1;

    v0 = ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_32;
    if ((s32)v0 <= 0) {
        if (((UnkStruct_ov114_0225DAC0 *)arg0)->unk_30 == 0) {
            PlaySE((u16) ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_34);
        }
        ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_30 = (s16)(((UnkStruct_ov114_0225DAC0 *)arg0)->unk_30 + 1);
        v1 = ov90_02258D4C(arg0 + 0x1C, ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_30);
        ov90_02259FC8(arg0, 0 - ((s32) *((s32 *)&((UnkStruct_ov114_0225DAC0 *)arg0)->unk_1C.unk_00) >> 0xC));
        return v1;
    }
    ((UnkStruct_ov114_0225DAC0 *)arg0)->unk_32 = (s16)(v0 - 1);
    return 0;
}
