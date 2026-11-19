#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_02215EE8(PokeathlonCourseData *data, s32 idx, const Ov96RdPos16 *pos);

void ov96_02215EE8(PokeathlonCourseData *data, s32 idx, const Ov96RdPos16 *pos) {
    Ov96RdWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    GF_ASSERT(!work->course.players[idx].obj.f28);
    work->course.players[idx].obj.f30 = 1;
    work->course.players[idx].obj.target.x = pos->x;
    work->course.players[idx].obj.target.y = pos->y;
}
