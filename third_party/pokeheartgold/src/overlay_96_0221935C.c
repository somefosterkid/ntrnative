#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_ov96_rd_work.h"

Ov96RdStats *ov96_0221935C(PokeathlonCourseData *arg0, u32 arg1, u32 arg2);

void ov96_02219398(Ov96RdObj *obj);
s32 sub_0200606C(u32, s32, s32);
void ov96_022193CC(u32 arg0, s32 arg1, s32 arg2);

Ov96RdStats *ov96_0221935C(PokeathlonCourseData *data, u32 arg1, u32 arg2) {
    Ov96RdWork *v0;
    Ov96RdStats *v1;

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
    v1 = &v0->stats[arg2 + arg1 * 3];
    if (v1 == NULL) {
        GF_AssertFail();
    }
    return v1;
}

void ov96_02219398(Ov96RdObj *obj) {
    obj->e = (obj->e + 1) % 3;
    obj->info = &obj->owner->obj.entries[obj->e];
}

void ov96_022193CC(u32 arg0, s32 arg1, s32 arg2) {
    s32 v0;

    if (arg1 != 0) {
        v0 = 3;
    } else {
        v0 = 4;
    }
    sub_0200606C((u16)arg0, v0, arg2);
}
