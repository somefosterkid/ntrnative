#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_02218784(PokeathlonCourseData *data, s32 row);

void ov96_02218784(PokeathlonCourseData *data, s32 row) {
    Ov96RdWork *work;
    u8 *hit;
    s32 i;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    i = 0;
    hit = work->course.hit[row];
    do {
        i++;
        *hit = 0;
        hit++;
    } while (i < 4);
}
