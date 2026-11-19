#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_02203468work.h"

void ov96_021EB5BC(void *param_1);

void ov96_02203CE4(void *param_1);

u32 ov96_02201B18(PokeathlonCourseData *param_1);

void ov96_021E9510(void *arg0);
void ov96_021E952C(void *arg0);
void ov96_021E93B4(void *, u32);
void ov96_021E9570(void *, u8);
void ov96_021E95F8(PokeathlonCourseData *, void *, u32, s32, u8, u8, u32);
void ov96_0221A56C(void *, u32);
u32 ov96_02201B34(PokeathlonCourseData *param_1);

u32 ov96_02201B18(PokeathlonCourseData *param_1) {
    Ov96_02203468Work *pvVar1;

    pvVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    ov96_02203CE4(pvVar1->unk_5E0);
    ov96_021EB5BC(pvVar1->unk_48);
    return 1;
}

u32 ov96_02201B34(PokeathlonCourseData *param_1) {
    u16 uVar1;
    void *arg0;
    u32 uVar2;

    arg0 = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(arg0);
    uVar2 = 0;
    do {
        uVar1 = PokeathlonCourse_GetField5F0_AtIndex(param_1, uVar2 & 0xff);
        ov96_021E95F8(param_1, arg0, uVar2, uVar1, 0, 3, 299);
        uVar2 = uVar2 + 1;
    } while ((s32)uVar2 < 4);
    ov96_021E93B4(arg0, 1);
    ov96_0221A56C(arg0, 8);
    ov96_021E952C(arg0);
    ov96_021E9570(arg0, 8);
    return 1;
}
