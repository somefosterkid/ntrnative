#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_arena_work.h"

void ov96_021EB5BC(void *);

s32 ov96_021F08E4(PokeathlonCourseData *arg0);

s32 ov96_021F08E4(PokeathlonCourseData *data) {
    Ov96ArenaWork *work = PokeathlonCourse_GetHeapAllocPtr4(data);

    ov96_021EB5BC(work->unk_18);
    return 1;
}
