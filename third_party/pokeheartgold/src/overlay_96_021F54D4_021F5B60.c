#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

s32 ov96_021E6454(PokeathlonCourseData *, s32);

s32 ov96_021F5D3C(PokeathlonCourseData *);

s32 ov96_021F5EC4(PokeathlonCourseData *);

s32 ov96_021F637C(UnkStruct_ov96_021F5018 *);

s32 ov96_021F6E38(UnkStruct_ov96_021F5018 *);

s32 ov96_021F6F3C(UnkStruct_ov96_021F5018 *, s32);

void ov96_021F5B60(PokeathlonCourseData *arg0);

void ov96_021F5B60(PokeathlonCourseData *data) {
    UnkStruct_ov96_021F5018 *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_021F5D3C(data);
    ov96_021F6F3C(v0, (u16)v0->unk_13C);
    ov96_021F6E38(v0);
    ov96_021F5EC4(data);
    ov96_021E6454(data, v0->unk_13C);
    ov96_021F637C(v0);
}
