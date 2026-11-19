#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void ov96_021F6424(s32 (*arg0)[0x80]);

s32 ov96_021E8318(PokeathlonCourseData *, u16);

s32 ov96_021EB144(void *, s32);

s32 ov96_021EB63C(void *, s32);

s32 ov96_021F5B60(PokeathlonCourseData *);

s32 ov96_021F6600(PokeathlonCourseData *, UnkStruct_ov96_021F5018 *);

void *ov96_021F6BB0(UnkStruct_ov96_021F5018 *);

s32 ov96_021F70AC(UnkStruct_ov96_021F5018 *, s32);

s32 ov96_021F7194(PokeathlonCourseData *, UnkStruct_ov96_021F5018 *);

s32 ov96_021F7598(UnkStruct_ov96_021F74A4 *);

s32 ov96_021F5A88(PokeathlonCourseData *arg0);

s32 ov96_021F5A88(PokeathlonCourseData *arg0) {
    s32 var_r1;
    s32 *temp_r5;
    u16 temp_r0;
    UnkStruct_ov96_021F5018 *temp_r4;
    u8 var_r0;
    s32 var_r5;

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r5 = (s32 *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0));
    temp_r4->unk_142 = 0U;
    if (temp_r4->unk_13C != 0) {
        temp_r4->unk_13C = (temp_r4->unk_13C - 1);
    }
    if (temp_r4->unk_13C <= 0) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    if (var_r0 != 0) {
        var_r5 = 0;
        ov96_021EB63C(temp_r4->unk_60, 1);
        ov96_021EB144(temp_r4->unk_18C, 1);
        var_r1 = 0;
        do {
            temp_r0 = temp_r4->unk_90[var_r1].unk_20;
            var_r1 += 1;
            var_r5 += temp_r0;
        } while (var_r1 < 3);
        if (var_r5 > 0x3E7) {
            var_r5 = 0x3E7;
        }
        ov96_021E8318(arg0, (u16)var_r5);
        ov96_021F70AC(temp_r4, (u16)var_r5);
        ov96_021F5B60(arg0);
        return 1;
    }
    ov96_021F6600(arg0, temp_r4);
    ov96_021F6424(temp_r4->unk_1A0);
    ov96_021F6BB0(temp_r4);
    ov96_021F7194(arg0, temp_r4);
    ov96_021F7598(temp_r4->unk_8C);
    ov96_021F5B60(arg0);
    *temp_r5 = (s32)temp_r4->unk_142;
    return 0;
}
