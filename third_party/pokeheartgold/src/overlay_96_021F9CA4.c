#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_icon_work.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

void ov96_021EB5BC(void *param_1);

BOOL ov96_021F9CA4(PokeathlonCourseData *param_1, int unused);

BOOL ov96_021F9CA4(PokeathlonCourseData *param_1, int unused) {
    UnkStruct_ov96_021F94A8 *pvVar1;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    ov96_021FC314(pvVar1->unk_228);
    ov96_021EB5BC(pvVar1->unk_224);
    return 1;
}
