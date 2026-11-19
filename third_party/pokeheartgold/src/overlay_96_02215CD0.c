#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"

u32 ov96_02215CD0(PokeathlonCourseData *param_1);

u32 ov96_02215CD0(PokeathlonCourseData *data) {
    Ov96RdWork *work;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    GF_ASSERT(work != NULL);
    SpriteSystem_DrawSprites(work->spriteManager);
    return 1;
}
