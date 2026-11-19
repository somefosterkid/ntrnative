#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_app_02210C98.h"

void ov96_022132FC(PokeathlonCourseData *arg0);

void ov96_022132FC(PokeathlonCourseData *arg0) {
    s32 temp_r2;
    s32 var_r4;
    s32 var_r5;
    s32 var_r7;
    Ov96App_02210C98_Unit *temp_r0;
    Ov96App_02210C98 *var_r6;

    var_r6 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    var_r5 = 0;
    var_r7 = 3;
    do {
        var_r4 = var_r5 / 3;
        temp_r0 = &var_r6->unk_5C[var_r4][var_r5 % 3];
        temp_r2 = temp_r0->unk_78;
        if (temp_r2 != 2) {
            if ((u32)temp_r0->unk_71 < 0x1EU) {
                temp_r0->unk_78 = 3;
            } else if (temp_r2 != 1) {
                temp_r0->unk_78 = 0;
            }
        }
        var_r5 = (s32)(u8)(var_r5 + 1);
    } while ((u32)var_r5 < 0xCU);
}
