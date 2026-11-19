#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "sprite_system.h"

void ov96_021EB5BC(void *param_1);

void ov96_021F87D0(void *param_1);

u32 ov96_021F7C4C(PokeathlonCourseData *param_1);

u32 ov96_021F7C4C(PokeathlonCourseData *param_1) {
    UnkStruct_ov96_021F7934 *pvVar1;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    ov96_021EB5BC(pvVar1->unk_10);
    SpriteSystem_DrawSprites(pvVar1->spriteManager);
    ov96_021F87D0(pvVar1->unk_1C.scene);
    return 1;
}
