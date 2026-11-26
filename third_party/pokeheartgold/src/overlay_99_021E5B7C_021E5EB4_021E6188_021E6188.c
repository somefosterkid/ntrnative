#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "overlay_99_pokeathlon_course_record.h"
#include "overlay_98_ov98text_sys.h"
#include "overlay_99.h"

s32 ov98_0221EF64(u32 arg0);

s32 ov99_021E5B70(const SaveData *);


void ov99_021E6188(void *arg0, int arg1);

void ov99_021E6188(void *arg0, int arg1) {
    MessageFormat *temp_r0;
    s32 temp_r0_2;
    Ov98TextSys *temp_r4;

    temp_r4 = ((PokeathlonCourseRecord *)arg0)->textContext;
    temp_r0 = ov98_0221EEFC(temp_r4);
    BufferPokeathlonMedalName(temp_r0, 0, (u8)arg1);
    ov98_0221EC08(((PokeathlonCourseRecord *)arg0)->textContext, 1, 4, 1, 0);
    BufferPokeathlonCourseName(temp_r0, 0, arg1 & 0xFF);
    ov98_0221EBD8(((PokeathlonCourseRecord *)arg0)->textContext, 3, 3, 1);
    if (ov99_021E5B70(*(void **)arg0) != 0) {
        temp_r0_2 = ov98_0221EF64(ov99_021E5B90(((PokeathlonCourseRecord *)arg0)->args, arg1));
        if (temp_r0_2 == 0) {
            ov98_0221EBD8(((PokeathlonCourseRecord *)arg0)->textContext, 4, 5, 1);
            return;
        }
        if (temp_r0_2 == 1) {
            ov98_0221EBD8(((PokeathlonCourseRecord *)arg0)->textContext, 4, 6, 1);
            return;
        }
        FillWindowPixelBuffer(&temp_r4->windows[4], 0U);
        ScheduleWindowCopyToVram(&temp_r4->windows[4]);
    }
}
