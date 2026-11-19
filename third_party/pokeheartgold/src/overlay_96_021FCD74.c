#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_ov96_rower_work.h"

void ov96_021EB5BC(void *param_1);

u32 ov96_021FCD74(PokeathlonCourseData *param_1);

u32 ov96_021FCD74(PokeathlonCourseData *param_1) {
    Ov96RowerWork *pvVar1;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    ov96_021EB5BC(pvVar1->unk18);
    return 1;
}
