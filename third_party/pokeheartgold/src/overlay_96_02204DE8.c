#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96hurdle_work.h"

void ov96_021EB5BC(void *param_1);

BOOL ov96_02204DE8(PokeathlonCourseData *param_1, int unused);

BOOL ov96_02204DE8(PokeathlonCourseData *param_1, int unused) {
    Ov96HurdleWork *pvVar1;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    ov96_021EB5BC(pvVar1->unk_20);
    return 1;
}
