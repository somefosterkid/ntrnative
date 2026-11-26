#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

BOOL ov96_02209738(PokeathlonCourseData *param_1, int unused);

BOOL ov96_02209738(PokeathlonCourseData *param_1, int unused) {
    Ov96_0220Data *pvVar1;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    SpriteSystem_DrawSprites(pvVar1->spriteManager);
    return 1;
}
