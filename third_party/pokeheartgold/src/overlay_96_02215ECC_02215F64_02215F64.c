#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_0221910C(Ov96RdObj *obj);

void ov96_02215F64(PokeathlonCourseData *arg0, s32 arg1);

void ov96_02215F64(PokeathlonCourseData *data, s32 idx) {
    ov96_0221910C(&((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->course.players[idx].obj);
}
