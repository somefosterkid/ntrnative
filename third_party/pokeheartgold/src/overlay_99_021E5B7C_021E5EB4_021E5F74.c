#include "global.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "overlay_99_pokeathlon_course_record.h"

extern s32 ov99_021E9690;

void ov99_021E5F74(void *arg0);

void ov99_021E5F74(void *arg0) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r6;
    u8 var_r4;
    void *temp_r2;
    if (0 != (*((s32 *)&((PokeathlonCourseRecord *)arg0)->selection.scrolling))) {
        temp_r0 = Bg_GetXpos(((PokeathlonCourseRecord *)arg0)->bgConfig, GF_BG_LYR_SUB_1_F);
        var_r6 = temp_r0 >> 2;
        if (temp_r0 == var_r6) {
            var_r6 = 0;
        }
        ScheduleSetBgPosText(((PokeathlonCourseRecord *)arg0)->bgConfig, 2U, BG_POS_OP_SET_ROT, var_r6);
        ScheduleSetBgPosText(((PokeathlonCourseRecord *)arg0)->bgConfig, 5U, BG_POS_OP_SET_ROT, var_r6);
        var_r4 = 0;
        do {
            temp_r0_2 = var_r4;
            temp_r0_2 = 0x3 * temp_r0_2;
            temp_r2 = (&ov99_021E9690) + temp_r0_2;
            ManagedSprite_SetPositionXY(((PokeathlonCourseRecord *)arg0)->sprites[(*((s32 *)((&ov99_021E9690) + temp_r0_2)))], (s16)((*((s32 *)(((u8 *)temp_r2) + 4))) - var_r6), (s16)(*((s32 *)(((u8 *)temp_r2) + 8))));
            var_r4 += 1;
        } while (((u32)var_r4) < 6U);
        if (var_r6 == 0) {
            *((s32 *)&((PokeathlonCourseRecord *)arg0)->selection.scrolling) = 0;
        }
    }
}
