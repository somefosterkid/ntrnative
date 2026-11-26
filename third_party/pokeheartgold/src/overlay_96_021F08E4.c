#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_arena_work.h"

void ov96_021EB5BC(void *);

BOOL ov96_021F08E4(PokeathlonCourseData *arg0, int unused);

BOOL ov96_021F08E4(PokeathlonCourseData *data, int unused) {
    Ov96ArenaWork *work = PokeathlonCourse_GetHeapAllocPtr4(data);

    ov96_021EB5BC(work->unk_18);
    return 1;
}
