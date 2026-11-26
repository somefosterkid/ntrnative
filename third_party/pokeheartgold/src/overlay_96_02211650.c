#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_app_02210C98.h"

void ov96_021EB5BC(void *param_1);

BOOL ov96_02211650(PokeathlonCourseData *param_1, int unused);

void ov96_021E9510(void *arg0);
void ov96_021E952C(void *arg0);
void ov96_021E93B4(void *, u32);
void ov96_021E9570(void *, u8);
void ov96_021E95F8(PokeathlonCourseData *, void *, u32, s32, u8, u8, u32);
void ov96_0221A56C(void *, u32);
BOOL ov96_02211660(PokeathlonCourseData *param_1, int unused);

BOOL ov96_02211650(PokeathlonCourseData *param_1, int unused) {
    Ov96App_02210C98 *app;

    app = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    ov96_021EB5BC(app->unk_0);
    return 1;
}

BOOL ov96_02211660(PokeathlonCourseData *param_1, int unused) {
    u16 uVar1;
    void *arg0;
    u32 uVar2;

    PokeathlonCourse_GetHeapAllocPtr4(param_1);
    arg0 = PokeathlonCourse_GetGraphicsSystem(param_1);
    ov96_021E9510(arg0);
    uVar2 = 0;
    do {
        uVar1 = PokeathlonCourse_GetField5F0_AtIndex(param_1, uVar2 & 0xff);
        ov96_021E95F8(param_1, arg0, uVar2, uVar1, 0, 3, 300);
        uVar2 = uVar2 + 1;
    } while ((s32)uVar2 < 4);
    ov96_021E93B4(arg0, 1);
    ov96_0221A56C(arg0, 9);
    ov96_021E952C(arg0);
    ov96_021E9570(arg0, 9);
    return 1;
}
