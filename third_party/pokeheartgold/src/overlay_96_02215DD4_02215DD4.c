#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

VecFx32 *ov96_02215DD4(PokeathlonCourseData *data, s32 idx);

VecFx32 *ov96_02215DD4(PokeathlonCourseData *data, s32 idx) {
    Ov96RdWork *work = PokeathlonCourse_GetHeapAllocPtr4(data);
    Ov96RdObj *obj = &work->course.players[idx].obj;
    return &obj->pos2C;
}
