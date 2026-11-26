#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"

BOOL ov96_02215CD0(PokeathlonCourseData *param_1, int unused);

BOOL ov96_02215CD0(PokeathlonCourseData *data, int unused) {
    Ov96RdWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    GF_ASSERT(work != NULL);
    SpriteSystem_DrawSprites(work->spriteManager);
    return 1;
}
