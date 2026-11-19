#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_app_021ed1d0.h"

void ov96_021EC790(PokeathlonCourseData *param_1, s32 param_2);

void ov96_021ECA70(PokeathlonCourseData *param_1, s32 param_2);

void ov96_021ECA70(PokeathlonCourseData *param_1, s32 param_2) {
    Ov96App_021ED1D0 *pvVar1;
    Ov96SpriteSetElem *piVar2;
    Ov96SpriteSetElem *piVar3;
    u32 uVar4;
    Ov96App_021ED1D0 *new_var;
    u32 uVar5;
    s32 iVar6;
    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    new_var = pvVar1;
    iVar6 = 0;
    uVar4 = 1;
    uVar5 = 2;
    do {
        piVar2 = ov96_021EAA04(new_var->spriteSet, uVar4 & 0xff);
        piVar3 = ov96_021EAA04(new_var->spriteSet, uVar5 & 0xff);
        ov96_021EAB38(piVar2, 0);
        ov96_021EAB38(piVar3, 0);
        uVar4 = uVar4 + 3;
        uVar5 = uVar5 + 3;
        iVar6 = iVar6 + 1;
    } while (iVar6 < 4);
    ov96_021EC790(param_1, param_2);
    return;
}
