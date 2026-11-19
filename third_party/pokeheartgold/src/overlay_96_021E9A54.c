#include "global.h"

#include "pokeathlon/pokeathlon.h"

void *ov96_021E9A54(s32 param_1, PokeathlonCourseData *param_2, s32 param_3);

s32 ov96_021E86FC(void *, u32);
void ov96_021E9A68(u16 arg0, void *arg1);

void *ov96_021E9A54(s32 param_1, PokeathlonCourseData *param_2, s32 param_3) {
    void *pvVar1;

    pvVar1 = PokeathlonCourse_GetFieldBA4(param_2);
    return (void *)((u8 *)pvVar1 + param_1 * param_3);
}

void ov96_021E9A68(u16 arg0, void *arg1) {
    ov96_021E86FC(arg1, (u8)arg0);
}
