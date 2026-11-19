#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_02215F2C(PokeathlonCourseData *arg0, s32 arg1);

void ov96_02215F2C(PokeathlonCourseData *data, s32 idx) {
    Ov96RdWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    GF_ASSERT(!work->course.players[idx].obj.f28);
    GF_ASSERT(!work->course.players[idx].obj.f26);
    work->course.players[idx].obj.f29 = 1;
}
