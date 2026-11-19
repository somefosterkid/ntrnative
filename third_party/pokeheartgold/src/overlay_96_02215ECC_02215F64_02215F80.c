#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_022186B8(Ov96RdObj *obj, const Ov96RdPos16 *pos);

void ov96_02215F80(PokeathlonCourseData *data, s32 idx, const Ov96RdPos16 *pos);

void ov96_02215F80(PokeathlonCourseData *data, s32 idx, const Ov96RdPos16 *pos) {
    ov96_022186B8(&((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->course.players[idx].obj, pos);
}
