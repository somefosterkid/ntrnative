#include "global.h"

#include "pokeathlon/pokeathlon.h"

s32 ov96_021E8318(PokeathlonCourseData *, u8 *);

s32 ov96_02207774(void *, u8 *);

void ov96_02207C64(PokeathlonCourseData *arg0);

void ov96_02207C64(PokeathlonCourseData *arg0) {
    PokeathlonCourseData *var_r4;
    u8 *var_r5;
    u8 temp_r1;
    void *var_r6;

    var_r4 = arg0;
    var_r5 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0xF0);
    var_r6 = PokeathlonCourse_GetHeapAllocPtr4(var_r4);
    temp_r1 = (u8)ov96_021E5F24(var_r4);
    var_r5 = (u8 *)(u16)((u16)(((u32) * (u8 **)(var_r5 + (temp_r1 * 4)) >> 0x12) & 0x3FF) + (u8)(0x1F & ((s32) * (u16 *)((u8 *)(var_r5 + (temp_r1 * 2)) + 0x18) >> 0xB)));
    if ((u32)var_r5 > 0x3E7U) {
        var_r5 = (u8 *)0x3FF;
        var_r5 = (u8 *)0x3E7;
    }
    ov96_021E8318(var_r4, var_r5);
    ov96_02207774(var_r6, var_r5);
}
