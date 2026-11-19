#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

u32 ov96_0220E5F4(PokeathlonCourseData *param_1);

u32 ov96_0220E5F4(PokeathlonCourseData *param_1) {
    UnkStruct_ov96_0220E3F0 *pvVar1;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    if (pvVar1 == NULL) {
        GF_AssertFail();
    }
    SpriteSystem_DrawSprites(pvVar1->spriteManager);
    return 1;
}
