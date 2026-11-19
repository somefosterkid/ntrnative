#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

UnkStruct_ov96_0220E3F0_Record *ov96_0220E74C(UnkStruct_ov96_0220E3F0 *, u32);

s32 ov96_0220E770(UnkStruct_ov96_0220E3F0 *, u32, u32, s32 *, s32 *);

s32 ov96_02210C48(s32, s32, s32, s32, s32, s32);

s32 ov96_02210BD0(void *arg0, u16 arg1, s32 arg2);

s32 ov96_02210BD0(void *arg0, u16 arg1, s32 arg2) {
    s32 spC;
    s32 sp8;
    s32 temp_r2;
    UnkStruct_ov96_0220E3F0_Record *temp_r0;
    u8 *new_var;
    UnkStruct_ov96_0220E3F0 *temp_r0_2;
    temp_r0_2 = PokeathlonCourse_GetHeapAllocPtr4(*((PokeathlonCourseData **)arg0));
    temp_r0 = ov96_0220E74C(temp_r0_2, (u8)arg1);
    if (((s32)temp_r0->unk_2) < 0x120) {
        return 0;
    }
    new_var = (u8 *)arg0;
    temp_r2 = *((s32 *)(new_var + 0xC));
    ov96_0220E770(temp_r0_2, (u8)(((u32)(temp_r2 << 0x1B)) >> 0x1E), (u8)(((u32)((*((s32 *)(((u8 *)arg0) + 0xC))) << 0x19)) >> 0x1E), &spC, &sp8);
    return ov96_02210C48(*((s16 *)(((u8 *)(&sp8)) + 4)), *((s16 *)(((u8 *)(&sp8)) + 6)), sp8, temp_r0->unk_0, (s32)((s16)(temp_r0->unk_2 - 0x120)), arg2);
}
