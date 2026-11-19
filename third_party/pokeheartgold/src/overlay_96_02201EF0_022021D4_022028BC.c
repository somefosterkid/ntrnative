#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "system.h"
#include "touchscreen.h"
#include "overlay_96_ov96_02203468work.h"

s32 ov96_021E8228(PokeathlonCourseData *, u32, s32, s32, s32);

s32 ov96_021E8324(PokeathlonCourseData *, s32 *);

s32 ov96_021EB144(void *, s32);

s32 ov96_021EB63C(void *arg0, s32 arg1);

s32 ov96_02202958(PokeathlonCourseData *);

extern s32 ov96_022038D4;

s32 ov96_022028BC(PokeathlonCourseData *arg0);

s32 ov96_022028BC(PokeathlonCourseData *arg0) {
    PokeathlonCourseData *var_r5;
    u8 *temp_r0;
    PokeathlonTouchRecord *var_r4;
    PokeathlonTally *tally;
    u8 *var_r7;
    Ov96_02203468Work *var_r6;
    s32 temp_r1;
    u8 var_flag;
    var_r5 = arg0;
    var_r6 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(var_r5);
    var_r7 = temp_r0;
    var_r4 = ov96_021E8A20(temp_r0);
    var_r7 += 0xF0;
    var_r4->touching = 0;
    tally = ov96_021E8A20(var_r7);
    temp_r1 = tally->pairs[4].word0;
    var_flag = 1;
    var_flag &= temp_r1 >> 0x18;
    if (var_flag) {
        ov96_021EB63C(var_r6->unk_48, 1);
        ov96_021EB144(var_r6->unk_5D8, 1);
        ov96_021E8324(var_r5, &ov96_022038D4);
        return 1;
    }
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(var_r5, (u8)ov96_021E5F24(var_r5), 3, 0, 1);
    }
    if (System_GetTouchHeld() != 0) {
        var_r4->touchX = (s8)gSystem.touchX;
        var_r4->touchY = (s8)gSystem.touchY;
        var_r4->touching = 1;
    }
    ov96_02202958(var_r5);
    return 0;
}
