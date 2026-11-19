#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_96_ov96rival.h"

s32 ov96_0220C9F4(Ov96Rival *, s32, PokeathlonCourseData *);

void *ov96_0220C93C(PokeathlonCourseData *arg0, enum HeapID arg1);

void *ov96_0220C93C(PokeathlonCourseData *data, enum HeapID heapID) {
    PokeathlonCourseData *pokeathlonCourseData;
    u32 v0;
    unsigned int new_var;
    Ov96Rival *v1;
    u32 v3;
    u32 v2;
    Ov96RivalSet *dest;
    new_var = 0U;
    pokeathlonCourseData = data;
    v2 = PokeathlonCourse_GetParticipantCount(data);
    dest = Heap_Alloc(heapID, sizeof(Ov96RivalSet));
    MI_CpuFill8(dest, new_var, sizeof(Ov96RivalSet));
    dest->heapID = heapID;
    dest->course = data;
    v0 = 4 - v2;
    dest->count = v0;
    if (v0 > 3U) {
        GF_AssertFail();
    }
    v3 = (v3 = 0);
    if (((u32)(dest->count)) > new_var) {
        v1 = &dest->rivals[0];
        do {
            ov96_0220C9F4(v1, (u8)v2, pokeathlonCourseData);
            v2 += 1;
            v3 += 1;
            v1++;
        } while (v3 < ((u32)(dest->count)));
    }
    return dest;
}
