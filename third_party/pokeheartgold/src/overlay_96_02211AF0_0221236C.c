#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_app_02210C98.h"
#include "overlay_96_ov96_course_hud.h"

void ov96_0221236C(PokeathlonCourseData *param_1);

void ov96_0221236C(PokeathlonCourseData *param_1) {
    Ov96App_02210C98 *pvVar1;
    u8 *puVar2;
    u8 uVar3;
    u32 uVar4;
    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    puVar2 = PokeathlonCourse_GetDataCopyArea(param_1);
    puVar2 = ov96_021E8A20(puVar2 + 0xf0);
    uVar3 = ((*((s32 *)(puVar2 + 0x1c))) >> 0x1c) & 3;
    uVar4 = uVar3;
    uVar3 = ((*((s32 *)(puVar2 + 0x20))) >> 0x19) & 0x7f;
    pvVar1->unk_814[uVar4] = (s8)uVar3;
    ov96_022148E8(pvVar1->unk_750, uVar3, uVar4);
    return;
}
