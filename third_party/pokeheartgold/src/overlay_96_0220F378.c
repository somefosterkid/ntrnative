#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

UnkStruct_ov96_0220E3F0_Cell *ov96_0220F378(PokeathlonCourseData *arg0, u32 arg1, u32 arg2);

UnkStruct_ov96_0220E3F0_Cell *ov96_0220F378(PokeathlonCourseData *data, u32 arg1, u32 arg2) {
    UnkStruct_ov96_0220E3F0 *v0;
    UnkStruct_ov96_0220E3F0_Cell *v1;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (arg1 >= 4U) {
        GF_AssertFail();
    }
    if (arg2 >= 3U) {
        GF_AssertFail();
    }
    v1 = &v0->cells[arg2 + (arg1 * 3)];
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return v1;
}

u16 ov96_0220F3B4(UnkStruct_ov96_0220E3F0_Cell *arg0);

u16 ov96_0220F3B4(UnkStruct_ov96_0220E3F0_Cell *arg0) {
    return (u16)(arg0->unk_0 * 2);
}

s32 ov96_0220F3BC(UnkStruct_ov96_0220E3F0_Record *arg0);

s32 ov96_0220F3BC(UnkStruct_ov96_0220E3F0_Record *arg0) {
    u32 v0;

    v0 = (u32)(arg0->unk_4 << 0x1B) >> 0x1D;
    switch (v0) {
    case 3:
        return 1;
    case 4:
        return 2;
    case 5:
        return 3;
    case 6:
        return 5;
    default:
        GF_AssertFail();
        return 0;
    }
}
